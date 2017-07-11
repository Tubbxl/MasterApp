#include "ethernetsetting.h"
#include "ui_ethernetsetting.h"
#include <QThread>

Ethernetsetting::Ethernetsetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EthernetSet)
{
    ui->setupUi(this);
    ui->textBrowser->close();
    unsigned char macaddr[20]={0};
    DLL_Pppdisconnect();
    DLL_WifiClose();
    DLL_EthGetMacAddr(macaddr);
    C_Mac = QString(QLatin1String((char*)macaddr));
    ui->label_7->setText(C_Mac);
    flag = 0;
    loadStyleSheet("uistyle1");
}

Ethernetsetting::~Ethernetsetting()
{
    delete ui;
}

void Ethernetsetting::loadStyleSheet(const QString &sheetName)
{

    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void Ethernetsetting::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void Ethernetsetting::on_OK_clicked()
{

    if(flag == 1)
    {
        ui->textBrowser->append("ping www.baidu.com test.please waiting...");
        ethpingthread = new ping(0);
       connect(ethpingthread,SIGNAL(pingresult(int)),this,SLOT(ping_result(int)));
        ethpingthread->start();
        return;
    }
     ui->textBrowser->clear();
    int ret = -1;
    ip = ui->ip->text();
    bcast = ui->bcast->text();
    mack = ui->mack->text();
    //mac = ui->mac->text();
    DLL_EthInit();

    QByteArray wd = ip.toLatin1();//str.toLatin1();
    char *IP = wd.data();
    if(ip.isEmpty()==true)
    {
         ui->textBrowser->append("Automatically get the IP, please waiting...");
         Ethcont = new ethconnect(this);
         connect(Ethcont,SIGNAL(ethconnetret(int)),this,SLOT(Deal(int )));
         Ethcont->start();
         ui->frame->close();
         ui->textBrowser->show();
         flag = 1;
         ui->OK->setEnabled(false);
         return ;
    }
    else
    {
        ui->textBrowser->append("Network setting, please waiting...");
        ui->textBrowser->append("The ip is:"+ip);
        ret=DLL_EthSetLocalIP((unsigned char *)IP);
        ui->textBrowser->append("The Bcast is:"+bcast);
        QByteArray bc = bcast.toLatin1();//str.toLatin1();
        char *BC = bc.data();
        ret= DLL_EthSetGateway((unsigned char *)BC);

        ui->textBrowser->append("The Mack is:"+mack);
        QByteArray ma = mack.toLatin1();//str.toLatin1();
        char *MA = ma.data();
        ret=DLL_EthSetNetMask((unsigned char *)MA);

     //   ui->textBrowser->append("The Mac is:"+mac);
       // QByteArray m = mac.toLatin1();//str.toLatin1();
      //  char *Ma = m.data();
     //   printf("Ma====%s\n",Ma);
    //    ret=DLL_EthSetMacAddr((unsigned char *)Ma);

   }
    ui->frame->close();
    ui->textBrowser->show();
    flag = 1;
}

void Ethernetsetting::on_back_clicked()
{
    flag = 0;
    ui->textBrowser->clear();
    ui->textBrowser->close();
    ui->frame->show();
    emit Back();
}

void Ethernetsetting::ping_result(int ret)
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

void Ethernetsetting::Deal(int ret)
{
    ui->OK->setEnabled(true);
    if(ret == 0)
    {
        ui->textBrowser->append("Connect network successful!");
        unsigned char IP[30]={0};
        unsigned char Mask[30]={0};
        unsigned char gate[30]={0};
        DLL_EthGetNetPara(IP,Mask,gate);
        ui->textBrowser->append("The IP is:"+QString(QLatin1String((char*)IP)));
        ui->textBrowser->append("The Mask is:"+QString(QLatin1String((char*)Mask)));
        ui->textBrowser->append("The Gateway is:"+QString(QLatin1String((char*)gate)));
    }
    else
    {
        ui->textBrowser->append("Connect network failed!");
    }
}

void Ethernetsetting::on_pushButton_clicked()
{
    ui->textBrowser->append("Automatically get the IP, please waiting...");
    Ethcont = new ethconnect(this);
    connect(Ethcont,SIGNAL(ethconnetret(int)),this,SLOT(Deal(int )));
    Ethcont->start();
    ui->frame->close();
    ui->textBrowser->show();
    flag = 1;
    ui->OK->setEnabled(false);
    return ;
}
