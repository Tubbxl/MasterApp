#include "passwdinput.h"
#include "ui_passwdinput.h"
#include <QThread>

#include <QTimer>

//#include "mappdefines.h"

passwdinput::passwdinput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::passwdinput)
{
    ui->setupUi(this);
    pswd = new QString;

     QTimer::singleShot( 60*1000, this, SLOT(uiTimeout()) );
}

void passwdinput::uiTimeout()
{
    on_pushButton_clicked();
}

passwdinput::~passwdinput()
{
    delete ui;
    delete pswd;
}

void passwdinput::setContainerPointer(QWidget* ptrContainer)
{
   mPtrContainer = ptrContainer;
}

void passwdinput::on_pushButton_clicked()
{
    mPtrContainer->close();
   // emit return_parent();
}

void passwdinput::on_pushButton_2_clicked()
{
    *pswd = ui->lineEdit->text();
   // ui->label->setText("connecting...please wait..");
   // ui->pushButton_2->setEnabled(false);
    //ui->pushButton->setEnabled(false);
   // QThread::sleep(1);
    emit return_passwd(pswd);
}
