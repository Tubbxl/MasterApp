#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QtGui>
#include <QDialog>
#include <QListWidget>

#include <QMessageBox>

#include <QString>

#include "updatesubapps.h"
#include "mappdefines.h"
#include "checkstatusthread.h"
#include "mfile.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    void initViews();
    void initData();
    void refreshAppsList();

    void loadStyleSheet(const QString &sheetName);
    void setParentPointer(QWidget * parent);

    int runApp(QString strAppPath);

    void updatePromptMsg(QString strPrompt);
    void refreshPromptAndExit(QString strPrompt);

    void hideAllCtrls();
    void showAllCtrls();

private slots:
    void on_btnSetting_clicked();
    void exitApp();
    void clickSubAppListItem(QListWidgetItem* item);

    void subAppStarted();
    void subAppFinished(int, QProcess::ExitStatus);
    void subAppErrorHandler(QProcess::ProcessError);

    void handleRefreshAppsList();

    void handleRefreshPrompt(QString strPrompt);
    void closePromptMsg();
    void handleUpdate32550OsPrompt();
    void handleMainUIRefresh();

private:
    Ui::MainDialog *ui;
    QWidget *mParent;
    QProcess *mProcess;
    MFile *mMFile;

    UpdateSubApps *mpUpdateSubApps;
    QString mAppsList[SUB_APP_MAX_NUMS];

    QMessageBox *mpPromptMsgBox;

    CheckStatusThread *mpCheckStatusThread;
};

#endif // MAINDIALOG_H
