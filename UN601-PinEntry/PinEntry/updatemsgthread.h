#ifndef UPDATEMSGTHREAD_H
#define UPDATEMSGTHREAD_H

#include <QThread>

class UpdateMsgThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateMsgThread(QObject *parent = 0);
    ~UpdateMsgThread();
    void  server(int readfd);//, int writefd) ;

protected:
    void run();

signals:
    void refreshPromptSignal(int itemid, QString strPrompt);
    void itemCtrlSignal(int mode,int count);
    void showPinEntrySignal(int mode);
    void closePromptSignal();

};

#endif // UPDATEMSGTHREAD_H
