#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QTimer>
#include "serialthread.h"

namespace Ui {
class serial;
}

class serial : public QWidget
{
    Q_OBJECT

public:
    explicit serial(QWidget *parent = 0);
    ~serial();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void comInit();

private slots:
    void on_start_clicked();

    //void on_back_clicked();

    void on_pass_clicked();
    void displayRxDatacom1();
   // void displayRxDatacom2();
  //  void displayRxDatacom3();
    void senddata();

    void on_pushButton_clicked();

signals:
    void return_test_result(int ret);

private:
    Ui::serial *ui;
    QWidget *mPtrContainer;
    QTimer *mtimer;

 //   Serialthread mCom1;
//    Serialthread mCom2;
//    Serialthread mCom3;
    Serialthread mCom1;


    int flag;

};

#endif // SERIAL_H
