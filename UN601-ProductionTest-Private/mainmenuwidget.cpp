#include "mainmenuwidget.h"
#include "ui_mainmenuwidget.h"

#include "settingmenu.h"
#include "versioninfodlg.h"
#include "lcd.h"
#include "common.h"
#include "dialog.h"
#include "keypad.h"
#include "wifisetting.h"
#include "ethernetsetting.h"
#include "gprssetting.h"
#include "sdcard.h"
#include "serial.h"
#include "scanner.h"

#include "icccard.h"

#include <QFile>
#include <QDebug>

mainmenuwidget::mainmenuwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainmenuwidget)
{
    ui->setupUi(this);
    mFailCount = 0;
    mPassCount = 0;
    connect(ui->pass,&QCheckBox::clicked,[=] {ui->fail->setCheckState(Qt::Unchecked);});
    connect(ui->fail,&QCheckBox::clicked,[=]{ui->pass->setCheckState(Qt::Unchecked); });

    loadStyleSheet("uistyle1");
}

mainmenuwidget::mainmenuwidget(QString  (*pStrList)[18], int listSize, QWidget *parent):
    QWidget(parent),
    ui(new Ui::mainmenuwidget)
{
    ui->setupUi(this);
    mIndex = 0;
    mpStrMenuUI = pStrList;
    miMenuGroup = listSize;
    mFailCount = 0;
    mPassCount = 0;
    mTsetCount =0;
    mStart = false;
    for(int i = 0;i<40;i++)
        mThreadCreate[i] = false;
    connect(ui->pass,&QCheckBox::clicked,[=] {ui->fail->setCheckState(Qt::Unchecked);});
    connect(ui->fail,&QCheckBox::clicked,[=]{ui->pass->setCheckState(Qt::Unchecked); });
    handleFuncTestType(mpStrMenuUI[0][mIndex+2]);
    loadStyleSheet("uistyle1");
}

mainmenuwidget::~mainmenuwidget()
{
    delete ui;
}

void mainmenuwidget::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

   setStyleSheet(styleSheet);

    file.close();
}

void mainmenuwidget::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void mainmenuwidget::displayResult(QString ret)
{
    qDebug()<<ret;
    ui->result->append(ret);
}

void mainmenuwidget::handleFuncTestType(QString strTestType)
{
    ui->item->setText(strTestType);
    qDebug("handleFuncType:[%s]", qPrintable(strTestType));

    if(0 == QString::compare(strTestType, FUNC_TYPE_IC_CARD_TEST)) //Ic
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                icccard *IccDlg = new icccard();
                connect(IccDlg,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                IccDlg->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), IccDlg, SLOT(iccdeal()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_BAR_CODE_TEST)) //scan
    {

        mMenuType = FUNC_TYPE_BAR_CODE_TEST;
            /*    if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                icccard *scan = new icccard();
                connect(scan,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                scan->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), scan, SLOT(scandeal()));*/
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_MSR_TEST)) //msr
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                 mMenuType.clear();
                icccard *msr = new icccard();
                connect(msr,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                connect(this,SIGNAL(toStopThread()),msr,SLOT(stopTheThread()),Qt::DirectConnection);
                msr->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), msr, SLOT(mcrdeal()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_NFC_TEST)) //picc
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                icccard *picc = new icccard();
                connect(picc,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                connect(this,SIGNAL(toStopThread()),picc,SLOT(stopTheThread()),Qt::DirectConnection);
                picc->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), picc, SLOT(piccdeal()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_LED_TEST)) //led
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                common *led = new common(&FUNC_TYPE_LED_TEST);
                connect(led,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                led->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), led, SLOT(ledtest()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_BEEP_TEST)) //beep
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                common *beep = new common(&FUNC_TYPE_BEEP_TEST);
                connect(beep,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                beep->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), beep, SLOT(ledtest()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_BACKLIGHT_TEST)) //lcd backlight
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                common *lcd = new common(&FUNC_TYPE_BACKLIGHT_TEST);
                connect(lcd,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                lcd->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), lcd, SLOT(backlight()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_DDR_TEST)) //ddr info
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                common *ddr = new common(&FUNC_TYPE_DDR_TEST);
                connect(ddr,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                ddr->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), ddr, SLOT(getDDRSize()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_DATE_TEST)) //date time
    {
                if(mThreadCreate[mIndex] == true)
                    return;
                mThreadCreate[mIndex] = true;

                mMenuType.clear();
                common *time = new common(&FUNC_TYPE_DDR_TEST);
                connect(time,SIGNAL(sendMessage(QString)),this,SLOT(receiveMsg(QString)));
                time->moveToThread(&mItemThreas[mIndex]);
                QObject::connect(&mItemThreas[mIndex], SIGNAL(started()), time, SLOT(refreshDisplayDateTime()));
    }
    else if(0 == QString::compare(strTestType, FUNC_TYPE_LCD_TEST)) //lcd rgb
    {
        mMenuType = FUNC_TYPE_LCD_TEST;
    }
    else
    {
        mMenuType.clear();
    }
}

