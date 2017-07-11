#include "picccheck.h"
extern "C" {
#include "dll_api.h"
}
Picccheck::Picccheck()
{
    stop= false;
}

void Picccheck::run()
{
        int iRet = -1;
        int i = 0;
        unsigned char mode;
        unsigned char cardType[2];
        unsigned char serialNo [11];
        APDU_SEND ApduSend;
        APDU_RECV ApduRecv;
        QString str,strTemp;

        iRet = DLL_PiccOpen();
        if(iRet != 0)
        {
            str.clear();
            str.sprintf("PICC open failed, return value:%d\n",iRet);
           emit sendMessage(str);
            return;
        }
       emit  sendMessage("PICC open success\n");


        mode = 'A';
        emit sendMessage("Please put card on\n");

        while(1)
        {
            iRet = DLL_PiccCheck(mode, cardType, serialNo);
            if(iRet == 0)
            {
                break;
            }
            if(stop)
            {
                DLL_PiccClose();
                return;
            }
        }
       emit sendMessage("Detect Card success!!!\r\n");
        str.clear();
        str.sprintf("Card type is: %02x %02x\n",cardType[0],cardType[1]);
        emit sendMessage(str);
        str.clear();
        str.sprintf("Serial numver is: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",serialNo[0],serialNo[1],serialNo[2],serialNo[3],serialNo[4],serialNo[5],serialNo[6],serialNo[7],serialNo[8],serialNo[9],serialNo[10]);
        emit sendMessage(str);

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
                emit sendMessage(str);
                str.clear();
                strTemp.clear();
                for(i = 0 ; i < ApduRecv.LenOut; i++) str+= strTemp.sprintf("%02x ",ApduRecv.DataOut[i]);
                str+="\n";
                emit sendMessage(str);

                str.clear();
                str.sprintf("SWA,SWB is:%02x,%02x\n",ApduRecv.SWA,ApduRecv.SWB);
                emit sendMessage(str);

            }
            else
            {
                str.clear();
                str.sprintf("Command Exchange failed,return value:%d\n",iRet);
                emit sendMessage(str);
                DLL_PiccClose();
                return;
            }
            emit sendMessage("Please remove card\n");
            iRet = DLL_PiccRemove();
            if(iRet != 0) {emit sendMessage("still detect card\n"); DLL_PiccClose(); return;}
           emit  sendMessage("Card removed\n");

            DLL_PiccClose();
            emit sendMessage("PICC close，Test Success!!!\n");
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
            if(iRet != 0) {emit sendMessage("M1 card authority failed!!!\n"); DLL_PiccClose(); return;}
            emit sendMessage("M1 card authority Success!!!\n");

            iRet = DLL_PiccM1WriteBlock(blkNo, blkValue);
            if(iRet != 0) {emit sendMessage("M1 card write block failed!!!\n"); DLL_PiccClose(); return;}
           emit  sendMessage("M1 card write block Success!!!\n");

            iRet = DLL_PiccM1ReadBlock(blkNo, blkValue);
            if(iRet != 0) {emit sendMessage("M1 card read block failed!!!\n"); DLL_PiccClose(); return;}
            str.clear();
            str.sprintf("M1 card read block Success!!!, blkValue is :%s\n",blkValue);
            emit sendMessage(str);

            iRet = DLL_PiccM1Operate('+', blkNo, value, updateBlkNo);
            if(iRet != 0) {emit sendMessage("M1 card charge money failed!!!\n"); DLL_PiccClose(); return;}
          emit   sendMessage("M1 card charge money Success!!!\n");

            iRet = DLL_PiccM1Operate('-', blkNo, value, updateBlkNo);
            if(iRet != 0) {emit sendMessage("M1 card deduct money failed!!!\n"); DLL_PiccClose(); return;}
            emit sendMessage("M1 card deduct money Success!!!\n");

            iRet = DLL_PiccM1Operate('=', blkNo, value, updateBlkNo);
            if(iRet != 0) {emit sendMessage("M1 card transfer failed!!!\n"); DLL_PiccClose(); return;}
           emit sendMessage("M1 card transfer Success!!!\n");

            iRet = DLL_PiccM1Operate('=', updateBlkNo, value, blkNo);
            if(iRet != 0) {emit sendMessage("M1 card restore failed!!!\n"); DLL_PiccClose(); return;}
          emit  sendMessage("M1 card restorey Success!!!\n");

            DLL_PiccClose();
           emit  sendMessage("PICC close，Test Success!!!\n");
        }
        else
        {
            DLL_PiccClose();
            emit  sendMessage("PICC close，NOT A,B or M1 card, Test Failed!!!\n");
        }

}

void Picccheck::stopPicc()
{
    stop = true;
}

