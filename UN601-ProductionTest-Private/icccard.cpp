#include "icccard.h"
extern "C" {
#include "dll_api.h"
}
icccard::icccard(QObject *parent) : QThread(parent)
{
    mStopflag =false;
    mIsRun = false;
}

void icccard::iccdeal()
{
    unsigned char atr[40];
    APDU_SEND	ApduSend;
    APDU_RECV	ApduRecv;
    int iret,i;
    QString str,strTemp;

    if(mIsRun == true)
    {
       // return;
    }
    ApduSend.Command[0]=0x00;
    ApduSend.Command[1]=0xa4;
    ApduSend.Command[2]=0x04;
    ApduSend.Command[3]=0x00;
    ApduSend.Lc=0x0e;
    ApduSend.Le=256;
    sendMessage("ICC Test start");
    iret = DLL_IccCheck(0);
    str.clear();
    str.sprintf("Icc Card Not Insert:%d\n",iret);
    if(iret) {sendMessage(str); return ;}
    sendMessage("Detect IC Card Inserted\n");

    iret=DLL_IccOpen(0,1,atr);
    str.clear();
    str.sprintf("Icc Reset failed::%d\n",iret);
    if(iret) {sendMessage(str); DLL_IccClose(0);return ;}
    str.clear();
    str.sprintf("ICC Reset OK.atr is: ");
    strTemp.clear();
    for(i = 0 ; i < atr[0]; i++) str+= strTemp.sprintf("%02x ",atr[i]);
    str+="\n";
    sendMessage(str);

    memcpy(ApduSend.DataIn,"1PAY.SYS.DDF01",ApduSend.Lc);
    iret=DLL_IccCommand(0,&ApduSend,&ApduRecv);
    if (iret==0){
        str.clear();
        str.sprintf("datalen out is:%d\n",ApduRecv.LenOut);
        sendMessage(str);
        str.clear();
        strTemp.clear();
        str.sprintf("data out is: ");
        for(i = 0 ; i < ApduRecv.LenOut; i++) str+= strTemp.sprintf("%02x ",ApduRecv.DataOut[i]);
        str+="\n";
        sendMessage(str);

        str.clear();
        str.sprintf("SWA,SWB is:%02x,%02x\n",ApduRecv.SWA,ApduRecv.SWB);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("Icc command failed: %d \n",iret);
        sendMessage(str);
    }
    DLL_IccClose(0);
    sendMessage("ICC Test Success!!!");
    mIsRun = true;
}

