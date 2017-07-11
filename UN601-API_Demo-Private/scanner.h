#ifndef SCANNER_H
#define SCANNER_H

#include <QWidget>
#include "scanread.h"

namespace Ui {
class Scanner;
}

class Scanner : public QWidget
{
    Q_OBJECT

public:
    explicit Scanner(QWidget *parent = 0);
    ~Scanner();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);


private slots:
    void on_set_clicked();

    void on_set_2_clicked();

    void on_mode_clicked();

    void on_mode_2_clicked();

    void on_prefix_clicked();

    void on_suffix_clicked();

    void on_pushButton_clicked();

    void datedeal();
    void getdata(int *len, unsigned char*data);
    void on_pushButton_2_clicked();

    void on_setkey_clicked();

signals:
    void readend();
private:
    Ui::Scanner *ui;
     char mMode;
     char *mPrefix;
     char *mSuffix;
     char mDate[100];
     int mDatalen;

     QWidget *mPtrContainer;
     scanread *mReadThread;
};

#endif // SCANNER_H
