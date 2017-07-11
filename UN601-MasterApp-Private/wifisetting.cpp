#include "wifisetting.h"
#include "ui_wifisetting.h"
#include <QDebug>
#include <QThread>
Wifisetting::Wifisetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WifiSet)
{
    ui->setupUi(this);
    //ui->textBrowser->close();

    ui->frame->close();
    ui->listWidget->close();
    ui->Setting->close();
    unsigned char macaddr[20]={0};

    DLL_EthClose();
    DLL_Pppdisconnect();
    DLL_WifiGetMacAddr(macaddr);
    C_Mac = QString(QLatin1String((char*)macaddr));
    ui->label_7->setText(C_Mac);
    pingtestflag = 0;

    ui->textBrowser->append("Network setting, please waiting...");
    ui->textBrowser->append("Scanning.please waiting...");
    Th = new wifiscan(this);

    connect(Th,SIGNAL(sendwifiret(T_WLANDEV *,int *)),this,SLOT(get_network(T_WLANDEV *,int *)));
    ui->OK->setEnabled(false);
    Th->start();
    loadStyleSheet("uistyle1");
}

Wifisetting::~Wifisetting()
{
    delete ui;
}

void Wifisetting::loadStyleSheet(const QString &sheetName)
{
       QFile file(":/qss/" + sheetName.toLower() + ".qss");
       file.open(QFile::ReadOnly);
       QString styleSheet = QLatin1String(file.readAll());

       qApp->setStyleSheet(styleSheet);

       file.close();
}

void Wifisetting::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void Wifisetting::on_back_clicked()
{
    pingtestflag = 0;
    ui->textBrowser->clear();
    ui->listWidget->close();
    ui->OK->setText("OK");
    emit cancel();
}

void Wifisetting::on_OK_clicked()
{

    if(pingtestflag == 1)
    {
        pingthread = new ping(1);
        ui->textBrowser->append("ping www.baidu.com test.please waiting...");
        connect(pingthread,SIGNAL(wifipingresult(int)),this,SLOT(get_ping_result(int)));
         pingthread->start();
        return;
    }
      if(pingtestflag == 0xaa)
      {
          ui->textBrowser->append("Connect the network.please waiting...");
          wifict->start();
          return ;
      }
/*
   ui->textBrowser->clear();
    //ui->frame_2->show();
    ui->frame->close();
    ui->textBrowser->show();
    ip = ui->ip->text();
    bcast = ui->bcast->text();
    mack = ui->mack->text();
    //mac = ui->mac->text();
    ui->textBrowser->append("The ip is:"+ip);
    QByteArray wd = ip.toLatin1();//str.toLatin1();
    char *IP = wd.data();
    ui->textBrowser->append("The Bcast is:"+bcast);
    QByteArray bc = bcast.toLatin1();//str.toLatin1();
    char *BC = bc.data();
    ui->textBrowser->append("The Mack is:"+mack);
    QByteArray ma = mack.toLatin1();//str.toLatin1();
    char *MA = ma.data();
    ui->textBrowser->append("The Mac is:"+C_Mac);
    ui->textBrowser->append("Network setting, please waiting...");
    DLL_WifiSetIP((unsigned char*)IP,(unsigned char*)MA,(unsigned char*)BC);
     ui->textBrowser->append("Network set seccssful...");*/
}

void Wifisetting::on_listWidget_clicked(const QModelIndex &index)
{

  //  ui->listWidget->close();
    view->show();
     SSID = ui->listWidget->currentItem()->text();
}

void Wifisetting::get_pswd(QString *temp)
{
    int i;
     view->close();
    update();
    WD=*temp;
    qDebug()<<"passwd:";
    qDebug()<<WD;

    QByteArray wd = WD.toLatin1();//str.toLatin1();
    char *mm = wd.data();
    QByteArray ssid = SSID.toLatin1();//str.toLatin1();
    char *id = ssid.data();
    qDebug()<<"SSID:";
    qDebug()<<SSID;
    ui->textBrowser->append("The SSID is:"+SSID);
    ui->textBrowser->append("The password is:"+WD);
    ui->textBrowser->append("Connect the network.please waiting...");
    ui->listWidget->close();
    ui->textBrowser->show();

    for(i=0;i<worknum;i++)
    {
      if( memcmp(id,work[i].ssid,strlen((char*)work[i].ssid))==0)
      {
            break;
      }
    }
    wifict = new wificonnectth(work[i],(unsigned char *)mm);
    connect(wifict,SIGNAL(connectend(int)),this,SLOT(isconnect(int )));
    wifict->start();
}

