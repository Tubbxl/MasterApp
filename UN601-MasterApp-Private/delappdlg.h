#ifndef DELAPPDLG_H
#define DELAPPDLG_H

#include "mfile.h"

#include "updatesubapps.h"

#include "mappdefines.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

namespace Ui {
class DelAppDlg;
}

class DelAppDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DelAppDlg(QWidget *parent = 0);
    ~DelAppDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

    void updatePromptMsg(QString strPrompt);

    void updatePromptMsgAndExit(QString strPrompt);

private slots:
     void clickSubAppListItem(QListWidgetItem* item);
      void closePromptMsg();

    void on_btnBack_clicked();

    void uiTimeout();

private:
    Ui::DelAppDlg *ui;
    QWidget *mPtrContainer;

  UpdateSubApps *mpUpdateSubApps;
  QString mAppsList[SUB_APP_MAX_NUMS];

    QMessageBox *mpPromptMsgBox;


};

#endif // DELAPPDLG_H
