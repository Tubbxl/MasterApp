#include "pinentry.h"
#include "ui_pinentry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtGui>
#include <QStyleFactory>
#include <QThread>
#include "updatemsgthread.h"
#include <QDateTime>


PinEntry::PinEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinEntry)
{
    ui->setupUi(this);
    UpdateMsgThread *pinthread = new UpdateMsgThread;
    connect(pinthread, SIGNAL(refreshPromptSignal(int,QString)), this, SLOT(handleRefreshPrompt(int,QString)));
    connect(pinthread, SIGNAL(itemCtrlSignal(int,int)), this, SLOT(handleItemCtrl(int,int)));
    connect(pinthread, SIGNAL(showPinEntrySignal(int)), this, SLOT(handlePinEntry(int)));
    pinthread->start();
}

void PinEntry::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);
}

PinEntry::~PinEntry()
{
    delete ui;
}


void PinEntry::handleRefreshPrompt(int itemid,QString msg)
{
    switch(itemid)
    {
    case 0:
        ui->labelPrompt->setText(msg);
        break;
    case 1:
        ui->labelPwd->setText(msg);
        break;
    case 2:
        ui->labelPwdConfirm->setText(msg);
        break;
    }
    QApplication::processEvents();
}


void PinEntry::handleItemCtrl(int mode,int count)
{
    switch(mode)
    {
    case 0:
        if(count == 0)
        {
            ui->labelPwd->setVisible(true);
            ui->labelPwdConfirm->setVisible(false);
            ui->PwdConfirm->setVisible(false);
        }
        if(count == 1)
        {
            ui->labelPwd->setVisible(false);
        }
        if(count == 2)
        {
            ui->labelPwdConfirm->setVisible(true);
            ui->PwdConfirm->setVisible(true);
        }
        if(count == 3)
        {
            ui->labelPwdConfirm->setVisible(false);
            ui->PwdConfirm->setVisible(false);
        }
        if(count == 4)
        {
            /*editConfirm.setFocusable(true);
            editConfirm.setFocusableInTouchMode(true);
            editConfirm.requestFocus();*/
            ui->PwdConfirm->setFocus();
        }
        break;
    case 1:
        if(count == 0) ui->PwdShow->setText("");
        else if(count == 100)//退出PIN显示框
        {
            ui->PwdShow->setText("");
            ui->PwdConfirm->setText("");
        }
        else setTextShow(0,count);//editPin.append("*");
        break;
    case 2:
        //editPin.setText("");
        if(count ==0 ) ui->PwdShow->setText("");
        break;
    case 3:
        if(count ==0 ) ui->PwdConfirm->setText("");
        break;
    case 4:
        setTextShow(0,count);
        break;
    case 5:
        setTextShow(1,count);
        break;
    }
    QApplication::processEvents();
}

void PinEntry::handlePinEntry(int mode)
{
    printf("mode is %d\r\n",mode);
    switch(mode)
    {
    case 0:
        printf("hide the dialog\r\n");
        this->showMinimized();
        break;
    case 1:
        printf("show the dialog\r\n");
        setWindowFlags( Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
        this->showMaximized();
        break;

    }
    QApplication::processEvents();
}



void PinEntry::setTextShow(int nBox, int count)
{
    printf("count is %d\r\n",count);
    switch(count)
            {
            case 0:
                if(nBox == 0) ui->PwdShow->setText("");
                if(nBox == 1) ui->PwdConfirm->setText("");
                break;
            case 1:
                if(nBox == 0) ui->PwdShow->setText("*");
                if(nBox == 1) ui->PwdConfirm->setText("*");
                break;
            case 2:
                if(nBox == 0) ui->PwdShow->setText("**");
                if(nBox == 1) ui->PwdConfirm->setText("**");
                break;
            case 3:
                if(nBox == 0) ui->PwdShow->setText("***");
                if(nBox == 1) ui->PwdConfirm->setText("***");
                break;
            case 4:
                if(nBox == 0) ui->PwdShow->setText("****");
                if(nBox == 1) ui->PwdConfirm->setText("****");
                break;
            case 5:
                if(nBox == 0) ui->PwdShow->setText("*****");
                if(nBox == 1) ui->PwdConfirm->setText("*****");
                break;
            case 6:
                if(nBox == 0) ui->PwdShow->setText("******");
                if(nBox == 1) ui->PwdConfirm->setText("******");
                break;
            case 7:
                if(nBox == 0) ui->PwdShow->setText("*******");
                if(nBox == 1) ui->PwdConfirm->setText("*******");
                break;
            case 8:
                if(nBox == 0) ui->PwdShow->setText("********");
                if(nBox == 1) ui->PwdConfirm->setText("********");
                break;
            case 9:
                if(nBox == 0) ui->PwdShow->setText("*********");
                if(nBox == 1) ui->PwdConfirm->setText("*********");
                break;
            case 10:
                if(nBox == 0) ui->PwdShow->setText("**********");
                if(nBox == 1) ui->PwdConfirm->setText("**********");
                break;
            case 11:
                if(nBox == 0) ui->PwdShow->setText("***********");
                if(nBox == 1) ui->PwdConfirm->setText("***********");
                break;
            case 12:
                if(nBox == 0) ui->PwdShow->setText("************");
                if(nBox == 1) ui->PwdConfirm->setText("************");
                break;
            default :
                break;
            }
}
