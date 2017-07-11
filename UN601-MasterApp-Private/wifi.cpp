#include "wifi.h"
#include "ui_wifi.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <wificonnectth.h>
#include "string.h"

#include <QTimer>

#include "mappdefines.h"


wifi::wifi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wifi)
{
    ui->setupUi(this);
    ui->label->setText("Scanning..");
    ui->pushButton->setEnabled(false);
    QString str;
    pswd=new passwdinput;
    Th = new wifiscan(this);
     //************************
    scene = new QGraphicsScene;
    w = scene->addWidget(pswd);
    w->setRotation(270);
    view = new QGraphicsView(scene);
    view->resize(320, 240);

    pswd->setContainerPointer(view);

//    view->show();
    //***************************
    connect(Th,SIGNAL(sendwifiret(T_WLANDEV *,int *)),this,SLOT(get_network(T_WLANDEV *,int *)));
    connect(pswd,SIGNAL(return_passwd(QString *)),this,SLOT(get_pswd(QString *)));
    connect(pswd,&passwdinput::return_parent,
            [=]{
                 emit clos_wifi_win();
            }
    );

    Th->start();

    QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );

}

void wifi::uiTimeout()
{
    on_pushButton_clicked();
}

wifi::~wifi()
{
    delete ui;
    delete pswd;
    delete Th;
    delete work;
    delete wifict;
     delete mpPromptMsgBox;

    delete scene;
   delete w ;
    delete view ;

}

void wifi::on_listWidget_clicked(const QModelIndex &index)
{

    SSID = ui->listWidget->currentItem()->text();

    qDebug()<<"SSID:";
    qDebug()<<SSID;
    //ssidindex = index;
    //qDebug()<<"ssidindex:"<<ssidindex<<endl;
    view->show();

}

void wifi::on_pushButton_clicked()
{
     //Th->set_wifi_RunFlag(false);
     Th->quit();
     Th->wait();
     emit clos_wifi_win();

}

void wifi::get_pswd(QString *temp)
{
    ui->label->setText("connect...");
    //ui->pushButton->setEnabled(false);
    WD=*temp;
//    int ret = -1;
    int i = 0;
    view->close();
    qDebug()<<"passwd:";
    qDebug()<<WD;
    QByteArray wd = WD.toLatin1();//str.toLatin1();
    char *mm = wd.data();
    QByteArray ssid = SSID.toLatin1();//str.toLatin1();
    char *id = ssid.data();
    for(i=0;i<worknum;i++)//while(worknum--)
    {
      if( memcmp(id,work[i].ssid,strlen((char*)work[i].ssid))==0)
            break;
    }

    mpPromptMsgBox = new QMessageBox(this);
    mpPromptMsgBox->setWindowTitle(tr("Prompt"));
   // mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
    mpPromptMsgBox->setIcon(QMessageBox::Information);
    mpPromptMsgBox->setStandardButtons(0);
    mpPromptMsgBox->setModal(false);

    mpPromptMsgBox->setText("Network setting, please waiting...");
    mpPromptMsgBox->move(140, 230);
    mpPromptMsgBox->setFixedSize(200, 80);
    mpPromptMsgBox->show();
    wifict = new wificonnectth(work[i],(unsigned char *)mm);
    connect(wifict,SIGNAL(connectend(int)),this,SLOT(isconnect(int )));
    wifict->start();
}


void wifi::get_network(T_WLANDEV *network, int *num)
{
    QString str;
    int Cnt = 0;
    work = network;
    int i = 0;
    Cnt = *num;
    worknum = *num;
    //Cnt = 4;
    qDebug()<<"network:"<<Cnt<<endl;
    if(Cnt>0)
    while(Cnt--)
    {

       str = QString(QLatin1String((char*)network[i++].ssid));
       ui->listWidget->addItem(str);
       qDebug()<<"SSID:"<<str<<"  "<<i<<endl;
     }
    ui->label->setText("chose to connect..");
    ui->pushButton->setEnabled(true);
    //Th->set_wifi_RunFlag(false);
    Th->quit();
    Th->wait();
}

void wifi::isconnect(int ret)
{
    ui->pushButton->setEnabled(true);
    mpPromptMsgBox->close();
    qDebug()<<"connect end";

    wifict->quit();
    wifict->wait();
    emit connectRet(ret);
}
