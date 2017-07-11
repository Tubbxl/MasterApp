#ifndef DISPLAYAPPLIST_H
#define DISPLAYAPPLIST_H

#include "mfile.h"

#include "updatesubapps.h"

#include "mappdefines.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

#include "mfile.h"

namespace Ui {
class DisplayAppList;
}

class DisplayAppList : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayAppList(QWidget *parent = 0);
    ~DisplayAppList();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();

    int getAppVerInfo( char * ptrAppName,  char *ptrAppVer);

private slots:

    void on_btnBack_clicked();

private:
    Ui::DisplayAppList *ui;
      QWidget *mPtrContainer;

      UpdateSubApps *mpUpdateSubApps;
      QString mAppsList[SUB_APP_MAX_NUMS];

      MFile *mpMFile;

};

#endif // DISPLAYAPPLIST_H
