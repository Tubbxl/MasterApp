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
    //ui->pass->close();

    flag = 0;
  //  comInit();
    connect(mtimer,SIGNAL(timeout()),this,SLOT(senddata()));

    mCom1.changeTxState(false);
    mCom1.TxData.clear();
    connect(&this->mCom1, SIGNAL(comRecive()), this, SLOT(displayRxDatacom1()));


    //start recive thread
    mCom1.startCom();
  //  mCom1.setPortnum("ttymxc0");
    mCom1.changeComState(false);
    mCom1.start();

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

}



void serial::on_pass_clicked()
{
    mCom1.setstop();
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

void serial::senddata()
{
    mCom1.TxData.clear();
    mCom1.TxData.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    mCom1.changeTxState(true);
     ui->com1->append("send :ABCDEFGHIJKLMNOPQRSTUVWXYZ");

}

void serial::on_pushButton_clicked()
{
    QString str;
    str = ui->lineEdit->text();
    if(str.isEmpty())
    {
        ui->com1->append("please input send data!");
    }
    else
    {
        mCom1.TxData.clear();
        mCom1.TxData.append(str);
        mCom1.changeTxState(true);
         ui->com1->append("send :"+str);
    }
}
