#include "adminschpwddlg.h"
#include "ui_adminschpwddlg.h"

#include "updatekernelthread.h"

#include "mappdefines.h"
#include "settingmenu.h"

#include "sdcardupdatedlg.h"

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

AdminsChPwdDlg::AdminsChPwdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminsChPwdDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void AdminsChPwdDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void AdminsChPwdDlg::initViews()
{
    mpPromptMsgBox = new QMessageBox(this);
     mpPromptMsgBox->setWindowTitle(tr("Prompt"));
    mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
     mpPromptMsgBox->setIcon(QMessageBox::Information);
     mpPromptMsgBox->setStandardButtons(0);
     mpPromptMsgBox->setModal(false);

     ui->listAdminsChPwd->setFocusPolicy(Qt::NoFocus);
     ui->listAdminsChPwd->resize(234, 200);

     ui->labelAdminsManage->setText(gb_Admins_Manage_UI[0]);

       int iCount =   sizeof(gb_Admins_Manage_UI)/sizeof(gb_Admins_Manage_UI[0]);
       int i = 0;
       for(i = 1; i < iCount; i++)
       {
             ui->listAdminsChPwd->addItem(gb_Admins_Manage_UI[i]);
       }

     loadStyleSheet("uistyle1");
}

void AdminsChPwdDlg::initData()
{

    connect(ui->listAdminsChPwd, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickAdminsManageMenuItem(QListWidgetItem*)));


    QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );

}

void AdminsChPwdDlg::uiTimeout()
{
    on_btnBack_clicked();
}

AdminsChPwdDlg::~AdminsChPwdDlg()
{
    delete ui;
}

void AdminsChPwdDlg::setContainerPointer(QWidget* ptrContainer)
{
   mPtrContainer = ptrContainer;
}

void AdminsChPwdDlg::updatePromptMsg(QString strPrompt)
{
      qDebug("updatePromptMsg strPrompt :[%s]", qPrintable(strPrompt));

     if(NULL != mpPromptMsgBox)
     {
         mpPromptMsgBox->setText(strPrompt);
     //    mPromptMsgBox->setGeometry(120, 230, 200, 80);
         mpPromptMsgBox->move(120, 230);
         mpPromptMsgBox->setFixedSize(200, 80);
          mpPromptMsgBox->show();
     }
}

void AdminsChPwdDlg::handleRefreshPrompt(QString strPrompt)
{
    mPtrContainer->show();

    updatePromptMsg(strPrompt);

}

void AdminsChPwdDlg::closePromptMsg()
{
     mPtrContainer->setEnabled(true);
      mPtrContainer->show();

     if(NULL != mpPromptMsgBox)
     {
          mpPromptMsgBox->hide();
          mpPromptMsgBox->close();
     }
}

void AdminsChPwdDlg::changeAdminsPwd(QString strFuncType, unsigned char pwdType)
{
    mPtrContainer->setDisabled(true);
    mPtrContainer->hide();

    mpAdminsLogin = new AdminsLogin(ADMINS_CHANGE_PWD, strFuncType, pwdType);
    connect(mpAdminsLogin, SIGNAL(refreshPromptSignal(QString)), this, SLOT(handleRefreshPrompt(QString)));
    connect(mpAdminsLogin, SIGNAL(closePromptSignal()), this, SLOT(closePromptMsg()));

    mpAdminsLogin->start();
}

void AdminsChPwdDlg::clickAdminsManageMenuItem(QListWidgetItem* item)
{

    QString strText = item->text();


    if(0 == QString::compare(strText, FUNC_TYPE_ADMIN_A_CHANGE_PWD))
    {
        changeAdminsPwd(strText, PWD_TYPE_ADMIN_A);
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_ADMIN_B_CHANGE_PWD))
    {
       changeAdminsPwd(strText, PWD_TYPE_ADMIN_B);
    }


}

void AdminsChPwdDlg::on_btnBack_clicked()
{

    mPtrContainer->close();

     sendShowSettingMenuSignal();
}
