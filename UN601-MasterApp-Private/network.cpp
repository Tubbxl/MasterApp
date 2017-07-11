#include "network.h"
#include "ui_network.h"
#include <QDebug>
#include <QMessageBox>
extern "C" {
#include "dll_api.h"
#include "include/SSLComm.h"
}

#include "mappdefines.h"

#include <QTimer>
network::network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::network)
{
    ui->setupUi(this);
    //************************
    wififram = NULL;
    gsm = NULL;
    ethcnt = NULL;

    scene1 = new QGraphicsScene;
    view1 = new QGraphicsView(scene1);
    scene2  = new QGraphicsScene;
    view2 = new QGraphicsView(scene2);

//    scene = new QGraphicsScene;
//    w = scene->addWidget(this);
//    w->setRotation(270);
//    view = new QGraphicsView(scene);
//    view->resize(320, 240);
//    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->testButton->setEnabled(false);
//    isHaveActive = false;


    mpPromptMsgBox = new QMessageBox(this);
    mpPromptMsgBox->setWindowTitle(tr("Prompt"));
    mpPromptMsgBox->setIcon(QMessageBox::Information);
    mpPromptMsgBox->setStandardButtons(0);
    mpPromptMsgBox->setModal(false);


    //***************************
    //view->show();

    mbNetSetThreadExit = true;
   mbOPTestThreadExit = true;

   QTimer::singleShot( UPDATE_APP_FM_TIME_OUT, this, SLOT(uiTimeout()) );
   mpQTimer = new QTimer(this);
   connect(mpQTimer, SIGNAL(timeout()), this, SLOT(testFunc()));
   mpQTimer->setInterval(WAIT_TIME_OUT);
   mpQTimer->setSingleShot(false);

}

void network::setContainerPointer(QWidget* ptrContainer)
{
   mPtrContainer = ptrContainer;
}

void network::waitTimeout()
{
    if(mbNetSetThreadExit &&
           mbOPTestThreadExit )
    {
        mpQTimer->stop();
        delete mpQTimer;

         on_btnBack_clicked();
    }
}

void network::uiTimeout()
{
    if(mbNetSetThreadExit &&
           mbOPTestThreadExit )
    {
         on_btnBack_clicked();
    }
    else
    {
       mpQTimer->start();
    }
}

network::~network()
{
    delete ui;
    delete wififram;
    delete gsm;
    delete ethcnt;
    delete mpPromptMsgBox;

//    delete scene;
//    delete w ;
//    delete view ;

    delete scene1;
    delete w1 ;
    delete view1 ;

    delete scene2;
    delete w2 ;
    delete view2 ;
}

void network::on_comboBox_activated(const QString &arg1)
{

   if(arg1.contains("3/4G",Qt::CaseInsensitive))
   {
       mbNetSetThreadExit = false;

        qDebug()<<arg1<<endl;
        flag = 34;
        DLL_Pppdisconnect();
        DLL_EthClose();
        DLL_WifiClose();
        gsm =  new gprs;
        w2 = scene2->addWidget(gsm);
        w2->setRotation(270);
        view2->resize(320, 240);

        //***************************
        connect(gsm,&gprs::return_network,
                [=]{
                    view2->close();
                    //view->show();
                }
        );
        connect(gsm,SIGNAL(successful(int)),this,SLOT(ConnectResult(int)));
 /*       connect(gsm,&gprs::successful,
                [=]{
                    view2->close();
                    view->show();

                }
        );*/
       // view->close();
        view2->show();

   }
   else if(arg1.contains("Ethernet",Qt::CaseInsensitive))
   {
            mbNetSetThreadExit = false;

       //DLL_WifiDisConnectNetwork();
       DLL_WifiClose();

       flag = 101;
       ethcnt =  new ethconnect;
       mpPromptMsgBox->setText("Network setting, please waiting...");
       mpPromptMsgBox->move(110, 200);
       mpPromptMsgBox->setFixedSize(200, 80);
       mpPromptMsgBox->show();
       connect(ethcnt,SIGNAL(ethconnetret(int)),this,SLOT(ConnectResult(int)));
        ethcnt->start();
   }
   else if(arg1.contains("WIFI",Qt::CaseInsensitive))
   {
       mbNetSetThreadExit = false;

       flag = 99;
       qDebug()<<arg1<<endl;
       DLL_Pppdisconnect();
       DLL_EthClose();
       //DLL_WifiDisConnectNetwork();
       DLL_WifiClose();
       wififram = new wifi;


       w1 = scene1->addWidget(wififram);
       w1->setRotation(270);

       view1->resize(320, 240);
       //view->close();
       view1->show();
       //***************************
       connect(wififram,SIGNAL(connectRet(int)),this,SLOT(ConnectResult(int)));
       connect(wififram,&wifi::clos_wifi_win,
               [=]{
                   view1->close();
                   //view->show();
               }
       );


   }
}

void network::updatePromptMsg(QString strPrompt)
{
      qDebug("updatePromptMsg strPrompt :[%s]", qPrintable(strPrompt));

     if(NULL != mpPromptMsgBox)
     {
         mpPromptMsgBox->setText(strPrompt);
         mpPromptMsgBox->move(110, 200);
         mpPromptMsgBox->setFixedSize(200, 80);
          mpPromptMsgBox->show();
     }
}

void network::handleRefreshPrompt(QString strPrompt)
{
    updatePromptMsg(strPrompt);

}

void network::closePromptMsg()
{
    mbOPTestThreadExit = true;
     if(NULL != mpPromptMsgBox)
     {
          mpPromptMsgBox->hide();
          mpPromptMsgBox->close();
     }
}

void network::updatePromptMsgAndExit(QString strPrompt)
{
    updatePromptMsg(strPrompt);
    QTimer::singleShot( 2000,this, SLOT(closePromptMsg()) );
}

void network::on_testButton_clicked()//connect ip port
{

    qDebug()<<"connect......"<<endl;
    QString strPort = ui->port->text();
    QString strIP = ui->IP->text();

    if(strPort.length() <= 0||
       strIP.length() <= 0
            )
    {
       updatePromptMsgAndExit("IP or Port can't be empty");
        return;
    }


    char*  ptrTemp = NULL ;
    QByteArray arrIP = strIP.toLatin1();
    ptrTemp=arrIP.data();



    int iPort = strPort.toInt();

    char szIP[16] = {0};

   strcpy(szIP, ptrTemp);

    qDebug("szIP: [%s]", szIP );
    qDebug("iPort: [%d]", iPort );

    mbOPTestThreadExit = false;

    mpOPTestThread = new OPTestThread(szIP, iPort);
    connect(mpOPTestThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpOPTestThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpOPTestThread->start();

}

void network::ConnectResult(int ret)
{
     mbNetSetThreadExit = true;

    if(flag==34)
    {
        view2->close();
        //view->show();
    }
    else if(flag == 99)
    {
        view1->close();
      //  view->show();
    }

    if(ret==0)
    {
        ui->testButton->setEnabled(true);
        updatePromptMsgAndExit("Set up the network successfully!");

    }
    else
    {
        updatePromptMsgAndExit("Set up the network failed!");
    }

}


void network::on_btnBack_clicked()
{
   mPtrContainer->close();
}
