#ifndef DOWNLOADAPPSIGNKEY_H
#define DOWNLOADAPPSIGNKEY_H

#include "sslcommpacket.h"
#include <QThread>

class DownloadAppSignKey : public QThread
{
    Q_OBJECT
public:
    explicit DownloadAppSignKey(QObject *parent = 0);
    ~DownloadAppSignKey();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
    void closePromptSignal();

private slots:

    void downloadKeysTimeout();

private:

    SSLCommPacket *mpSSLCommPacket;

    bool mbExitSSLConnect;
};

#endif // DOWNLOADAPPSIGNKEY_H
