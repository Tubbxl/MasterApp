#include "serial.h"
#include "ui_serial.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QFile>
#include <QDebug>
//open the tty0
serial::serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial)
{
    ui->setupUi(this);
    mtimer = new QTimer(this);
    ui->pass->close();
    ui->back->close();
    ui->start->setText("start");
    flag = 0;
    comInit();
    connect(mtimer,SIGNAL(timeout()),this,SLOT(senddata()));

    mCom1.changeTxState(false);
    mCom1.TxData.clear();
    connect(&this->mCom1, SIGNAL(comRecive()), this, SLOT(displayRxDatacom1()));
    mCom2.changeTxState(false);
    mCom2.TxData.clear();
    connect(&this->mCom2, SIGNAL(comRecive()), this, SLOT(displayRxDatacom2()));
    mCom3.changeTxState(false);
    mCom3.TxData.clear();
    connect(&this->mCom3, SIGNAL(comRecive()), this, SLOT(displayRxDatacom3()));


    //start recive thread
    mCom1.startCom();
    mCom1.setPortnum("ttymxc0");
    mCom1.changeComState(false);
    mCom1.start();

    mCom2.startCom();
    mCom2.setPortnum("ttymxc1");
    mCom2.changeComState(false);
   // mCom2.start();
    mCom3.startCom();
    mCom3.setPortnum("ttymxc5");
    mCom3.changeComState(false);
   // mCom3.start();
    loadStyleSheet("uistyle1");
}

serial::~serial()
{
    delete ui;
}

void serial::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);

    file.close();
}

void serial::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void serial::comInit()
{

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
           //ui->comboBox->addItem(info.portName());
            serial.close();
        }
    }
}

void serial::on_start_clicked()
{
    if(flag == 0)
    {
        ui->start->setText("stop");
        mtimer->start(2000);
        flag = 1;
    }
    else
    {
        mtimer->stop();
        ui->start->close();
        ui->back->show();
        ui->pass->show();
        mCom3.stop();
        mCom2.stop();
        mCom1.stop();
    }
}

void serial::on_back_clicked()
{
    mCom1.stop();
    mCom1.quit();
    mCom1.exit(0);
    mCom1.wait();
    mCom2.stop();
    mCom2.quit();
    mCom2.exit(0);
    mCom2.wait();
    mCom3.stop();
    mCom3.quit();
    mCom3.exit(0);
    mCom3.wait();
    mPtrContainer->close();
    emit return_test_result(1);
}

void serial::on_pass_clicked()
{
    mPtrContainer->close();
    emit return_test_result(0);
}

void serial::displayRxDatacom1()
{
    QString str;
    char tmp[100];
    char *buf;
    char var;
    if(mCom1.requestData.size()>0)
   {
        buf=mCom1.requestData.data();
        qDebug() << "receive num:" <<mCom1.requestData.size();
        for(var=0;var<mCom1.requestData.size();var++)
        {
            ::snprintf(tmp,100, "%02X", (unsigned char)(*buf));
            buf++;
            str+=QString::fromUtf8(tmp);
            str+=" ";
        }
        ui->com1->append("receive:"+str);
      //  ui->com1->append(str);
    }
    mCom1.requestData.clear();
}

void serial::displayRxDatacom2()
{
    QString str;
    char tmp[100];
    char *buf;
    char var;
    if(mCom2.requestData.size()>0)
   {
        buf=mCom2.requestData.data();
        qDebug() << "receive num:" <<mCom2.requestData.size();
        for(var=0;var<mCom2.requestData.size();var++)
        {
            ::snprintf(tmp,100, "%02X", (unsigned char)(*buf));
            buf++;
            str+=QString::fromUtf8(tmp);
            str+=" ";
        }
        ui->com2->append("receive:"+str);
       // ui->com2->append(str);
    }
    mCom2.requestData.clear();
}

void serial::displayRxDatacom3()
{
    QString str;
    char tmp[100];
    char *buf;
    char var;
    if(mCom3.requestData.size()>0)
   {
        buf=mCom3.requestData.data();
        qDebug() << "receive num:" <<mCom3.requestData.size();
        for(var=0;var<mCom3.requestData.size();var++)
        {
            ::snprintf(tmp,100, "%02X", (unsigned char)(*buf));
            buf++;
            str+=QString::fromUtf8(tmp);
            str+=" ";
        }
        ui->com3->append("receive:"+str);
       // ui->com3->append(str);
    }
    mCom3.requestData.clear();
}

void serial::senddata()
{
    mCom1.TxData.clear();
    mCom1.TxData.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    mCom1.changeTxState(true);
    mCom2.TxData.clear();
    mCom2.TxData.append("12345678909876543210");
    mCom2.changeTxState(true);
    mCom3.TxData.clear();
    mCom3.TxData.append("abcdefghijklmnopqrstuvwxyz");
    mCom3.changeTxState(true);
    ui->com1->append("send :ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   //  ui->com2->append("[com2] send :12345678909876543210");
  //   ui->com3->append("[com3] send :abcdefghijklmnopqrstuvwxyz");
}
