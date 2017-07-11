#include "gprs.h"
#include "ui_gprs.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>

#include <QTimer>

#include "mappdefines.h"

extern "C" {
#include "dll_api.h"
}

gprs::gprs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gprs)
{
    ui->setupUi(this);

      QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );

}

void gprs::uiTimeout()
{
    on_pushButton_2_clicked();
}

gprs::~gprs()
{
    delete ui;

    delete th;
    delete mpPromptMsgBox;
}

void gprs::on_pushButton_clicked()
{

//    int ret = -1;
//    int csq = 0;
    mpPromptMsgBox = new QMessageBox(this);
    mpPromptMsgBox->setWindowTitle(tr("Prompt"));
   // mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
    mpPromptMsgBox->setIcon(QMessageBox::Information);
    mpPromptMsgBox->setStandardButtons(0);
    mpPromptMsgBox->setModal(false);


    mpPromptMsgBox->move(140, 230);
    mpPromptMsgBox->setFixedSize(200, 80);
    ui->pushButton->setEnabled(false);
    apn = ui->APN->text();
    name = ui->username->text();
    pswd = ui->PASSWD->text();
    tel = ui->number->text();

    mpPromptMsgBox->setText("Network setting, please waiting...");
    mpPromptMsgBox->show();

    QByteArray apn_char = apn.toLatin1();//str.toLatin1();
     char *input_apn = apn_char.data();
    QByteArray name_char = name.toLatin1();//str.toLatin1();
     char *input_name = name_char.data();
    QByteArray pswd_char = pswd.toLatin1();//str.toLatin1();
     char *input_pswd = pswd_char.data();
    QByteArray tel_char = tel.toLatin1();//str.toLatin1();
     char *input_tel = tel_char.data();

    if(pswd_char.isEmpty()) input_pswd = NULL;
    if(name_char.isEmpty()) input_name = NULL;


    qDebug()<<"Username:"<<input_name<<"passwd:"<<input_pswd<<"APN:"<<input_apn<<"tel:"<<input_tel<<endl;

    th = new gprsconnect((unsigned char *)input_apn,(unsigned char *)input_name,(unsigned char *)input_pswd,(unsigned char *)input_tel);
    connect(th,SIGNAL(gprscntret(int)),this,SLOT(deal(int )));
    th->start();


}

void gprs::on_pushButton_2_clicked()
{
    this->close();
    emit return_network();
}

void gprs::deal(int ret)
{
     ui->pushButton->setEnabled(true);
    th->quit();
    th->wait();
    mpPromptMsgBox->close();
   emit successful(ret);
}