void Wifisetting::get_network(T_WLANDEV *network, int *num)
{
    QString str;
    int Cnt = 0;
    work = network;
    int i = 0;
    Cnt = *num;
    worknum = *num;
    qDebug()<<"network:"<<Cnt<<endl;
    if(Cnt>0)
    while(Cnt--)
    {
       str = QString(QLatin1String((char*)network[i++].ssid));
       ui->listWidget->addItem(str);
       qDebug()<<"SSID:"<<str<<"  "<<i<<endl;
     }
    Th->quit();
    Th->wait();
    ui->textBrowser->close();
    ui->listWidget->show();

    pswd=new passwdinput;
    scene = new QGraphicsScene;
    w = scene->addWidget(pswd);
    w->setRotation(270);
    view = new QGraphicsView(scene);
    view->resize(320, 240);
    pswd->setContainerPointer(view);
    connect(pswd,SIGNAL(return_passwd(QString *)),this,SLOT(get_pswd(QString *)));
   /* connect(pswd,&passwdinput::return_parent,
            [=]
    {
        ui->listWidget->show();
        view->close();
    });*/
}
void Wifisetting::isconnect(int ret)
{
    qDebug()<<"connect end";
    if(ret == 0)
    {
         ui->textBrowser->append("Connect the network successful...");
         ui->OK->setText("ping");
         ui->OK->setEnabled(true);
         ui->Setting->show();
        pingtestflag = 1;
        unsigned char IP[30]={0};
        unsigned char Mask[30]={0};
        unsigned char gate[30]={0};
        DLL_WifiGetNetPara(IP,Mask,gate);
        ui->textBrowser->append("The IP is:"+QString(QLatin1String((char*)IP)));
        ui->textBrowser->append("The Mask is:"+QString(QLatin1String((char*)Mask)));
        ui->textBrowser->append("The Gateway is:"+QString(QLatin1String((char*)gate)));
        ui->textBrowser->append("Press Setting to set  parameters");
    }
    else
    {
         ui->textBrowser->append("Connect the network failed.please retry...");
        ui->OK->setEnabled(true);
        pingtestflag = 0xaa;
    }
  //  wifict->quit();
  //  wifict->wait();
}

void Wifisetting::get_ping_result(int ret)
{
    if(ret == 0)
    {
         ui->textBrowser->append("ping www.baidu.com test.successful!");
    }
    else
    {
        ui->textBrowser->append("ping www.baidu.com test.failed!");
    }
}

void Wifisetting::on_Setting_clicked()
{
    if(pingtestflag == 2)
    {
        pingtestflag = 1;
        ui->frame->close();
        ui->textBrowser->show();
        ip = ui->ip->text();
        bcast = ui->bcast->text();
        mack = ui->mack->text();
        //mac = ui->mac->text();
        ui->textBrowser->append("The ip is:"+ip);
        QByteArray wd = ip.toLatin1();//str.toLatin1();
        char *IP = wd.data();
        ui->textBrowser->append("The Bcast is:"+bcast);
        QByteArray bc = bcast.toLatin1();//str.toLatin1();
        char *BC = bc.data();
        ui->textBrowser->append("The Mack is:"+mack);
        QByteArray ma = mack.toLatin1();//str.toLatin1();
        char *MA = ma.data();
        ui->textBrowser->append("The Mac is:"+C_Mac);
        ui->textBrowser->append("Network setting, please waiting...");
        DLL_WifiSetIP((unsigned char*)IP,(unsigned char*)MA,(unsigned char*)BC);
        ui->textBrowser->append("Network set seccssful...");
        return;
    }
    ui->textBrowser->close();
    ui->frame->show();
    pingtestflag =2;
    qDebug()<<"on_Setting_clicked:";
    ui->Setting->setText("OK");

}
