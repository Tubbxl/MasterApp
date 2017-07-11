#ifndef PROMPTUPDATEOSDLG_H
#define PROMPTUPDATEOSDLG_H

#include "mfile.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class PromptUpdateOsDlg;
}

class PromptUpdateOsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PromptUpdateOsDlg(QWidget *parent = 0);
    ~PromptUpdateOsDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

private slots:

    void on_btnConfirm_clicked();

private:
    Ui::PromptUpdateOsDlg *ui;
      QWidget *mPtrContainer;

};

#endif // PROMPTUPDATEOSDLG_H
