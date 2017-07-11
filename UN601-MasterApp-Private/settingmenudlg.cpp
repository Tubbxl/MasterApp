#include "settingmenudlg.h"
#include "ui_settingmenudlg.h"

#include "mappdefines.h"
#include "settingmenu.h"

#include "sdcardupdatedlg.h"
#include "sysupdatemenudlg.h"

#include "downloadcathread.h"
#include "downloadfmsignkey.h"
#include "downloadappsignkey.h"
#include "downloadmkey.h"

#include "setdatetimedlg.h"

#include "displayapplist.h"

#include "versioninfodlg.h"

#include "adminslogin.h"

#include "adminschpwddlg.h"

#include "delappdlg.h"

#include "network.h"

#include "wifisetting.h"
#include"ethernetsetting.h"
#include "gprssetting.h"

#include <QGraphicsView>
#include <QGraphicsProxyWidget>


SettingMenuDlg::SettingMenuDlg(QString  (*pStrList)[18], int listSize,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingMenuDlg)
{
    ui->setupUi(this);

    mpStrMenuUI = pStrList;
    miMenuGroup = listSize;

    initViews();

    initData();
}

void SettingMenuDlg::setMenuNode(QString strList[],  int listSize, QWidget * container, QVBoxLayout *layout)
{
    QLabel* labelNode = new QLabel(container);
    labelNode->setText((strList[0]));
    layout->addWidget(labelNode);
    layout->setStretchFactor(labelNode, 1);

    QListWidget  *ctrlList = new QListWidget(container);
    ctrlList->setFocusPolicy(Qt::NoFocus);

    int i = 0;
    //  qDebug("start---setMenuNode---count:[%d]", listSize);

    for ( i=0; i < listSize; i++)
    {
        //     qDebug("etMenuNode---index :[%d]", i);
        ctrlList->addItem(strList[i+2]);
    }

    //     qDebug("setMenuNode---height:[%d]",  i*35);
    layout->addWidget(ctrlList);

    if(listSize > 0)
    {
        qDebug("etMenuNode---listSize - 1 :[%d]", listSize);
        layout->setStretchFactor(ctrlList, listSize);
    }

    connect(ctrlList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickSettingMenuItem(QListWidgetItem*)));
}

void  SettingMenuDlg::setBackBtn(QWidget * container, QVBoxLayout *layout)
{
    QHBoxLayout *hLayout = new QHBoxLayout;

    QPushButton *btnBack = new QPushButton(container);
    btnBack->setFixedSize(120, 35);
    btnBack->setText("Back");
    hLayout->addWidget(btnBack);

    layout->addLayout(hLayout);
    layout->setStretchFactor(hLayout, 1);

    connect(btnBack, SIGNAL(clicked()), this, SLOT(clickBack()));
}

int SettingMenuDlg::setMenu(QString  (*pStrList)[18], int listSize, QWidget * container, QVBoxLayout *layout)
{
    int i = 0;
    int iCountMenuNode = 0;
    int iCountTotal = 0;
    for(i = 0; i < listSize; i++)
    {
        iCountMenuNode = pStrList[i][1].toInt();
        qDebug("setMenu---iCountMenuNode:[%d]", iCountMenuNode);

        setMenuNode(pStrList[i], iCountMenuNode,  container, layout);

        iCountTotal += (iCountMenuNode + 1);
    }

    return iCountTotal;
}


void SettingMenuDlg::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void SettingMenuDlg::exAppStarted()
{
    qDebug("exAppStarted");
       mPtrContainer->setDisabled(true);
}

void SettingMenuDlg::exAppFinished(int, QProcess::ExitStatus)
{
      qDebug("exAppFinished");

    QTimer::singleShot( 500,this, SLOT(exitExApp()) );
}

void SettingMenuDlg::exAppErrorHandler(QProcess::ProcessError)
{
     qDebug("exAppErrorHandler");
}

void SettingMenuDlg::exitExApp()
{
     qDebug("exitApp---function");
    mPtrContainer->setEnabled(true);
}

int SettingMenuDlg::runExApp(QString strAppPath)
{
   if(NULL == strAppPath)
       return -1;

    mProcess->start(strAppPath);

    return 0;
}

void SettingMenuDlg::initData()
{
    mProcess = new QProcess();

    connect(mProcess, SIGNAL(started()), SLOT(exAppStarted()));
    connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(exAppFinished(int, QProcess::ExitStatus)));
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)), SLOT(exAppErrorHandler (QProcess::ProcessError)));
}

