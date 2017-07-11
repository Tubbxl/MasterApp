#ifndef PASSWDINPUT_H
#define PASSWDINPUT_H

#include <QWidget>

namespace Ui {
class passwdinput;
}

class passwdinput : public QWidget
{
    Q_OBJECT

public:
    explicit passwdinput(QWidget *parent = 0);
    ~passwdinput();
    void setContainerPointer(QWidget* ptrContainer);
    void uiTimeout();

signals:
    void return_parent();
    void return_passwd(QString *);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::passwdinput *ui;
    QString *pswd;
     QWidget *mPtrContainer;

};

#endif // PASSWDINPUT_H