void mainmenuwidget::LCDRgb()
{
    lcd *LcdDlg = new lcd(0);

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *proxy = scene->addWidget(LcdDlg);
    proxy->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    view->resize(320, 240);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    LcdDlg->setContainerPointer(view);
    connect(LcdDlg,SIGNAL(return_test_result(QString)),this,SLOT(receiveMsg(QString)) );
    view->show();
}

void mainmenuwidget::scannerTest()
{
    Scanner *scan = new Scanner;

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *proxy = scene->addWidget(scan);
    proxy->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    view->resize(320, 240);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scan->setContainerPointer(view);
    connect(scan,SIGNAL(return_test_result(QString)),this,SLOT(receiveMsg(QString)) );
    view->show();
}

void mainmenuwidget::on_next_clicked()
{
    ui->result->clear();
    if(ui->pass->isChecked())
    {
        if(mStart)
            mPassCount ++;
        qDebug("mPassCount %d",mPassCount);
    }
      if(ui->fail->isChecked())
      {
          if(mStart)
              mFailCount ++;
          qDebug("mPassCount %d",mFailCount);
      }
      emit toStopThread();
     if((mIndex>0))// if((mIndex>0)&& (true==mMenuType.isEmpty()))
      {
         mItemThreas[mIndex].exit();
          mItemThreas[mIndex].quit();
          mItemThreas[mIndex].wait();
      }
    mIndex ++;
    if(mpStrMenuUI[0][mIndex+2]=="")
    {
        displayResult("All Test Finished");
        QString str;
        str.sprintf("The all count is : %d",mTsetCount);
        displayResult(str);
        str.sprintf("Pass count is : %d",mPassCount);
        displayResult(str);
        str.clear();
        str.sprintf("Fail count is : %d",mFailCount);
        displayResult(str);
        mFailCount = 0;
        mPassCount = 0;
        mTsetCount =0;
        mIndex = 0;
    }
    mStart = false;
    ui->pass->setCheckState(Qt::Unchecked);
    ui->fail->setCheckState(Qt::Unchecked);


    handleFuncTestType(mpStrMenuUI[0][mIndex+2]);
}

void mainmenuwidget::on_start_2_clicked()
{
    mPtrContainer->close();
}

void mainmenuwidget::on_count_clicked()
{
    qDebug("on_count_clicked");
    QString str;
    str.sprintf("The all count is : %d",mTsetCount);
    displayResult(str);
    str.sprintf("Pass count is : %d",mPassCount);
    displayResult(str);
    str.clear();
    str.sprintf("Fail count is : %d",mFailCount);
    displayResult(str);
}

void mainmenuwidget::on_start_clicked()
{
    mStart = true;
    mTsetCount ++;
    if( (true==mMenuType.isEmpty()))
        mItemThreas[mIndex].start();
    else if(mMenuType == FUNC_TYPE_LCD_TEST)
    {
        LCDRgb();
    }
    else if(mMenuType == FUNC_TYPE_BAR_CODE_TEST)
    {
        scannerTest();
    }
}

void mainmenuwidget::on_pass_clicked()
{
  /*  if(ui->pass->isChecked())
    {
        if(mStart)
            mPassCount ++;
        qDebug("mPassCount %d",mPassCount);
    }*/
}

void mainmenuwidget::on_fail_clicked()
{
  /*  if(ui->fail->isChecked())
    {
        if(mStart)
            mFailCount ++;
        qDebug("mPassCount %d",mFailCount);
    }*/
}

void mainmenuwidget::receiveMsg(QString Msg)
{
    displayResult(Msg);
}
