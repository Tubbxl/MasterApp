#include "sdcardupdatedlg.h"
#include "ui_sdcardupdatedlg.h"

#include "updatebootthread.h"
#include "updatekernelthread.h"
#include "update32550osthread.h"
#include "updatesyslibthread.h"
#include "updatemasterappthread.h"
#include "updatesubappthread.h"
#include"updateapiserversthread.h"

#include "mappdefines.h"

#include <QtGui>

SDCardUpdateDlg::SDCardUpdateDlg(int fileType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SDCardUpdateDlg)
{
    ui->setupUi(this);

    initData(fileType);

    initViews();
}

void SDCardUpdateDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void SDCardUpdateDlg::initViews()
{
    mPromptMsgBox = new QMessageBox(this);
     mPromptMsgBox->setWindowTitle(tr("Prompt"));
    mPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
     mPromptMsgBox->setIcon(QMessageBox::Information);
     mPromptMsgBox->setStandardButtons(0);
     mPromptMsgBox->setModal(false);
     ui->listFiles->setFocusPolicy(Qt::NoFocus);
     ui->listFiles->resize(220, 200);

     foreach(QFileInfo fileinfo, mFileList)
     {
          ui->listFiles->addItem(fileinfo.fileName());

              qDebug("fileName :[%s]", qPrintable(fileinfo.fileName()));
     }

     loadStyleSheet("uistyle1");
}

void SDCardUpdateDlg::initData(int fileType)
{
    mFile = new MFile();
    mFileList = mFile->getSpecifiedFileTypeList(SD_CARD_FILE_PATH, fileType);

    miFileType = fileType;

    connect(ui->listFiles, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickSettingMenuItem()));

    mbThreadExit = true;
   QTimer::singleShot( UPDATE_APP_FM_TIME_OUT, this, SLOT(uiTimeout()) );
   mpQTimer = new QTimer(this);
   connect(mpQTimer, SIGNAL(timeout()), this, SLOT(testFunc()));
   mpQTimer->setInterval(WAIT_TIME_OUT);
   mpQTimer->setSingleShot(false);

    qDebug("mFilePathList size:[%d]", mFileList.size());

}

void SDCardUpdateDlg::waitTimeout()
{
    if(mbThreadExit)
    {
        mpQTimer->stop();
        delete mpQTimer;

         on_btnBack_clicked();
    }
}

void SDCardUpdateDlg::uiTimeout()
{
    if(mbThreadExit)
    {
         on_btnBack_clicked();
    }
    else
    {
       mpQTimer->start();
    }
}

void SDCardUpdateDlg::updatePromptMsg(QString strPrompt)
{
      qDebug("updatePromptMsg strPrompt :[%s]", qPrintable(strPrompt));

     if(NULL != mPromptMsgBox)
     {
         mPromptMsgBox->setText(strPrompt);
         mPromptMsgBox->setGeometry(120, 230, 200, 80);
          mPromptMsgBox->show();
     }
}

void SDCardUpdateDlg::closePromptMsg()
{
    mbThreadExit = true;

     if(NULL != mPromptMsgBox)
     {
          mPromptMsgBox->hide();
          mPromptMsgBox->close();
     }
}

SDCardUpdateDlg::~SDCardUpdateDlg()
{
    delete ui;
}

void SDCardUpdateDlg::setContainerPointer(QWidget* ptrContainer)
{
   mPtrContainer = ptrContainer;
}

void SDCardUpdateDlg::handleRefreshPrompt(QString strPrompt)
{
     updatePromptMsg(strPrompt);

}

void SDCardUpdateDlg::updateBoot(QString strFilePath)
{
      mpUpdateBootThread = new UpdateBootThread(strFilePath);
      connect(mpUpdateBootThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdateBootThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdateBootThread->start();
}

void SDCardUpdateDlg::updateKernel(QString strFilePath)
{

      mpUpdateKernelThread = new UpdateKernelThread(strFilePath);
      connect(mpUpdateKernelThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdateKernelThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdateKernelThread->start();
}

void SDCardUpdateDlg::updateSystemLib(QString strFilePath)
{

      mpUpdateSysLibThread = new UpdateSysLibThread(strFilePath);
      connect(mpUpdateSysLibThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdateSysLibThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdateSysLibThread->start();
}

void SDCardUpdateDlg::update32550Os(QString strFilePath)
{
    qDebug("update32550Os");

      mpUpdate32550OsThread = new Update32550OsThread(strFilePath);
      connect(mpUpdate32550OsThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      //connect(((Update32550OsThread*)mpUpdate32550OsThread)->mpLoadApp, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdate32550OsThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdate32550OsThread->start();
        qDebug("update32550Os--thread start");
}

void SDCardUpdateDlg::updateMasterApp(QString strFilePath)
{

      mpUpdateMasterAppThread = new UpdateMasterAppThread(strFilePath);
      connect(mpUpdateMasterAppThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdateMasterAppThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdateMasterAppThread->start();
}

void SDCardUpdateDlg::updateSubApp(QString strFilePath)
{
      mpUpdateSubAppThread = new UpdateSubAppThread(strFilePath);
      connect(mpUpdateSubAppThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
      connect(mpUpdateSubAppThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
      mpUpdateSubAppThread->start();
}

void SDCardUpdateDlg::updateApiservers(QString strFilePath)
{
    mpUpdateApiserversThread = new UpdateApiserversThread(strFilePath);
    connect(mpUpdateApiserversThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpUpdateApiserversThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpUpdateApiserversThread->start();
}

void SDCardUpdateDlg::clickSettingMenuItem()
{

     int index = ui->listFiles->currentRow();

     qDebug("clickSettingMenuItem---index:[%d]", index);

     QString strFilePath = mFileList.at(index).absoluteFilePath();

     qDebug("clickSettingMenuItem---mstrFilePath:[%s]", qPrintable(strFilePath));

     if(strFilePath.length() > SD_CARD_FILE_PATH.length())
     {
         mbThreadExit  = false;

         if(FILE_TYPE_BOOT == miFileType)
         {
             updateBoot(strFilePath);
         }
         else if(FILE_TYPE_KERNEL == miFileType)
         {
              updateKernel(strFilePath);
         }
         else if(FILE_TYPE_32550_OS == miFileType)
         {
             update32550Os(strFilePath);
         }
         else if(FILE_TYPE_SYSTEM_LIB == miFileType)
         {
            updateSystemLib(strFilePath);
         }
         else if(FILE_TYPE_MASTER_APP == miFileType)
         {
            updateMasterApp(strFilePath);
         }
         else if(FILE_TYPE_SUB_APP == miFileType)
         {
            updateSubApp(strFilePath);
         }
         else if(FILE_TYPE_API_SERVER == miFileType)
         {
            updateApiservers(strFilePath);
         }

     }
}

void SDCardUpdateDlg::on_btnBack_clicked()
{
    mPtrContainer->close();
}