void SettingMenuDlg::initViews()
{

    mPromptMsgBox = new QMessageBox(this);
    mPromptMsgBox->setWindowTitle(tr("Prompt"));
    mPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
    mPromptMsgBox->setIcon(QMessageBox::Information);
    mPromptMsgBox->setStandardButtons(0);
    mPromptMsgBox->setModal(false);


    //滚动区域
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setGeometry(0, 0, 240, 320);
    //垂直滚动条不可见，只能通过鼠标滑动
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //窗体1，其父窗体为滚动区域
    QWidget *form= new QWidget(scrollArea);
    //设置滚动区域的窗体
    scrollArea->setWidget(form);

    QVBoxLayout *vLayout = new QVBoxLayout;

    int iCountTotal = 0;
    iCountTotal = setMenu(mpStrMenuUI, miMenuGroup, form, vLayout);
    qDebug("iCountTotal:[%d]", iCountTotal);

    //Back Button
    setBackBtn(form, vLayout);
    //back btn
    iCountTotal += 1;

    form->setLayout(vLayout);

    int pxHeight  = 36;
    if(iCountTotal/miMenuGroup < 4)
    {
        pxHeight = 42;

//        if(miMenuGroup < 5)
//            pxHeight = 50;
    }

    form->setGeometry(0, 0, 240, iCountTotal*pxHeight);


    loadStyleSheet("uistyle1");
}

void SettingMenuDlg::updatePromptMsg(QString strPrompt)
{
    mPtrContainer->show();

    if(NULL != mPromptMsgBox)
    {
        mPromptMsgBox->setText(strPrompt);
        mPromptMsgBox->setGeometry(120, 230, 200, 80);
        mPromptMsgBox->show();
    }
}

void SettingMenuDlg::closePromptMsg()
{
      mPtrContainer->setEnabled(true);
      mPtrContainer->show();
    if(NULL != mPromptMsgBox)
    {
        mPtrContainer->setDisabled(false);
        mPromptMsgBox->hide();
        mPromptMsgBox->close();
    }
}

void SettingMenuDlg::closeSettingDlg()
{
   clickBack();
}

void SettingMenuDlg::handleRefreshPrompt(QString strPrompt)
{
    updatePromptMsg(strPrompt);

}

