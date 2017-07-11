#ifndef ETHERNETSETTING_H
#define ETHERNETSETTING_H

#include "ping.h"
#include "ethconnect.h"

#include <QWidget>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
extern "C" {
#include "include/dll_api.h"
}
namespace Ui {
class EthernetSet;
}

class Ethernetsetting : public QWidget
{
    Q_OBJECT

public:
    explicit Ethernetsetting(QWidget *parent = 0);
    ~Ethernetsetting();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
signals:
     void Back();

private slots:
    void on_OK_clicked();

    void on_back_clicked();

    void ping_result(int ret);
    void Deal(int ret);
    void on_pushButton_clicked();

private:
    Ui::EthernetSet *ui;
    QString  C_Mac;
    QString ip;
    QString bcast;
    QString mack;
    QString mac;
    int flag;
    ethconnect *Ethcont;

    ping *ethpingthread;

    QWidget *mPtrContainer;

    QMessageBox *mpPromptMsgBox;
};

#endif // ETHERNET_H
