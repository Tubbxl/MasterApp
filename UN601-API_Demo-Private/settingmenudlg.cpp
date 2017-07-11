#include "settingmenudlg.h"
#include "ui_settingmenudlg.h"

#include "settingmenu.h"
#include "versioninfodlg.h"
#include "lcd.h"
//#include "common.h"
#include "dialog.h"
//#include "keypad.h"
#include "wifisetting.h"
#include "ethernetsetting.h"
#include "gprssetting.h"
#include "sdcard.h"
#include "serial.h"
#include "setdatetimedlg.h"
#include "scanner.h"
#include "signature.h"

#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QProcess>
#include <QImage>
SettingMenuDlg::SettingMenuDlg(QString  (*pStrList)[18], int listSize,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingMenuDlg)
{
    ui->setupUi(this);
    mIndex = 0;
    mpStrMenuUI = pStrList;
    miMenuGroup = listSize;

    initViews();
}

void SettingMenuDlg::setMenuNode(QString strList[],  int listSize, QWidget * container, QVBoxLayout *layout)
{
    QLabel* labelNode = new QLabel(container);
    labelNode->setText((strList[0]));
    layout->addWidget(labelNode);
    layout->setStretchFactor(labelNode, 1);
    mMenuList = strList;
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

  //  qApp->setStyleSheet(styleSheet);
    setStyleSheet(styleSheet);
    file.close();
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
  /*  mPtrContainer->setDisabled(true);

    mpDownloadCAThread = new DownloadCAThread();
    connect(mpDownloadCAThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadCAThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadCAThread->start();*/
}

void SettingMenuDlg::downloadFmSignKey()
{
   /* mPtrContainer->setDisabled(true);

    mpDownloadFmSignKeyThread = new DownloadFmSignKey();
    connect(mpDownloadFmSignKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadFmSignKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    connect(mpDownloadFmSignKeyThread, SIGNAL(closeSettingDlgSignal()), this, SLOT(closeSettingDlg()));
    mpDownloadFmSignKeyThread->start();*/
}

void SettingMenuDlg::downloadAppSignKey()
{
    /*mPtrContainer->setDisabled(true);

    mpDownloadAppSignKeyThread = new DownloadAppSignKey();
    connect(mpDownloadAppSignKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadAppSignKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadAppSignKeyThread->start();*/
}

void SettingMenuDlg::downloadMKey()
{
/*    mPtrContainer->setDisabled(true);

    mpDownloadMKeyThread = new DownloadMKey();
    connect(mpDownloadMKeyThread, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpDownloadMKeyThread, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    mpDownloadMKeyThread->start();*/
}

void SettingMenuDlg::verifyAdminsPwd(QString strFuncType)
{
  /*  mPtrContainer->setDisabled(true);

    mpAdminsLogin = new AdminsLogin(ADMINS_VERIFY_PWD, strFuncType);
    connect(mpAdminsLogin, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpAdminsLogin, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));
    connect(mpAdminsLogin, SIGNAL(invokeFuncTypeSignal(QString)), this, SLOT(handleFuncType(QString)));


    mpAdminsLogin->start();*/
}

bool SettingMenuDlg::needVerifyPwd(QString strFuncType)
{
 //return false;

/*    mPtrContainer->hide();

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
*/
    return false;
}

