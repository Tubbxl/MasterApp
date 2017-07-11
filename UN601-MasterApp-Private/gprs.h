#ifndef GPRS_H
#define GPRS_H

#include <QWidget>
#include <QMessageBox>
#include "gprsconnect.h"
namespace Ui {
class gprs;
}

class gprs : public QWidget
{
    Q_OBJECT

public:
    explicit gprs(QWidget *parent = 0);
    ~gprs();

signals:
    void sendsingals(QString *APN,QString *username,QString *passwd);
    void return_network();
    void successful(int ret);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void deal(int ret);

    void uiTimeout();
private:
    Ui::gprs *ui;
    QString apn;
    QString name;
    QString pswd;
    QString tel;
    gprsconnect *th;
    QMessageBox *mpPromptMsgBox;
};

#endif // GPRS_H
