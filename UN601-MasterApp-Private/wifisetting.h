#ifndef WIFISETTING_H
#define WIFISETTING_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include "passwdinput.h"
#include "wifiscan.h"
#include "wificonnectth.h"
#include "ping.h"
extern "C" {
#include "include//dll_api.h"
}
namespace Ui {
class WifiSet;
}

class Wifisetting : public QWidget
{
    Q_OBJECT

 signals:
    void cancel();
public:
    explicit Wifisetting(QWidget *parent = 0);
    ~Wifisetting();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);

private slots:
    void on_back_clicked();

    void on_OK_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

    void get_pswd(QString *temp);
    void get_network(T_WLANDEV *network, int *num);
   void isconnect(int ret);
   void get_ping_result(int ret);

   void on_Setting_clicked();

private:
    Ui::WifiSet *ui;
    QString  C_Mac;
    QString ip;
    QString bcast;
    QString mack;
    QString mac;
    int pingtestflag;
    ping *pingthread;

    passwdinput *pswd;
    QString SSID;
    int ssidindex;
    int worknum;
    QString WD;
    wifiscan *Th;
    T_WLANDEV *work;
    wificonnectth *wifict;

    QGraphicsScene *scene;
    QGraphicsProxyWidget *w ;
    QGraphicsView *view ;

    QWidget *mPtrContainer;

    QMessageBox *mpPromptMsgBox;
};

#endif // WIFI_H
