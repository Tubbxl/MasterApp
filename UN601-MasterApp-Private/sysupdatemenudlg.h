#ifndef SYSUPDATEMENUDLG_H
#define SYSUPDATEMENUDLG_H

#include "mfile.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class SysUpdateMenuDlg;
}

class SysUpdateMenuDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SysUpdateMenuDlg(QWidget *parent = 0);
    ~SysUpdateMenuDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

private slots:
    void on_btnBack_clicked();
    void clickSysUpdateMenuItem(QListWidgetItem* item);

    void uiTimeout();

private:
      Ui::SysUpdateMenuDlg *ui;
      QWidget *mPtrContainer;
};

#endif // SYSUPDATEMENUDLG_H
