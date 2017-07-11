#ifndef VERSIONINFODLG_H
#define VERSIONINFODLG_H

#include "mfile.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class VersionInfoDlg;
}

class VersionInfoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit VersionInfoDlg(QWidget *parent = 0);
    ~VersionInfoDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

     char* getSecurityChipBootVer();
     int getSecurityChipBootVer(char * bootVer);
     int getSecurityChipOsVer(char * osVer);
     int getTotalFmVer(char *  totalVer);
     char*  getSecurityChipOsVer();
     char*  getLinuxKernelVer();
     char* getUbootVer();
     char* getMasterAppVer();

private slots:

    void on_btnBack_clicked();

private:
    Ui::VersionInfoDlg *ui;
      QWidget *mPtrContainer;

};

#endif // VERSIONINFODLG_H