void icccard::piccdeal()
{
    int iRet = -1;
    int i = 0;
    unsigned char mode;
    unsigned char cardType[2];
    unsigned char serialNo [11];
    APDU_SEND ApduSend;
    APDU_RECV ApduRecv;
    QString str,strTemp;
    if(mIsRun == true)
    {
     //   return;
    }
    iRet = DLL_PiccOpen();
    if(iRet != 0)
    {
        str.clear();
        str.sprintf("PICC open failed, return value:%d\n",iRet);
        sendMessage(str);
        mIsRun = true;
        return;
    }
    sendMessage("PICC open success\n");


    mode = 'A';
    sendMessage("Please put card on\n");

    while(1)
    {
        iRet = DLL_PiccCheck(mode, cardType, serialNo);
        if(iRet == 0)
        {
            break;
        }
        if(mStopflag)
        {
            DLL_PiccClose();
            mIsRun = true;
            return;
        }
    }
    sendMessage("Detect Card success!!!\r\n");
    str.clear();
    str.sprintf("Card type is: %02x %02x\n",cardType[0],cardType[1]);
    sendMessage(str);
    str.clear();
    str.sprintf("Serial numver is: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",serialNo[0],serialNo[1],serialNo[2],serialNo[3],serialNo[4],serialNo[5],serialNo[6],serialNo[7],serialNo[8],serialNo[9],serialNo[10]);
    sendMessage(str);

    if(mode == 'A' || mode == 'B')
    {
        ApduSend.Command[0]=0x00;
        ApduSend.Command[1]=0xa4;
        ApduSend.Command[2]=0x04;
        ApduSend.Command[3]=0x00;
        ApduSend.Lc=0x0e;
        ApduSend.Le=256;
        memcpy(ApduSend.DataIn,"2PAY.SYS.DDF01",ApduSend.Lc);

        iRet = DLL_PiccCommand(&ApduSend, &ApduRecv);
        if(iRet == 0)
        {
            str.clear();
            str.sprintf("datalen out is:%d\n",ApduRecv.LenOut);
            sendMessage(str);
            str.clear();
            strTemp.clear();
            for(i = 0 ; i < ApduRecv.LenOut; i++) str+= strTemp.sprintf("%02x ",ApduRecv.DataOut[i]);
            str+="\n";
            sendMessage(str);

            str.clear();
            str.sprintf("SWA,SWB is:%02x,%02x\n",ApduRecv.SWA,ApduRecv.SWB);
            sendMessage(str);

        }
        else
        {
            str.clear();
            str.sprintf("Command Exchange failed,return value:%d\n",iRet);
            sendMessage(str);
            DLL_PiccClose();
            mIsRun = true;
            return;
        }
        sendMessage("Please remove card\n");
        iRet = DLL_PiccRemove();
        if(iRet != 0) {sendMessage("still detect card\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("Card removed\n");

        DLL_PiccClose();
        sendMessage("PICC close，Test Success!!!\n");
    }
    else if(mode == 'M')
    {
        unsigned char blkNo = 4;
        unsigned char  updateBlkNo = 8;
        unsigned char pwd [4];
        unsigned char blkValue[16];
        unsigned char value[4];

        memcpy(pwd,"\xFF\xFF\xFF",3);
        iRet = DLL_PiccM1Authority('A', blkNo, pwd, serialNo);
        if(iRet != 0) {sendMessage("M1 card authority failed!!!\n");mIsRun = true; DLL_PiccClose(); return;}
        sendMessage("M1 card authority Success!!!\n");

        iRet = DLL_PiccM1WriteBlock(blkNo, blkValue);
        if(iRet != 0) {sendMessage("M1 card write block failed!!!\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("M1 card write block Success!!!\n");

        iRet = DLL_PiccM1ReadBlock(blkNo, blkValue);
        if(iRet != 0) {sendMessage("M1 card read block failed!!!\n");mIsRun = true; DLL_PiccClose(); return;}
        str.clear();
        str.sprintf("M1 card read block Success!!!, blkValue is :%s\n",blkValue);
        sendMessage(str);

        iRet = DLL_PiccM1Operate('+', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card charge money failed!!!\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("M1 card charge money Success!!!\n");

        iRet = DLL_PiccM1Operate('-', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card deduct money failed!!!\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("M1 card deduct money Success!!!\n");

        iRet = DLL_PiccM1Operate('=', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card transfer failed!!!\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("M1 card transfer Success!!!\n");

        iRet = DLL_PiccM1Operate('=', updateBlkNo, value, blkNo);
        if(iRet != 0) {sendMessage("M1 card restore failed!!!\n"); mIsRun = true;DLL_PiccClose(); return;}
        sendMessage("M1 card restorey Success!!!\n");

        DLL_PiccClose();
        sendMessage("PICC close，Test Success!!!\n");
        mIsRun = true;
    }
    else
    {
        DLL_PiccClose();
        sendMessage("PICC close，NOT A,B or M1 card, Test Failed!!!\n");
    }
    mIsRun = true;
}

void icccard::scandeal()
{
    if(mIsRun == true)
    {
      //  return;
    }
    char str[100]={0};
    int ret = -1;

    ret=DLL_ScanOpen();
    if(ret==0)
    {
        sendMessage("The scanner open successful!\n");
        ret=DLL_ScanTrigger();
    }
    if(ret == 0)
    {
        sendMessage("The scanner trigger successful!\n");
        sendMessage("Please scan code!\n");
        ret=DLL_ScanRead(str);
        printf("str is %s\r\n",str);
        if(ret == 0)
        {
            sendMessage("Read successful!\n");
            sendMessage("Data is:");
            sendMessage(str);

        }
    }
    DLL_ScanClose();
    mIsRun = true;

}

void icccard::mcrdeal()
{
    int iRet = -1;
    unsigned char track1Len[1];
    unsigned char track2Len[1];
    unsigned char track3Len[1];
    unsigned char track1[256];
    unsigned char track2[256];
    unsigned char track3[256];
    QString str;
     static int totalCount = 0;
     static int successCount = 0;
    if(mIsRun == true)
    {
       // return;
    }
    iRet = DLL_McrOpen();
    str.clear();
    str.sprintf("MCR open failed, return value is: %d\n",iRet);
    if(iRet!=0) { sendMessage(str); mIsRun = true;return;}
    else sendMessage("MCR open success!\n");

    iRet = DLL_McrReset();
    str.clear();
    str.sprintf("MCR reset failed, return value is: %d\n",iRet);
    if(iRet!=0) {sendMessage(str); mIsRun = true;DLL_McrClose(); return;}
    else sendMessage("MCR reset success!\n");

    sendMessage("Please swipe card...\n");

    while(1)
    {
        iRet = DLL_McrCheck();
        if(iRet == 0)
        {
            break;
        }
        if(mStopflag)
        {
            DLL_McrClose();
            mIsRun = true;
            return;
        }
    }
    totalCount++;
    str.clear();
    str.sprintf("swipe count:%d\n",totalCount);
    sendMessage(str);
    sendMessage("Detect card swiped\n");
    memset(track1,0,sizeof(track1));
    memset(track2,0,sizeof(track2));
    memset(track3,0,sizeof(track3));
    iRet = DLL_McrRead(track1Len,track1,track2Len,track2,track3Len,track3);
    if(iRet > 0)
    {
        successCount++;
        str.clear();
        str.sprintf("success count:%d\n",successCount);
        sendMessage(str);

        str.clear();
        QString string;
        string.clear();
        if(iRet <= 7){
            if((iRet & 0x01) == 0x01)
            {
                str.clear();
                //string = getStringFromUnsignedChar(track1,track1Len[0]);

                str.sprintf("track1:%s",track1);
                str.append(string);
                str+="\n";
                sendMessage(str);
            }
            if((iRet & 0x02) == 0x02)
             {
                str.clear();
                //string = getStringFromUnsignedChar(track2,track2Len[0]);
                str.sprintf("track2:%s",track2);
                str.append(string);
                str+="\n";
                sendMessage(str);
              }
            if((iRet & 0x04) == 0x04)
            {
                str.clear();
                //string = getStringFromUnsignedChar(track3,track3Len[0]);
                str.sprintf("track3:%s",track3);
                str.append(string);
                str+="\n";
                sendMessage(str);
            }
        }else{
            string = "McrRead data error\n";
            str += string;
            sendMessage(str);
        }

        DLL_McrClose();
        sendMessage("\nMCR close, Test Success!!!\n");
    }
    else
    {
        str.clear();
        str.sprintf("success count:%d\n",successCount);
        sendMessage(str);
        sendMessage("Read card failed!!!\n");
        DLL_McrClose();
    }
    mIsRun = true;
}

void icccard::stopTheThread()
{
    mStopflag =true;
    qDebug("222222222222222222222222222222222222222222222222222222222222222222222222222222220");
}

