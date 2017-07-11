#ifndef WIFI_H
#define WIFI_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include "passwdinput.h"
#include "setthread.h"
#include "wifiscan.h"
#include "wificonnectth.h"
extern "C" {
#include "dll_api.h"
}

namespace Ui {
class wifi;
}

class wifi : public QWidget
{
    Q_OBJECT

public:
    explicit wifi(QWidget *parent = 0);
    ~wifi();

private slots:
    void on_listWidget_clicked(const QModelIndex &index);

    void on_pushButton_clicked();
signals:
    void clos_wifi_win();
    void connectRet(int ret);
public slots:
    void get_pswd(QString *temp);
    void get_network(T_WLANDEV *network, int *num);
    void isconnect(int ret);
    void uiTimeout();
    //void sendinfo(unsigned char *id,unsigned char *mm);
private:
    Ui::wifi *ui;
    passwdinput *pswd;
    QString SSID;
    int ssidindex;
    int worknum;
    QString WD;
    wifiscan *Th;
    T_WLANDEV *work;
    wificonnectth *wifict;
     QMessageBox *mpPromptMsgBox;

    QGraphicsScene *scene;
    QGraphicsProxyWidget *w ;
    QGraphicsView *view ;
};

#endif // WIFI_H
