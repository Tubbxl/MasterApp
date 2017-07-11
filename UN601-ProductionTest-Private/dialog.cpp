#include "dialog.h"
#include "ui_dialog.h"
#include <QtGui>
#include <QStyleFactory>
#include <QDateTime>
#include "settingmenu.h"

extern "C" {
#include "dll_api.h"
}

QString getStringFromUnsignedChar(unsigned char *str, int strLen)
{

    QString s;
    QString result = "";

    // Print String in Reverse order....
    for ( int i = 0; i<strLen; i++)
        {
           s = QString("%1").arg(str[i],0,16);

           if(s == "0"){
              s="00";
             }
         result.append(s);

         }
   return result;
}


void Dialog::sleep(int msec)//自定义Qt延时函数,单位毫秒
{
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while (1)
    {
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= msec)
        {
            break;
        }
    }
}



Dialog::Dialog(const QString *type) :
    //QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    TYPE = *type;
    connect(this,SIGNAL(sendMessage(QString)),this,SLOT(receiveMessage(QString)));//
    QObject::connect( ui->btnExit, SIGNAL(clicked()), qApp, SLOT(quit()) );
    //iret = DLL_ApiInit();
    //if(iret != 0) sendMessage("DLL_ApiInit failed!!!\n");
    hideAllCtl();
    if(TYPE ==FUNC_TYPE_W_SN_TEST )
    {
        ui->lineEdit->show();
        ui->btnSn->show();
        sendMessage("Write SN Test! Please wait\n");
    }
    if(0==QString::compare(TYPE,FUNC_TYPE_IC_CARD_TEST))
                sendMessage("IC card test start,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_PRINTER_TEST))
                sendMessage("Printer test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_NFC_TEST))
                sendMessage("NFC Test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_MSR_TEST))
                sendMessage("MSR Test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_BAR_CODE_TEST))
                sendMessage("Barcode (1D 2D)Test,Please wait\n");
     loadStyleSheet("uistyle1");
     mtimer = new QTimer(this);
     connect(mtimer,SIGNAL(timeout()),this,SLOT(InitView()));
     mtimer->start(500);
     //InitView();
}


Dialog::Dialog(const QString *type,int mode)
{
    TYPE = *type;
   // connect(this,SIGNAL(sendMessage(QString)),this,SLOT(receiveMessage(QString)));//
    if(TYPE ==FUNC_TYPE_W_SN_TEST )
    {
        sendMessage("Write SN Test! Please wait\n");
    }
    if(0==QString::compare(TYPE,FUNC_TYPE_IC_CARD_TEST))
                sendMessage("IC card test start,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_PRINTER_TEST))
                sendMessage("Printer test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_NFC_TEST))
                sendMessage("NFC Test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_MSR_TEST))
                sendMessage("MSR Test,Please wait\n");
    else if(0==QString::compare(TYPE,FUNC_TYPE_BAR_CODE_TEST))
                sendMessage("Barcode (1D 2D)Test,Please wait\n");
     mtimer = new QTimer(this);
     connect(mtimer,SIGNAL(timeout()),this,SLOT(InitView()));
     mtimer->start(500);
}

void Dialog::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setContainerPointer(QWidget* ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void Dialog::InitView()
{
    if(TYPE == FUNC_TYPE_MSR_TEST)
    {
        MSRtest();
    }
    else if(TYPE == FUNC_TYPE_IC_CARD_TEST)
    {
        ICC_test();
    }
    else if(TYPE == FUNC_TYPE_PRINTER_TEST)
    {
        printertest();
    }
    else if(TYPE == FUNC_TYPE_NFC_TEST)
    {
        Picctest();
    }
    else if(TYPE == FUNC_TYPE_BAR_CODE_TEST)
    {

        scanTest();
    }
    else if(TYPE == FUNC_TYPE_W_SN_TEST)
    {
         sendMessage("Please scan code or input SN!\n");
        writeSn();
    }
    else if(TYPE == FUNC_TYPE_PCI_TEST)
    {
        PCI_Test();
    }
    mtimer->stop();
}

void Dialog::ICC_test()
{
     on_btnICC_clicked();
}

void Dialog::Sys_test()
{
    on_btnSys_clicked();
}

void Dialog::Picctest()
{
    on_btnPicc_clicked();
}

void Dialog::MSRtest()
{
    on_btnMSR_clicked();

}

void Dialog::printertest()
{

    on_btnPrinter_clicked();

}

void Dialog::scanTest()
{

    on_btnScan_clicked();
}
void Dialog::writeSn()
{
    char str[100]={0};
    int ret = -1;
    //ui->textBrowser->clear();

    ret=DLL_ScanOpen();
    if(ret==0)
    {
       // sendMessage("The scanner open successful!\n");
        ret=DLL_ScanTrigger();
    }
    if(ret == 0)
    {
        //sendMessage("The scanner trigger successful!\n");
        //sendMessage("Please scan code!\n");
        ret=DLL_ScanRead(str);
        printf("str is %s\r\n",str);
        if(ret == 0)
        {
            //sendMessage("Read successful!\n");
            //sendMessage("Data is:");
           // sendMessage(str);
            ui->lineEdit->setText(str);
        }
    }
    DLL_ScanClose();
    //on_btnSn_clicked();
}

void Dialog::PCI_Test()
{
    on_btnPCI_clicked();
}
void Dialog::hideAllCtl()
{
    ui->btnSn->close();
    ui->lineEdit->close();
    ui->frame->close();
}

void Dialog::receiveMessage(QString msg)
{
    if(msg.isEmpty()) ui->textBrowser->clear();
    else ui->textBrowser->append(msg);//添加字符串作为一个段落到TextBrowser控件中
    QApplication::processEvents();
}



void Dialog::on_btnSys_clicked()
{
    int iret = -1;
    unsigned char version[7];
    unsigned char timebuf[7];
    unsigned char serialNo[14];
    QString str;
    int i = 0;

    ui->textBrowser->clear();
    sendMessage("SYS function test...\n");

    iret = DLL_SysBeep();
    if(iret != 0) sendMessage("Sound beeper test failed!!!\n");

    for(i = 0 ; i < 7; i++)
    {
        iret = DLL_SysBeef(i, 200);
        if(iret != 0) sendMessage("Sound beeper test failed!!!\n");
        sleep(500);
    }

    sendMessage("Sound beeper test success!!!\n");

    for(i =0 ; i < 4; i++)
    {
        iret = DLL_SysLedControl(i,1);
        sleep(1000);
    }
    iret = DLL_SysLedControl(0,0);
    iret = DLL_SysLedControl(1,0);
    iret = DLL_SysLedControl(2,0);
    iret = DLL_SysLedControl(3,0);
    sendMessage("led test success!!!\n");

    memset(version,0,sizeof(version));
    memset(timebuf,0,sizeof(timebuf));

    iret = DLL_SysGetVersion(version);
    if(iret) sendMessage("get version failed!!!\n");
    else
    {
        sendMessage("get version success!!!\n");
        str.clear();
        str.sprintf("MAX32550 boot version is: %d.%d.%d, OS version is: %d.%d.%d.\n",version[0],version[1],version[2],version[3],version[4],version[5]);
        sendMessage(str);
    }

    iret = DLL_SysGetDateTime(timebuf);
    if(iret) sendMessage("get rtc failed!!!\n");
    else
    {
        sendMessage("get rtc success!!! \n");
        str.clear();;
        str.sprintf(" the current time is:%02x%02x%02x%02x%02x%02x\n",timebuf[0],timebuf[1],timebuf[2],timebuf[3],timebuf[4],timebuf[5]);
        sendMessage(str);
    }
    iret = DLL_SysReadSecurityChipSN(serialNo);
    if(iret) sendMessage("get secure chip sn failed!!!\n");
    else
    {
        sendMessage("get secure chip sn success!!! \n");
        str.clear();;
        str.sprintf(" the chip  sn is:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",serialNo[0],serialNo[1],serialNo[2],serialNo[3],serialNo[4],serialNo[5],
                serialNo[6],serialNo[7],serialNo[8],serialNo[9],serialNo[10],serialNo[11],serialNo[12]);
        sendMessage(str);
    }
    sendMessage("system module function test success!!!\n");
}

void Dialog::on_btnICC_clicked()
{
    unsigned char atr[40];
    APDU_SEND	ApduSend;
    APDU_RECV	ApduRecv;
    int iret,i;
    QString str,strTemp;

    ApduSend.Command[0]=0x00;
    ApduSend.Command[1]=0xa4;
    ApduSend.Command[2]=0x04;
    ApduSend.Command[3]=0x00;
    ApduSend.Lc=0x0e;
    ApduSend.Le=256;

    ui->textBrowser->clear();
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

}

void Dialog::on_btnPicc_clicked()
{
    int iRet = -1;
    int i = 0;
    unsigned char mode;
    unsigned char cardType[2];
    unsigned char serialNo [11];
    APDU_SEND ApduSend;
    APDU_RECV ApduRecv;
    QString str,strTemp;

    ui->textBrowser->clear();
    iRet = DLL_PiccOpen();
    if(iRet != 0)
    {
        str.clear();
        str.sprintf("PICC open failed, return value:%d\n",iRet);
        sendMessage(str);
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
            return;
        }
        sendMessage("Please remove card\n");
        iRet = DLL_PiccRemove();
        if(iRet != 0) {sendMessage("still detect card\n"); DLL_PiccClose(); return;}
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
        if(iRet != 0) {sendMessage("M1 card authority failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card authority Success!!!\n");

        iRet = DLL_PiccM1WriteBlock(blkNo, blkValue);
        if(iRet != 0) {sendMessage("M1 card write block failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card write block Success!!!\n");

        iRet = DLL_PiccM1ReadBlock(blkNo, blkValue);
        if(iRet != 0) {sendMessage("M1 card read block failed!!!\n"); DLL_PiccClose(); return;}
        str.clear();
        str.sprintf("M1 card read block Success!!!, blkValue is :%s\n",blkValue);
        sendMessage(str);

        iRet = DLL_PiccM1Operate('+', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card charge money failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card charge money Success!!!\n");

        iRet = DLL_PiccM1Operate('-', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card deduct money failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card deduct money Success!!!\n");

        iRet = DLL_PiccM1Operate('=', blkNo, value, updateBlkNo);
        if(iRet != 0) {sendMessage("M1 card transfer failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card transfer Success!!!\n");

        iRet = DLL_PiccM1Operate('=', updateBlkNo, value, blkNo);
        if(iRet != 0) {sendMessage("M1 card restore failed!!!\n"); DLL_PiccClose(); return;}
        sendMessage("M1 card restorey Success!!!\n");

        DLL_PiccClose();
        sendMessage("PICC close，Test Success!!!\n");
    }
    else
    {
        DLL_PiccClose();
        sendMessage("PICC close，NOT A,B or M1 card, Test Failed!!!\n");
    }
}

void Dialog::on_btnMSR_clicked()
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

    ui->textBrowser->clear();
    iRet = DLL_McrOpen();
    str.clear();
    str.sprintf("MCR open failed, return value is: %d\n",iRet);
    if(iRet!=0) { sendMessage(str); return;}
    else sendMessage("MCR open success!\n");

    iRet = DLL_McrReset();
    str.clear();
    str.sprintf("MCR reset failed, return value is: %d\n",iRet);
    if(iRet!=0) {sendMessage(str); DLL_McrClose(); return;}
    else sendMessage("MCR reset success!\n");

    sendMessage("Please swipe card...\n");

    while(1)
    {
        iRet = DLL_McrCheck();
        if(iRet == 0)
        {
            break;
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
}

void Dialog::on_btnPrinter_clicked()
{
    int iRet = -1;
    int tempValue;
    QString str;

    ui->textBrowser->clear();
    iRet = DLL_PrnInit();
    if(iRet != 0) {sendMessage("Printer initial failed!!!\n"); return;}
    sendMessage("Printer initial success!!!\n");

    sendMessage("Start send data into printer buffer...\n");

     DLL_PrnSetFont(16, 16, 0x33);
     DLL_PrnStr("   聯合信用卡處理中心\n");
     DLL_PrnSetFont(24, 24, 0x00);
     DLL_PrnStr("-------------------------------\n");
     DLL_PrnStr("     拍檔科技股份有限公司\n");
     DLL_PrnStr("\n");
     DLL_PrnStr("商店代號:   100010001\n");
     DLL_PrnStr("端末機代號:  00026715\n");
     DLL_PrnStr("-------------------------------\n");
     DLL_PrnStr("卡別(CARD Type): VISA\n");
     DLL_PrnStr("卡號(CARD NO):12345******23456\n");
     DLL_PrnStr("交易類型(TXN. TYPE):00 Sale\n");
     DLL_PrnStr("卡有效期(EXP. DATE):2023/08\n");
     DLL_PrnStr("-------------------------------\n");
     DLL_PrnStr("批次號碼(BATCH NO):  000023\n");
     DLL_PrnStr("調閱編號(Inv. No.):  000018\n");
     DLL_PrnStr("授權碼(AUTH NO.):   987654\n");
     DLL_PrnStr("交易日期(DATE/TIME):\n");
     DLL_PrnStr("2016/11/28 16:46:32\n");
     DLL_PrnStr("序號(REF. NO):200801280015\n");
     DLL_PrnStr("\n");
     DLL_PrnStr("\n");
     DLL_PrnSetFont(16, 16, 0x33);
     DLL_PrnStr("總計(Total): NT 888\n");
     DLL_PrnSetFont(24, 24, 0x00);
     DLL_PrnStr("-------------------------------\n");
     DLL_PrnStr("\n");
     DLL_PrnSetFont(16, 16, 0x00);
     DLL_PrnStr("持卡人簽名(CARDHOLDER SIGNATURE)\n");
     DLL_PrnSetFont(16, 16, 0x33);
     DLL_PrnStr("\n");
     DLL_PrnStr("     免    簽    名\n");
     DLL_PrnStr("\n");
     DLL_PrnSetFont(4, 24, 0x00);
     DLL_PrnStr("-------------------------------\n");
     DLL_PrnStr("\n");
     DLL_PrnStr("\n");
     DLL_PrnStr("\n");
     DLL_PrnStr("\n");

     sendMessage("Send data into printer buffer success!!!\n");

    sendMessage("Start print...\n");
    iRet = DLL_PrnStart();
    DLL_PrnGetTemperature(&tempValue);
    str.clear();
    str.sprintf("printer temperautre is:%d\n",tempValue);
    sendMessage(str);
    if(iRet != 0)
    {
        if(iRet == -4009) sendMessage("printer cover closed!!!\n"); //printer cover closed.
        if(iRet == -4002) sendMessage("printer no paper\n");//no paper
        if(iRet == -4005) sendMessage("printer over heat\n");//over heat
        sendMessage("print failed!!!\n");
        return;
    }
    sendMessage("Print ticket test success!!!\n");
}

void Dialog::on_btnScan_clicked()
{
    char str[100]={0};
    int ret = -1;

    ui->textBrowser->clear();
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

}

void Dialog::on_btnPCI_clicked()
{
    int iret = -1;
    QString str;
    PinContext ctx;
    unsigned char pinBlock[9],outKsn[10],macOut[9],desOut[25];
    unsigned char keydata[100],bdk[16],ksn[10],cardno[50],amount[15],datain[100];


    ui->textBrowser->clear();
   //write Master key
    memset(keydata,0,sizeof(keydata));
    memcpy(keydata,"\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11",16);
    iret = DLL_PciWriteMKey(0,0,16,keydata,0);
    str.clear();
    str.sprintf("DLL_PciWriteMKey,PIN_MK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(keydata,"\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22\x22",16);
    iret = DLL_PciWriteMKey(1,0,16,keydata,0);
    str.clear();
    str.sprintf("DLL_PciWriteMKey,MAC_MK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(keydata,"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33",24);
    iret = DLL_PciWriteMKey(2,0,24,keydata,0);
    str.clear();
    str.sprintf("DLL_PciWriteMKey,DES_MK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(keydata,"\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44\x44",16);
    iret = DLL_PciWriteMKey(3,0,16,keydata,0);
    str.clear();
    str.sprintf("DLL_PciWriteMKey,FIX_MK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    //write session key
    memcpy(keydata,"\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55",16);
    iret = DLL_PciWriteSKey(4,0,16,keydata,0x81,0);
    str.clear();
    str.sprintf("DLL_PciWriteSKey,PINK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(keydata,"\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66\x66",16);
    iret = DLL_PciWriteSKey(5,0,16,keydata,0x81,0);
    str.clear();
    str.sprintf("DLL_PciWriteSKey,MACK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(keydata,"\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77\x77",24);
    iret = DLL_PciWriteSKey(6,0,24,keydata,0x81,0);
    str.clear();
    str.sprintf("DLL_PciWriteSKey,DESK:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    memcpy(bdk,"\x6A\xC2\x92\xFA\xA1\x31\x5B\x4D\x85\x8A\xB3\xA3\xD7\xD5\x93\x3A",16);
    memcpy(ksn,"\xFF\xFF\x98\x76\x54\x32\x10\xE0\x00\x00",16);
    iret = DLL_PciWriteDukptKey(7,1,16,bdk,10,ksn,0);
    str.clear();
    str.sprintf("DLL_PciWriteDukptKey:%d\n",iret);
    if(iret) {sendMessage(str); return ;}

    sendMessage("write key ok\r\n");

    memset(ctx.prompt,0,sizeof(ctx.prompt));
    memset(ctx.font,0,sizeof(ctx.font));
    memset(ctx.background,0,sizeof(ctx.background));
    memcpy(ctx.prompt,"Pls cover the keypad with hand and enter PIN:",strlen("Pls cover the keypad with hand and enter PIN:"));
    printf("prompt len is %d\r\n",strlen("Pls cover the keypad with hand and enter PIN:"));
    memcpy(ctx.font,"\xff\x00\xff\x14",4);
    memcpy(ctx.background,"\xff\xff\xff",3);

    //pink pin
    memcpy(cardno,"1234567890123456",16);
    memset(amount,0,sizeof(amount));
    memcpy(amount,"1234.56",7);
    mPtrContainer->setDisabled(true);
    iret = DLL_PciGetPin(0,4,12,16,cardno,0,pinBlock,1,amount,0,&ctx);
    mPtrContainer->setEnabled(true);
    if(iret == 0 )
    {
        str.clear();
        str.sprintf("PINK PINBLOCK is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",pinBlock[0],pinBlock[1],pinBlock[2],pinBlock[3],pinBlock[4],pinBlock[5],pinBlock[6],pinBlock[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetPin is %d\r\n",iret);
        sendMessage(str);
    }


    //fix pin
    mPtrContainer->setDisabled(true);
    iret = DLL_PciGetPinByFixKey(0,4,12,16,cardno,0,pinBlock,1,amount,0,&ctx);
    mPtrContainer->setEnabled(true);
    if(iret == 0 )
    {
        str.clear();
        str.sprintf("FIXK PINBLOCK is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",pinBlock[0],pinBlock[1],pinBlock[2],pinBlock[3],pinBlock[4],pinBlock[5],pinBlock[6],pinBlock[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetPinByFixKey is %d\r\n",iret);
        sendMessage(str);
    }

    //dukpt pin
    memcpy(cardno,"4012345678909",strlen("4012345678909"));
    mPtrContainer->setDisabled(true);
    iret = DLL_PciGetPinDukpt(1,4,12,13,cardno,0,pinBlock,1,amount,0,outKsn,&ctx);
    mPtrContainer->setEnabled(true);
    if(iret == 0 )
    {
        str.clear();
        str.sprintf("DUKPT PINBLOCK is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",pinBlock[0],pinBlock[1],pinBlock[2],pinBlock[3],pinBlock[4],pinBlock[5],pinBlock[6],pinBlock[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetPinDukpt is %d\r\n",iret);
        sendMessage(str);
    }

    //get mac
    memcpy(datain,"\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55",16);
    iret = DLL_PciGetMac(0,16,datain,macOut,0);
    if(0==iret)
    {
        str.clear();
        str.sprintf("MAC OUT 0 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMac is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMac(0,16,datain,macOut,1);
    if(0==iret)
    {
        str.clear();
        str.sprintf("MAC OUT 1 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMac is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMac(0,16,datain,macOut,2);
    if(0==iret)
    {
        str.clear();
        str.sprintf("MAC OUT 2 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMac is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMac(0,16,datain,macOut,3);
    if(0==iret)
    {
        str.clear();
        str.sprintf("MAC OUT 3 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMac is %d\r\n",iret);
        sendMessage(str);
    }

    //get des
    memcpy(datain,"\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55\x55",24);
    iret = DLL_PciGetDes(0,16,datain,desOut,1);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DES OUT is: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",desOut[0],desOut[1],desOut[2],desOut[3],desOut[4],desOut[5],desOut[6],desOut[7],desOut[8],desOut[9],desOut[10],desOut[11],desOut[12],desOut[13],desOut[14],desOut[15]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetDes is %d\r\n",iret);
        sendMessage(str);
    }

    //dukpt mac
    memcpy(datain,"4012345678909D987",17);
    iret = DLL_PciGetMacDukpt(1,17,datain,macOut,0,outKsn);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DUKPT MAC OUT 0 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMacDukpt is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMacDukpt(1,17,datain,macOut,1,outKsn);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DUKPT MAC OUT 1 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMacDukpt is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMacDukpt(1,17,datain,macOut,2,outKsn);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DUKPT MAC OUT 2 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMacDukpt is %d\r\n",iret);
        sendMessage(str);
    }

    iret = DLL_PciGetMacDukpt(1,17,datain,macOut,3,outKsn);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DUKPT MAC OUT 3 is: %02x%02x%02x%02x%02x%02x%02x%02x\r\n",macOut[0],macOut[1],macOut[2],macOut[3],macOut[4],macOut[5],macOut[6],macOut[7]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMacDukpt is %d\r\n",iret);
        sendMessage(str);
    }

    //dukpt des
    iret = DLL_PciGetDesDukpt(1,17,datain,desOut,1,outKsn);
    if(0==iret)
    {
        str.clear();
        str.sprintf("DUKPT DES OUT is: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",desOut[0],desOut[1],desOut[2],desOut[3],desOut[4],desOut[5],desOut[6]
                ,desOut[7],desOut[8],desOut[9],desOut[10],desOut[11],desOut[12],desOut[13],desOut[14],desOut[15],desOut[16],desOut[17],desOut[18],desOut[19],desOut[20],desOut[21],desOut[22],desOut[23]);
        sendMessage(str);
    }
    else
    {
        str.clear();
        str.sprintf("DLL_PciGetMacDukpt is %d\r\n",iret);
        sendMessage(str);
    }
    sendMessage("PCI Test Success!!!\r\n");
}
#define KEY1             0x31
#define KEY2             0x32
#define KEY3             0x33
#define KEY4             0x34
#define KEY5             0x35
#define KEY6             0x36
#define KEY7             0x37
#define KEY8             0x38
#define KEY9             0x39
#define KEY0             0x30
#define KEYWELL        0x23
#define KEYCLEAR         0x08
#define KEYALPHA         0x07
#define KEYENTER         0x0d
#define KEYCANCEL        0x1b
#define KEYBACKSPACE     0x1c
#define KEYF1				 0x1e
#define KEYF2				 0x1f
#define KEYF3				 0x20
#define KEYF4				 0x21

/*
void Dialog::on_btnKeyPad_clicked()
{
    unsigned char keyValue [1];
    keyValue[0] = 0xff;

    DLL_KbInputStart();
    while(true)
    {
        DLL_KbGetKey(keyValue);

        if(0xff != keyValue[0])
        {
            printf("keyValue: %02x\r\n",keyValue[0]);
            switch(keyValue[0])
            {
            case KEY1:
                sendMessage("1");
                break;
            case KEY2:
                sendMessage("2");
                break;
            case KEY3:
                sendMessage("3");
                break;
            case KEY4:
                sendMessage("4");
                break;
            case KEY5:
                sendMessage("5");
                break;
            case KEY6:
                sendMessage("6");
                break;
            case KEY7:
                sendMessage("7");
                break;
            case KEY8:
                sendMessage("8");
                break;
            case KEY9:
                sendMessage("9");
                break;
            case KEY0:
                sendMessage("0");
                break;
            case KEYENTER:
                sendMessage("ENTER");
                break;
            case KEYCANCEL:
                sendMessage("CANCEL");
                DLL_KbInputStop();
                return;
                break;
            case KEYCLEAR:
                sendMessage("CLEAR");
                break;
            case KEYF1:
                sendMessage("F1");
                break;
            case KEYF2:
                sendMessage("F2");
                break;
            case KEYF3:
                sendMessage("F3");
                break;
            case KEYF4:
                sendMessage("F4");
                break;
            case KEYALPHA:
                sendMessage("ALPHA");
                break;
            case KEYWELL:
                sendMessage("#");
                break;
            default:
                break;
            }
        }
    }
}
*/
void Dialog::on_btnSn_clicked()
{
    QString strSn;
    unsigned char sn[13];
    int iret = -1;
    memset(sn,0,sizeof(sn));
    if(ui->lineEdit->text().isEmpty()) {sendMessage("sn can't be empty!!!"); return;}
    memcpy(sn, ui->lineEdit->text().toLatin1().data(),ui->lineEdit->text().length());
    iret = DLL_SysWriteSN(12,sn);
    printf("DLL_SysWriteSN is %d\r\n",iret);
    memset(sn,0,sizeof(sn));
   iret =  DLL_SysReadSN(sn);
    printf("DLL_SysReadSN is %d\r\n",iret);
    strSn.sprintf("SN is :%s",sn);
    sendMessage(strSn);
}

void Dialog::on_btnExit_clicked()
{

}

void Dialog::on_fail_clicked()
{
    mPtrContainer->close();
    emit return_test_result(1);
}

void Dialog::on_pass_clicked()
{
    mPtrContainer->close();
    emit return_test_result(0);
}
