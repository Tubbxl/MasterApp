#ifndef DOWNLOADFMSIGNKEY_H
#define DOWNLOADFMSIGNKEY_H

#include "sslcommpacket.h"
#include <QThread>

class DownloadFmSignKey : public QThread
{
    Q_OBJECT
public:
    explicit DownloadFmSignKey(QObject *parent = 0);
    ~DownloadFmSignKey();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
    void closePromptSignal();
    void closeSettingDlgSignal();

private slots:

    void downloadKeysTimeout();

private:

    SSLCommPacket *mpSSLCommPacket;

    bool mbExitSSLConnect;
};

#endif // DOWNLOADFMSIGNKEY_H
