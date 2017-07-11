#include "gprssetting.h"
#include "ui_gprssetting.h"

Gprssetting::Gprssetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GprsSet)
{
    ui->setupUi(this);
    ui->textBrowser->close();
    DLL_EthClose();
    DLL_Pppdisconnect();
    DLL_WifiClose();
    flag = 0;
}

Gprssetting::~Gprssetting()
{
    delete ui;
}

void Gprssetting::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void Gprssetting::setContainerPointer(QWidget *ptrContainer)
{
        mPtrContainer = ptrContainer;
}

void Gprssetting::on_connect_clicked()
{
    int ret;
    int csq = 0;
    if(flag == 1)
    {
        ui->textBrowser->append("ping www.baidu.com test.please waiting...");
        gprsping = new ping(2);
        connect(gprsping,SIGNAL(gprspingresult(int)),this,SLOT(gprs_ping_result(int)));
        gprsping->start();
        return;
    }
    ui->textBrowser->clear();

    ret = DLL_PppCheckSim();
    if(ret == 0)
        ui->textBrowser->append("The sim card insert");
    else
    {
         ret = DLL_PppCheckSim();
         if(ret == 0)
             ui->textBrowser->append("The sim card insert");
         else
         {
             ui->textBrowser->append("The sim card  not insert %d");
             qDebug("DLL_PppCheckSim[%d]",ret);
             ui->textBrowser->show();
              ui->frame->close();
             return;
         }
    }
    csq= DLL_PppCheckCsq();
     if(csq > 0)
     {
         QString str;
         str.sprintf("The CSQ is: %d",csq);
       ui->textBrowser->append(str);
     }

    QString   apn = ui->APN->text();
    ui->textBrowser->append("The APN is:"+apn);
    QString   username = ui->username->text();
    ui->textBrowser->append("The user name is:"+username);
    QString    password = ui->PASSWD->text();
    ui->textBrowser->append("The password is:"+password);
    QString   number = ui->lineEdit->text();
    ui->textBrowser->append("The telephone is:"+number);
    ui->frame->close();
    ui->textBrowser->show();
    ui->textBrowser->append("Connect the network.please waiting...");

    QByteArray apn_char = apn.toLatin1();//str.toLatin1();
    char *input_anp = apn_char.data();
    QByteArray name_char = username.toLatin1();//str.toLatin1();
    char *input_name = name_char.data();
    QByteArray pswd_char = password.toLatin1();//str.toLatin1();
    char *input_pswd = pswd_char.data();
    QByteArray tel_char = number.toLatin1();//str.toLatin1();
    char *input_tel = tel_char.data();
    if(apn.isEmpty())
        input_anp = NULL;
    if(username.isEmpty())
        input_name = NULL;
    if(password.isEmpty())
        input_pswd = NULL;
    if(number.isEmpty())
        input_tel = NULL;
     th = new gprsconnect((unsigned char *)input_anp,(unsigned char *)input_name,(unsigned char *)input_pswd,(unsigned char *)input_tel);
    connect(th,SIGNAL(gprscntret(int)),this,SLOT(deal(int )));
    th->start();
    ui->connect->setEnabled(false);
   // ui->textBrowser->append("00000000");
}

void Gprssetting::on_pushButton_2_clicked()
{
    flag = 0;
    ui->textBrowser->clear();
    ui->textBrowser->close();
    ui->connect->setText("connect");
    ui->frame->show();
    emit back();
}

void Gprssetting::deal(int ret)
{
   th->quit();
   th->wait();
   ui->connect->setEnabled(true);
   if(ret == 0)
   {
       ui->textBrowser->append("Connect the network successful...");
       ui->connect->setText("OK");
       unsigned char IP[30]={0};
       unsigned char Mask[30]={0};
       unsigned char gate[30]={0};
       DLL_PppGetNetPara(IP,Mask,gate);
       ui->textBrowser->append("The IP is:"+QString(QLatin1String((char*)IP)));
       ui->textBrowser->append("The P-t-P is:"+QString(QLatin1String((char*)Mask)));
       ui->textBrowser->append("The Gateway is:"+QString(QLatin1String((char*)gate)));
       flag =1;
   }
   else
   {
       //ui->textBrowser->append("Connect the network failed...");
       ui->textBrowser->append("Connect the network failed.please retry...");
   }
}

void Gprssetting::gprs_ping_result(int ret)
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
