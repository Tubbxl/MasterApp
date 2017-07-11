#ifndef SDCARDUPDATEDLG_H
#define SDCARDUPDATEDLG_H

#include "mfile.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class SDCardUpdateDlg;
}

class SDCardUpdateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SDCardUpdateDlg(int fileType, QWidget *parent = 0);
    ~SDCardUpdateDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData(int fileType);
    void initViews();
    void updatePromptMsg(QString strPrompt);

    void updateBoot(QString strFilePath);
    void updateKernel(QString strFilePath);
    void update32550Os(QString strFilePath);
    void updateSystemLib(QString strFilePath);
    void updateMasterApp(QString strFilePath);
    void updateSubApp(QString strFilePath);
    void updateApiservers(QString strFilePath);

private slots:
    void on_btnBack_clicked();
    void handleRefreshPrompt(QString strPrompt);
    void closePromptMsg();
    void clickSettingMenuItem();

    void uiTimeout();
     void waitTimeout();

private:
    Ui::SDCardUpdateDlg *ui;
      QWidget *mPtrContainer;

    QThread *mpUpdateBootThread;
    QThread *mpUpdateKernelThread;
    QThread *mpUpdate32550OsThread;
    QThread *mpUpdateSysLibThread;
    QThread *mpUpdateMasterAppThread;
    QThread *mpUpdateSubAppThread;
    QThread *mpUpdateApiserversThread;


    QMessageBox *mPromptMsgBox;

    MFile *mFile;
    QFileInfoList mFileList;
    int miFileType;

    bool mbThreadExit;

    QTimer *mpQTimer;
};

#endif // SDCARDUPDATEDLG_H
