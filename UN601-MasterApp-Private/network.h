#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "wifi.h"
#include "gprs.h"
#include "ethconnect.h"

#include "optestthread.h"

#include <QTimer>

namespace Ui {
class network;
}

class network : public QWidget
{
    Q_OBJECT

public:
    explicit network(QWidget *parent = 0);
    ~network();
    void setContainerPointer(QWidget* ptrContainer);

    void updatePromptMsg(QString strPrompt);
    void updatePromptMsgAndExit(QString strPrompt);


private slots:
    void on_comboBox_activated(const QString &arg1);

    void on_testButton_clicked();

    void handleRefreshPrompt(QString strPrompt);
    void closePromptMsg();

    void on_btnBack_clicked();

    void ConnectResult(int ret);

    void uiTimeout();
    void waitTimeout();

private:
    Ui::network *ui;
    wifi *wififram;
    gprs *gsm;
    ethconnect *ethcnt;
    int flag;
    QMessageBox *mpPromptMsgBox;

    QWidget *mPtrContainer;

//    QGraphicsScene *scene;
//    QGraphicsProxyWidget *w ;
//    QGraphicsView *view ;

    QGraphicsScene *scene1;
    QGraphicsProxyWidget *w1 ;
    QGraphicsView *view1 ;

    QGraphicsScene *scene2;
    QGraphicsProxyWidget *w2 ;
   QGraphicsView *view2 ;


   OPTestThread *mpOPTestThread;

   QTimer *mpQTimer;

   bool mbNetSetThreadExit;
   bool mbOPTestThreadExit;

};

#endif // NETWORK_H