void SettingMenuDlg::setContainerPointer(QWidget* ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void SettingMenuDlg::clickBack()
{
    refreshAppsListSignal();

     refreshMainUISignal();

//    if(gb_RefreshMainUI == true)
//    {

//        gb_RefreshMainUI = false;
//    }

    mPtrContainer->close();
}

void SettingMenuDlg::downloadCA()
{
    mPtrContainer->setDisabled(true);

    mpDownloadCAThread = new DownloadCAThread();
    connect(mpDownloadCAThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadCAThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadCAThread->start();
}

void SettingMenuDlg::downloadFmSignKey()
{
    mPtrContainer->setDisabled(true);

    mpDownloadFmSignKeyThread = new DownloadFmSignKey();
    connect(mpDownloadFmSignKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadFmSignKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    connect(mpDownloadFmSignKeyThread, SIGNAL(closeSettingDlgSignal()), this, SLOT(closeSettingDlg()));
    mpDownloadFmSignKeyThread->start();
}

void SettingMenuDlg::downloadAppSignKey()
{
    mPtrContainer->setDisabled(true);

    mpDownloadAppSignKeyThread = new DownloadAppSignKey();
    connect(mpDownloadAppSignKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadAppSignKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadAppSignKeyThread->start();
}

void SettingMenuDlg::downloadMKey()
{
    mPtrContainer->setDisabled(true);

    mpDownloadMKeyThread = new DownloadMKey();
    connect(mpDownloadMKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadMKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadMKeyThread->start();
}

void SettingMenuDlg::verifyAdminsPwd(QString strFuncType)
{
    mPtrContainer->setDisabled(true);

    mpAdminsLogin = new AdminsLogin(ADMINS_VERIFY_PWD, strFuncType);
    connect(mpAdminsLogin, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpAdminsLogin, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    connect(mpAdminsLogin, SIGNAL(invokeFuncTypeSignal(QString)), this, SLOT(handleFuncType(QString)));


    mpAdminsLogin->start();
}

bool SettingMenuDlg::needVerifyPwd(QString strFuncType)
{
 //return false;

    mPtrContainer->hide();

    if(0 == QString::compare(strFuncType, FUNC_TYPE_CA_DOWNLOAD) ||
            0 == QString::compare(strFuncType, FUNC_TYPE_SIGN_KEY_DOWNLOAD)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_CHANGE_PASSWORD)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_APPLICATION_MANAGE)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_APP_KEYS_DOWNLOAD)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_PAYMENT_KEY_DOWNLOAD)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_SDCARD_APP_UPDATE)  ||
            0 == QString::compare(strFuncType, FUNC_TYPE_OP_TEST) )
    {
        return true;
    }

    if(RELEASE_MODE == gb_RunMode)
    {
        if(0 == QString::compare(strFuncType, FUNC_TYPE_DATE_TIME_SETTING)  ||
                0 == QString::compare(strFuncType, FUNC_TYPE_SDCARD_SYSTEM_UPDATE) )
        {
            return true;
        }
    }

    return false;
}

void SettingMenuDlg::handleFuncType(QString strFuncType)
{
    qDebug("handleFuncType---strFuncType:[%s]", qPrintable(strFuncType));
    mPtrContainer->setEnabled(true);
    mPtrContainer->show();

    if(0 == QString::compare(strFuncType, FUNC_TYPE_SDCARD_SYSTEM_UPDATE))
    {
        SysUpdateMenuDlg *sysUpdateMenuDlg = new SysUpdateMenuDlg;

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(sysUpdateMenuDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        sysUpdateMenuDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_SDCARD_APP_UPDATE))
    {
        int fileType = -1;
        fileType = FILE_TYPE_SUB_APP;
        SDCardUpdateDlg *sdcardUpdateDlg = new SDCardUpdateDlg(fileType);

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(sdcardUpdateDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        sdcardUpdateDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_DATE_TIME_SETTING))
    {
        SetDateTimeDlg *setDateTimeDlg = new SetDateTimeDlg();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(setDateTimeDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        setDateTimeDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_APPLICATION_LIST))
    {
        DisplayAppList *displayAppListDlg = new DisplayAppList();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(displayAppListDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        displayAppListDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_CHANGE_PASSWORD))
    {
        mPtrContainer->hide();

        AdminsChPwdDlg *adminsChPwdDlg = new AdminsChPwdDlg();

          connect(adminsChPwdDlg, SIGNAL(sendShowSettingMenuSignal()), this, SLOT(showSettingMenu()));

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(adminsChPwdDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        adminsChPwdDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_OP_TEST))
    {
          network *netWorkDlg = new network;

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(netWorkDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        netWorkDlg->setContainerPointer(view);

        view->show();


    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_APPLICATION_MANAGE))
    {
        DelAppDlg *delAppDlg = new DelAppDlg();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(delAppDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        delAppDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_TEST_TOOL))
    {
         runExApp(FILE_PATH_FACTORY_TEST);//runExApp(FILE_PATH_PRODUCTION_TEST);
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_APIDEMO_TOOL))
    {
          runExApp(FILE_PATH_PRODUCTION_TEST);
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_VERSION_INFO))
    {
        qDebug("start VersionInfoDlg");

        VersionInfoDlg *versionInfoDlg = new VersionInfoDlg();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(versionInfoDlg);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        versionInfoDlg->setContainerPointer(view);

        view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_CA_DOWNLOAD))
    {
        downloadCA();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_SIGN_KEY_DOWNLOAD))
    {
        downloadFmSignKey();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_APP_KEYS_DOWNLOAD))
    {
        downloadAppSignKey();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_PAYMENT_KEY_DOWNLOAD))
    {
        downloadMKey();
    }

    // Network setting
    else if(0==QString::compare(strFuncType,FUNC_TYPE_ETHERNET_SETTING))
    {
        Ethernetsetting *Ethernet = new Ethernetsetting();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(Ethernet);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        Ethernet->setContainerPointer(view);
        connect(Ethernet,&Ethernetsetting::Back,
                [=]
            {
                view->close();
                 showSettingMenu();
            }
                );
        view->show();
    }
    else if(0==QString::compare(strFuncType,FUNC_TYPE_WIFI_SETTING))
    {
        Wifisetting *wifi = new Wifisetting();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(wifi);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        wifi->setContainerPointer(view);
        connect(wifi,&Wifisetting::cancel,
                [=]
            {
                 view->close();
                 showSettingMenu();
            }
                );
        view->show();
    }
    else if(0==QString::compare(strFuncType,FUNC_TYPE_3G_4G_SETTING))
    {
        Gprssetting *gprs = new Gprssetting();

        QGraphicsScene *scene = new QGraphicsScene;
        QGraphicsProxyWidget *proxy = scene->addWidget(gprs);
        proxy->setRotation(270);
        QGraphicsView *view = new QGraphicsView(scene);
        view->resize(320, 240);
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gprs->setContainerPointer(view);
        connect(gprs,&Gprssetting::back,
                [=]
            {
                 view->close();
                showSettingMenu();
            }
                );
        view->show();
    }

}

void SettingMenuDlg::showSettingMenu()
{
    mPtrContainer->show();
}


void SettingMenuDlg::clickSettingMenuItem(QListWidgetItem* item)
{
    QString strText = item->text();

    if(true == needVerifyPwd(strText))
    {
        verifyAdminsPwd(strText);
    }
    else
    {
        handleFuncType(strText);
    }
}

SettingMenuDlg::~SettingMenuDlg()
{
    delete ui;
}
