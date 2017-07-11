#include "maindialog.h"
#include "ui_maindialog.h"

#include "settingmenudlg.h"

#include "settingmenu.h"
#include "promptupdateosdlg.h"

#include "mappdefines.h"

#include "mfile.h"

extern "C"
{
    #include "include/dll_api.h"
}

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <unistd.h>


 int gb_RunMode = RELEASE_MODE;


MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);


    initData();

    initViews();

}

void MainDialog::hideAllCtrls()
{
    ui->label->hide();
   ui->listApps->hide();
    ui->btnSetting->hide();
}

void MainDialog::showAllCtrls()
{
    ui->label->show();
   ui->listApps->show();
    ui->btnSetting->show();
}

void MainDialog::initViews()
{

    ui->listApps->setFocusPolicy(Qt::NoFocus);
     ui->listApps->resize(220, 200);

     mpPromptMsgBox = new QMessageBox(this);
      mpPromptMsgBox->setWindowTitle(tr("Prompt"));
     mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
      mpPromptMsgBox->setIcon(QMessageBox::Information);
      mpPromptMsgBox->setStandardButtons(0);
      mpPromptMsgBox->setModal(false);

       hideAllCtrls();


      loadStyleSheet("uistyle1");
}

void MainDialog::refreshAppsList()
{
//   int iCount = ui->listApps->count();
//   for(int index=0;index<iCount;index++)
//   {
//       QListWidgetItem *item = ui->listApps->takeItem(0);
//           delete item;
//   }

    ui->listApps->clear();

    for(int i = 0; i < SUB_APP_MAX_NUMS; i++)
    {
        mAppsList[i].clear();
    }

     mpUpdateSubApps->loadAppsList(mAppsList);

    for(int i = 0; i < SUB_APP_MAX_NUMS; i++)
    {
           if(mAppsList[i].length() <= 0)
               break;

           ui->listApps->addItem(mAppsList[i]);
    }
}


void MainDialog::initData()
{
       mMFile = new MFile();

        mpUpdateSubApps = new UpdateSubApps();

        mpUpdateSubApps->initSubAppsFileDir();

        refreshAppsList();

       mProcess = new QProcess();

       mpCheckStatusThread = new CheckStatusThread();

       connect(mProcess, SIGNAL(started()), SLOT(subAppStarted()));
       connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(subAppFinished(int, QProcess::ExitStatus)));
       connect(mProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(subAppErrorHandler (QProcess::ProcessError)));

       connect(mpCheckStatusThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
       connect(mpCheckStatusThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
       connect(mpCheckStatusThread, SIGNAL(update32550OsPromptSignal()), this, SLOT(handleUpdate32550OsPrompt()));
       connect(mpCheckStatusThread, SIGNAL(refreshMainUISignal()), this, SLOT(handleMainUIRefresh()));
       connect( ui->listApps, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickSubAppListItem(QListWidgetItem*)));

       mpCheckStatusThread->start();

}

void MainDialog::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void MainDialog::updatePromptMsg(QString strPrompt)
{
     if(NULL != mpPromptMsgBox)
     {
         mpPromptMsgBox->setText(strPrompt);
         mpPromptMsgBox->move(120, 230);
         mpPromptMsgBox->setFixedSize(200, 80);
          mpPromptMsgBox->show();

           mParent->setDisabled(true);
     }
}

void MainDialog::closePromptMsg()
{
     if(NULL != mpPromptMsgBox)
     {
          mpPromptMsgBox->hide();
          mpPromptMsgBox->close();

          mParent->setEnabled(true);
     }
}

void MainDialog::handleRefreshPrompt(QString strPrompt)
{
     updatePromptMsg(strPrompt);

}

void MainDialog::handleUpdate32550OsPrompt()
{
    PromptUpdateOsDlg *dlg = new PromptUpdateOsDlg();

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *proxy = scene->addWidget(dlg);
    proxy->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    view->resize(320, 240);

    dlg->setContainerPointer(view);

    view->show();


}

void MainDialog::handleMainUIRefresh()
{
    qDebug("handleMainUIRefresh");
    if(gb_RunMode == FACTORY_MODE)
    {
        ui->btnSetting->setText("Setting(Production)");
    }
    else
    {
        ui->btnSetting->setText("Setting");
    }

    showAllCtrls();

    mParent->show();
}




