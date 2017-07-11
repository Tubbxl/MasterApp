#ifndef OPTESTTHREAD_H
#define OPTESTTHREAD_H

#include "sslcommpacket.h"
#include <QThread>

class OPTestThread : public QThread
{
    Q_OBJECT
public:
    explicit OPTestThread(char* pszIP, int iPort, QObject *parent = 0);
    ~OPTestThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
    void closePromptSignal();

private:

    char mszIP[16];
    int miPort;

};

#endif // OPTESTTHREAD_H