void SettingMenuDlg::handleFuncType(QString strFuncType)
{
    qDebug("handleFuncType---strFuncType:[%s]", qPrintable(strFuncType));
    mPtrContainer->setEnabled(true);
    mPtrContainer->show();

     if(0 == QString::compare(strFuncType, FUNC_TYPE_MSR_TEST))   //MSR Test
    {

                Dialog *MSRDlg = new Dialog(&FUNC_TYPE_MSR_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(MSRDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                MSRDlg->setContainerPointer(view);
                connect(MSRDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_NFC_TEST))//Picc
    {

                Dialog *PiccDlg = new Dialog(&FUNC_TYPE_NFC_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(PiccDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                PiccDlg->setContainerPointer(view);
                connect(PiccDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_PRINTER_TEST)) //printer
    {

                Dialog *PrinterDlg = new Dialog(&FUNC_TYPE_PRINTER_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(PrinterDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                PrinterDlg->setContainerPointer(view);
                connect(PrinterDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_SYS_TEST)) //sys
    {

                Dialog *SysDlg = new Dialog(&FUNC_TYPE_SYS_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(SysDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                SysDlg->setContainerPointer(view);
                connect(SysDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
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
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_BAR_CODE_TEST))//scan
    {

                Scanner *ScanDlg = new Scanner;

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(ScanDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                ScanDlg->setContainerPointer(view);
                connect(ScanDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_IC_CARD_TEST)) //Ic
    {

                Dialog *IccDlg = new Dialog(&FUNC_TYPE_IC_CARD_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(IccDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                IccDlg->setContainerPointer(view);
                connect(IccDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_PCI_TEST)) //Ic
    {

                Dialog *PciDlg = new Dialog(&FUNC_TYPE_PCI_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(PciDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                PciDlg->setContainerPointer(view);
                connect(PciDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0 == QString::compare(strFuncType, FUNC_TYPE_W_SN_TEST)) //Ic
    {

                Dialog *SnDlg = new Dialog(&FUNC_TYPE_W_SN_TEST);

                QGraphicsScene *scene = new QGraphicsScene;
                QGraphicsProxyWidget *proxy = scene->addWidget(SnDlg);
                proxy->setRotation(270);
                QGraphicsView *view = new QGraphicsView(scene);
                view->resize(320, 240);
                view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                SnDlg->setContainerPointer(view);
                connect(SnDlg,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
                 mPtrContainer->close();
                view->show();
    }
    else if(0==QString::compare(strFuncType,FUNC_TYPE_ETH_TEST))
        {
            Ethernetsetting *Ethernet = new Ethernetsetting();

            mPtrContainer->close();
            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *proxy = scene->addWidget(Ethernet);
            proxy->setRotation(270);
            QGraphicsView *view = new QGraphicsView(scene);
            view->resize(320, 240);
            view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            Ethernet->setContainerPointer(view);
            connect(Ethernet,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
            view->show();
        }
    else if(0==QString::compare(strFuncType,FUNC_TYPE_WIFI_TEST))
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
            connect(wifi,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
            view->show();
        }
       else if(0==QString::compare(strFuncType,FUNC_TYPE_GPRS_TEST))
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
            connect(gprs,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
            view->show();
        }


    else if(0==QString::compare(strFuncType,FUNC_TYPE_UART_TEST))
        {
            serial *ser = new serial();

            QGraphicsScene *scene = new QGraphicsScene;
            QGraphicsProxyWidget *proxy = scene->addWidget(ser);
            proxy->setRotation(270);
            QGraphicsView *view = new QGraphicsView(scene);
            view->resize(320, 240);
            view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            ser->setContainerPointer(view);
            connect(ser,SIGNAL(return_test_result(int)),this,SLOT(test_result(int)) );
            view->show();
        }
     else if(0==QString::compare(strFuncType,FUNC_TYPE_SIG_TEST))
         {
                signature *sig = new signature(this);
         }
}

void SettingMenuDlg::showSettingMenu()
{
    mPtrContainer->show();
}

void SettingMenuDlg::test_result(int ret)
{
    showSettingMenu();
    if(ret == 0)
    {
    //    mLaseTest->setTextColor(QColor("blue"));
    }
    else
    {
       //  mLaseTest->setTextColor(QColor("red"));
    }
    mIndex++;
 //  handleFuncType(mMenuList[mIndex+3]);
}


void SettingMenuDlg::clickSettingMenuItem(QListWidgetItem* item)
{
    mLaseTest = item;
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
