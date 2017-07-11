#include "delappdlg.h"
#include "ui_delappdlg.h"

#include "mappdefines.h"

extern "C"
{
    #include "include/dll_api.h"
}


#include <QtGui>

#include <QDebug>

DelAppDlg::DelAppDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelAppDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void DelAppDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void DelAppDlg::initViews()
{
    mpPromptMsgBox = new QMessageBox(this);
     mpPromptMsgBox->setWindowTitle(tr("Prompt"));
    mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
     mpPromptMsgBox->setIcon(QMessageBox::Information);
     mpPromptMsgBox->setStandardButtons(0);
     mpPromptMsgBox->setModal(false);

    ui->listApps->setFocusPolicy(Qt::NoFocus);
     ui->listApps->resize(220, 200);

    ui->listApps->clear();

     mpUpdateSubApps->loadAppsList(mAppsList);

    for(int i = 0; i < SUB_APP_MAX_NUMS; i++)
    {
           if(mAppsList[i].length() <=0)
               break;

           ui->listApps->addItem(mAppsList[i]);
    }

     loadStyleSheet("uistyle1");
}

void DelAppDlg::initData()
{
    mpUpdateSubApps = new UpdateSubApps();
    connect( ui->listApps, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickSubAppListItem(QListWidgetItem*)));

      QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );
}

 void DelAppDlg::setContainerPointer(QWidget* ptrContainer)
 {
    mPtrContainer = ptrContainer;
 }

DelAppDlg::~DelAppDlg()
{
    delete ui;
}

void DelAppDlg::uiTimeout()
{
    on_btnBack_clicked();
}

void DelAppDlg::updatePromptMsg(QString strPrompt)
{
     if(NULL != mpPromptMsgBox)
     {
         mpPromptMsgBox->setText(strPrompt);
         mpPromptMsgBox->move(120, 230);
         mpPromptMsgBox->setFixedSize(200, 80);
          mpPromptMsgBox->show();

           mPtrContainer->setDisabled(true);
     }
}

void DelAppDlg::closePromptMsg()
{
     if(NULL != mpPromptMsgBox)
     {
          mpPromptMsgBox->hide();
          mpPromptMsgBox->close();

           mPtrContainer->setEnabled(true);
     }
}

void DelAppDlg::updatePromptMsgAndExit(QString strPrompt)
{
    updatePromptMsg(strPrompt);
    QTimer::singleShot( 2000,this, SLOT(closePromptMsg()) );
}

void DelAppDlg::clickSubAppListItem(QListWidgetItem* item)
{
    QString strAppName= item->text();

    QByteArray arrAppName = strAppName.toLatin1();
     char *appName = arrAppName.data();

     qDebug("clickSubAppListItem appName:[%s]", appName);

     char szAppName[32] = {0};

     strcpy(szAppName, appName);

     int appNo = -1;
     appNo = mpUpdateSubApps->getAppNo(szAppName);

     if(appNo < 0)
     {
         updatePromptMsgAndExit("Get app no failed!");
         return ;
     }

        int iRet = -1;
     iRet = DLL_PciClearAppkeys(appNo);
     if(0 != iRet)
     {
         updatePromptMsgAndExit("Delete app keys failed!");
         return;
     }

   char filePath[256] = {0};
   char szShellCmd[256] = {0};
   sprintf(filePath, "/home/subapps/subapp%d/", appNo);
   strcat(filePath, szAppName);
   sprintf(szShellCmd, "rm -rf %s", filePath);
   iRet = system(szShellCmd);
   system("sync");
   if(0 != iRet)
   {
       updatePromptMsgAndExit("Delete app file failed!");
       return ;
   }

     int index = -1;
     iRet = mpUpdateSubApps->delAppName(appName);
     system("sync");
     qDebug("clickSubAppListItem---delAppName iRet[%d]", iRet);
     if(0 != iRet)
     {
         updatePromptMsgAndExit("Delete app info failed!");
        return;
     }



    index  = ui->listApps->row(item);
    ui->listApps->takeItem(index);

}

void DelAppDlg::on_btnBack_clicked()
{
    mPtrContainer->close();
}
