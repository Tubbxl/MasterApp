#ifndef CHECKSTATUSTHREAD_H
#define CHECKSTATUSTHREAD_H

#include <QThread>

#include "mfile.h"

class CheckStatusThread : public QThread
{
    Q_OBJECT
public:
    explicit CheckStatusThread(QObject *parent = 0);
    ~CheckStatusThread();
    void refreshPromptAndExit(QString strPrompt);

    int sync32550DateTime();

    void initShellScripts();

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

    void update32550OsPromptSignal();
    void refreshMainUISignal();

private:

    MFile *mMFile;

};

#endif // CHECKSTATUSTHREAD_H
