#ifndef DOWNLOADMKEY_H
#define DOWNLOADMKEY_H

#include "sslcommpacket.h"
#include "appnofile.h"
#include "updatesubapps.h"

#include <QThread>



class DownloadMKey : public QThread
{
    Q_OBJECT
public:
    explicit DownloadMKey(QObject *parent = 0);
    ~DownloadMKey();
    void refreshPromptAndExit(QString strPrompt);

    int  hex2str(unsigned char *in, int inlen,  char *out);

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
    bool mbExitDownloadMkeyThread;

    AppNoFile *mpAppNoFile;
    UpdateSubApps *mpUpdateSubApps;
};

#endif // DOWNLOADMKEY_H
