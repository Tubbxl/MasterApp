#ifndef SETTINGMENUDLG_H
#define SETTINGMENUDLG_H

#include <QtGui>
#include <QScrollArea>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SettingMenuDlg;
}

class SettingMenuDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingMenuDlg(QString  (*pStrList)[18], int listSize, QWidget *parent = 0);
    ~SettingMenuDlg();
    void loadStyleSheet(const QString &sheetName);
    void  setBackBtn(QWidget * container, QVBoxLayout *layout);
    void setMenuNode(QString strList[], int listSize, QWidget * container, QVBoxLayout *layout);
    int setMenu(QString  (*pStrList)[18], int listSize, QWidget * container, QVBoxLayout *layout);

    void updatePromptMsg(QString strPrompt);

    void initViews();
    void initData();

    void setContainerPointer(QWidget* ptrContainer);

     void downloadCA();
     void downloadFmSignKey();
     void downloadAppSignKey();
     void downloadMKey();

     bool needVerifyPwd(QString strFuncType);

     void verifyAdminsPwd(QString strFuncType);

     int runExApp(QString strAppPath);
  //   void changeAdminsPwd(QString strFuncType);





private:
    Ui::SettingMenuDlg *ui;
    QWidget *mPtrContainer;
    QMessageBox *mPromptMsgBox;
    QString (*mpStrMenuUI)[18];
    int miMenuGroup;


  QProcess *mProcess;

    QThread *mpDownloadCAThread;
    QThread *mpDownloadFmSignKeyThread;
    QThread *mpDownloadAppSignKeyThread;
     QThread *mpDownloadMKeyThread;

    QThread *mpAdminsLogin;



private slots:
    void clickBack();
    void clickSettingMenuItem(QListWidgetItem* item);

    void handleRefreshPrompt(QString strPrompt);
    void closePromptMsg();
    void closeSettingDlg();

     void handleFuncType(QString strFuncType);

     void showSettingMenu();

     void exAppStarted();
     void exAppFinished(int, QProcess::ExitStatus);
     void exAppErrorHandler(QProcess::ProcessError);
     void exitExApp();

signals:
    void refreshAppsListSignal();

    void refreshMainUISignal();

};

#endif // SETTINGMENUDLG_H
