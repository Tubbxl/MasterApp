#include "keypad.h"
#include "ui_keypad.h"

#include <QFile>
#include <QKeyEvent>
#include <QDebug>
extern "C"{
#include "dll_api.h"
}

Keypad::Keypad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keypad)
{
    ui->setupUi(this);
     mKeyCnt = 0;
     ui->frame_2->show();
     ui->pass->close();
     ui->fail->close();
    loadStyleSheet("uistyle1");
    ui->frame_2->setFocus(Qt::ActiveWindowFocusReason);
}

Keypad::~Keypad()
{
    delete ui;
}

void Keypad::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);

    file.close();
}

void Keypad::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void Keypad::keyPressEvent(QKeyEvent *event)
{
    qDebug("KEY:%08x mKeyCnt:%02x",event->key(),mKeyCnt);
    switch(event->key())
    {
    case Qt::Key_1:
        ui->bnt1->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00001;
        break;
    case Qt::Key_2:
        ui->bnt2->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00002;
        break;
    case Qt::Key_3:
        ui->bnt3->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00004;
        break;
    case Qt::Key_4:
        ui->bnt4->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
       mKeyCnt|=0x00008;//
        break;
    case Qt::Key_5:
        ui->bnt5->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00010;
        break;
    case Qt::Key_6:
        ui->bnt6->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00020;
        break;
    case Qt::Key_7:
        ui->bnt7->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
       mKeyCnt|=0x00040;
        break;
    case Qt::Key_8:
        ui->bnt8->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00080;
        break;
    case Qt::Key_9:
        ui->bnt9->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00100;
        break;
    case Qt::Key_0:
        ui->bnt0->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00200;
        break;
    case Qt::Key_Return: //KEYENTER
        ui->ENTER->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00400;
        break;
    case Qt::Key_Backspace:
        ui->CANCEL->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x00800;
        break;
    case Qt::Key_Shift :// KEYCLEAR
        ui->CLEAR->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x01000;
        break;
    case Qt::Key_F1:
        ui->F1->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x02000;
        break;
    case Qt::Key_F2:
        ui->F2->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x04000;
        break;
    case Qt::Key_F3:
        ui->F3->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x08000;
        break;
    case Qt::Key_F4:
        ui->F4->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x10000;
        break;
    case KEYALPHA:
        ui->ALPHA->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x20000;
        break;
    case Qt::Key_Alt:  //23
        ui->WELL->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x40000;
        break;
    case KEYBACKSPACE:
        ui->BACKSPACE->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
        mKeyCnt|=0x80000;
        break;
    default:
        break;
    }
    qDebug("mKeyCnt:%02x",mKeyCnt);
    if((mKeyCnt^0xfffff)==0)//if((mKeyCnt^0xfffff)==0)
    {
        ui->frame_2->close();
        ui->pass->show();
        ui->fail->show();
    }
}

void Keypad::on_fail_clicked()
{

     mPtrContainer->close();
     emit return_test_result(0);
}

void Keypad::on_pass_clicked()
{

    mPtrContainer->close();
    emit return_test_result(1);
}