void MainDialog::setParentPointer(QWidget * parent)
{
     mParent = parent;
}

void MainDialog::exitApp()
{
     qDebug("exitApp---function");
    mParent->setEnabled(true);
}

int MainDialog::runApp(QString strAppPath)
{
   if(NULL == strAppPath)
       return -1;

    mProcess->start(strAppPath);

    return 0;
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_btnSetting_clicked()
{
     int iCountLine = -1;
     SettingMenuDlg *fMenuDlg = NULL;

     mParent->hide();


    if(gb_RunMode == FACTORY_MODE)
    {
        iCountLine  =   sizeof(gb_Factory_Pci_UI)/sizeof(gb_Factory_Pci_UI[0]) ;
        fMenuDlg = new SettingMenuDlg((QString  (*)[18])gb_Factory_Pci_UI, iCountLine);
    }
    else
    {
        iCountLine  =   sizeof(gb_Release_Pci_UI)/sizeof(gb_Release_Pci_UI[0]) ;
        fMenuDlg = new SettingMenuDlg((QString  (*)[18])gb_Release_Pci_UI, iCountLine);
    }

    connect(fMenuDlg, SIGNAL(refreshAppsListSignal()), this, SLOT(handleRefreshAppsList()));
    connect(fMenuDlg, SIGNAL(refreshMainUISignal()), this, SLOT(handleMainUIRefresh()));


   QGraphicsScene *scene = new QGraphicsScene;
   QGraphicsProxyWidget *proxy = scene->addWidget(fMenuDlg);
   proxy->setRotation(270);
   QGraphicsView *view = new QGraphicsView(scene);
   view->resize(320, 240);
   view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


  fMenuDlg->setContainerPointer(view);

   view->show();
}

void MainDialog::handleRefreshAppsList()
{
    qDebug("handleRefreshAppsList");
    refreshAppsList();
}

void MainDialog::subAppStarted()
{
    qDebug("subAppStarted");
       mParent->setDisabled(true);
}

void MainDialog::subAppFinished(int, QProcess::ExitStatus)
{
      qDebug("subAppFinished");

    QTimer::singleShot( 500,this, SLOT(exitApp()) );
}

void MainDialog::subAppErrorHandler(QProcess::ProcessError)
{
     qDebug("subAppErrorHandler");
}

void MainDialog::clickSubAppListItem(QListWidgetItem* item)
{
    QString strAppName= item->text();

    QByteArray arrAppName = strAppName.toLatin1();
     char *appName = arrAppName.data();

     qDebug("clickSubAppListItem appName:[%s]", appName);

     char szAppName[SUB_APP_NAME_LEN+1] = {0};

     strcpy(szAppName, appName);

     qDebug("clickSubAppListItem szAppName:[%s]", szAppName);

     int appNo = -1;
     appNo = mpUpdateSubApps->getAppNo(szAppName);

     qDebug("clickSubAppListItem appNo:[%d]", appNo);

    //sign verify
     char filePath[256] = {0};
   sprintf(filePath, "/home/subapps/subapp%d/", appNo);
   strcat(filePath, szAppName);

   qDebug("clickSubAppListItem filePath:[%s]", filePath);

   //sign verify
   unsigned char signBuf[SIGN_DATA_BYTE_LEN+1] = {0};
   int iRet  =  -1;
    iRet = mMFile->getFileSignData(filePath, signBuf);
     qDebug("clickSubAppListItem getFileSignData iRet:[%d]", iRet);
    if(0 != iRet)
    {
         handleRefreshPrompt("Get app sign data failed!");
         sleep(2);
         closePromptMsg();
        return ;
    }

    iRet = DLL_PciVerifySignature(0x01, signBuf);
    qDebug("clickSubAppListItem DLL_PciVerifySignature iRet:[%d]", iRet);
    if(0 != iRet)
    {
         handleRefreshPrompt("app sign verify failed!");
         sleep(2);
          closePromptMsg();
        return ;
    }

    char szCmd[256] = {0};
    sprintf(szCmd, "su subapp%d -l -c %s", appNo, filePath);

    QString strCmd = QString(QLatin1String(szCmd));

     qDebug("clickSubAppListItem strCmd:[%s]", qPrintable(strCmd));

    runApp(strCmd);

    qDebug("item text:[%d]", iRet);

}


