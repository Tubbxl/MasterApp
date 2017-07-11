#ifndef SETDATETIMEDLG_H
#define SETDATETIMEDLG_H

#include "mfile.h"

#include <QDialog>
#include <QMessageBox>
#include <QListWidget>

#include <QString>

namespace Ui {
class SetDateTimeDlg;
}

class SetDateTimeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SetDateTimeDlg(QWidget *parent = 0);
    ~SetDateTimeDlg();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void initData();
    void initViews();
    void refreshDisplayDateTime();
    void updatePromptMsg(QString strPrompt);


    int getCurrDateTime(char* pszDate,  char* pszTime);
    int setDateTime(char* pszDate,  char* pszTime);

    void stringToHex(unsigned char *pbyInData, int iInLen, unsigned char *pbyOut);



private slots:
    void closePromptMsg();

    void on_btnBack_clicked();

    void on_btnConfirm_clicked();

     void uiTimeout();

private:
    Ui::SetDateTimeDlg *ui;
    QWidget *mPtrContainer;

    QMessageBox *mpPromptMsgBox;

};

#endif // SETDATETIMEDLG_H
