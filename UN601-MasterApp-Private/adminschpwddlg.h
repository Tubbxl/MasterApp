#ifndef ADMINSCHPWD_H
#define ADMINSCHPWD_H


#include "adminslogin.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class AdminsChPwdDlg;
}

class AdminsChPwdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AdminsChPwdDlg(QWidget *parent = 0);
    ~AdminsChPwdDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

    void updatePromptMsg(QString strPrompt);

    void changeAdminsPwd(QString strFuncType, unsigned char pwdType);
signals:
    void sendShowSettingMenuSignal();

private slots:
    void on_btnBack_clicked();
    void clickAdminsManageMenuItem(QListWidgetItem* item);


    void handleRefreshPrompt(QString strPrompt);
    void closePromptMsg();

    void uiTimeout();

private:
      Ui::AdminsChPwdDlg *ui;
      QWidget *mPtrContainer;

      QThread *mpAdminsLogin;

      QMessageBox *mpPromptMsgBox;
};

#endif // ADMINSCHPWD_H
