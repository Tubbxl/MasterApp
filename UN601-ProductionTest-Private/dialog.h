#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QThread>

namespace Ui {
class Dialog;
}

class Dialog : public  QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const QString *type);
    Dialog(const QString *type, int mode);
    ~Dialog();
    void loadStyleSheet(const QString &sheetName);
public:
    void sleep(int msec);
    QWidget *mPtrContainer;
    void setContainerPointer(QWidget* ptrContainer);


    void  ICC_test();
    void Sys_test();
    void Picctest();
    void MSRtest();
    void printertest();
    void scanTest();
    void writeSn();
    void PCI_Test();
    void hideAllCtl();

private slots:
    void on_btnICC_clicked();

    void on_btnSys_clicked();

    void on_btnPicc_clicked();

    void on_btnMSR_clicked();

    void on_btnPrinter_clicked();

    void on_btnScan_clicked();

    void receiveMessage(QString msg);//槽函数

    void on_btnPCI_clicked();

    //void on_btnKeyPad_clicked();

    void on_btnSn_clicked();

    void on_btnExit_clicked();

    void on_fail_clicked();

    void on_pass_clicked();

    void InitView();

signals:
    void sendMessage(QString msg);//信号函数只有声明，不需要写定义
    void return_test_result(int ret);
private:
    Ui::Dialog *ui;
    QString TYPE;
    QTimer *mtimer;
};

#endif // DIALOG_H
