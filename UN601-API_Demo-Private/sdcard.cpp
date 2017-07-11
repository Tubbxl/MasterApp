#include "sdcard.h"
#include "ui_sdcard.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

#include <QTextStream>
#include <QFile>
#include<QThread>
#include <QDebug>
#include "settingmenu.h"

SDcard::SDcard(const QString *type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SDcard)
{
    ui->setupUi(this);

     TYPE = *type;

     mSaveFlag = 0;
     loadStyleSheet("uistyle1");

     saveDate();
     QThread::msleep(300);
     getDateToCompare();
}

SDcard::~SDcard()
{
    delete ui;
   delete mPtrContainer;
}

void SDcard::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);

    file.close();
}

void SDcard::setContainerPointer(QWidget *ptrContainer)
{
        mPtrContainer = ptrContainer;
}

void SDcard::saveDate()
{
    if(TYPE ==FUNC_TYPE_SD_CARD_TEST)
    {
        strcpy(mFilePath, "/media/media/mmcblk0p1/tese.dat");
        strcpy(mDirPath, "/media/media/mmcblk0p1/");
    }
    else if(TYPE ==FUNC_TYPE_USB_OTG_TEST||TYPE ==FUNC_TYPE_USB_HOST_TEST)
    {
        if(TYPE ==FUNC_TYPE_USB_OTG_TEST)
             ui->label->setText("USB OTG Test");
        else
            ui->label->setText("USB HOST Test");

         if(0 == access( "/media/media/sda/",F_OK))
         {
            strcpy(mFilePath, "/media/media/sda/tese.dat");
            strcpy(mDirPath, "/media/media/sda/");
         }
         else if(0 == access( "/media/media/sda1/",F_OK))
         {
            strcpy(mFilePath, "/media/media/sda1/tese.dat");
            strcpy(mDirPath, "/media/media/sda1/");
         }
         else if(0 == access( "/media/media/sda2/",F_OK))
         {
            strcpy(mFilePath, "/media/media/sda2/tese.dat");
            strcpy(mDirPath, "/media/media/sda2/");
         }
         else if(0 == access( "/media/media/sda3/",F_OK))
         {
            strcpy(mFilePath, "/media/media/sda3/tese.dat");
            strcpy(mDirPath, "/media/media/sda3/");
         }
         else if(0 == access( "/media/media/sda4/",F_OK))
         {
            strcpy(mFilePath, "/media/media/sda4/tese.dat");
            strcpy(mDirPath, "/media/media/sda4/");
         }
        qDebug(mFilePath);
    }
     if(0 != access(mDirPath,F_OK))
    {
         if(TYPE ==FUNC_TYPE_SD_CARD_TEST)
         {
            ui->textEdit->append("No SD Card");
         }
         else if(TYPE ==FUNC_TYPE_USB_OTG_TEST||TYPE ==FUNC_TYPE_USB_HOST_TEST)
         {
                  ui->textEdit->append("No USB Device");
         }
         return ;
    }

    QFile file(mFilePath);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        ui->textEdit->append("Save fail!");
        return;
    }
    ui->textEdit->append("Save date :");

    QTextStream txtOutput(&file);
    QString s1;
    if(TYPE ==FUNC_TYPE_USB_OTG_TEST)
         s1="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    else if(TYPE ==FUNC_TYPE_USB_OTG_TEST)
         s1="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
    else
         s1="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    txtOutput << s1 << endl;
    ui->textEdit->append(s1);
    ui->textEdit->append("Save Successful!\r\n");
    mSaveFlag= 1;
    file.close();
}

void SDcard::getDateToCompare()
{
    if(mSaveFlag == 0)
        return;
    QFile file(mFilePath);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ui->textEdit->append("Read fail!");
        return;
    }
    QTextStream txtIntput(&file);
    QString s1;

    while(!txtIntput.atEnd())
    {
        s1 = txtIntput.readLine();
    }
    ui->textEdit->append("Read Data:\r\n");
    ui->textEdit->append(s1);
    mSaveFlag= 0;
    file.close();

}


void SDcard::on_pass_clicked()
{
    mPtrContainer->close();
    emit return_test_result(0);
}

void SDcard::on_fail_clicked()
{
    mPtrContainer->close();
    emit return_test_result(1);
}
