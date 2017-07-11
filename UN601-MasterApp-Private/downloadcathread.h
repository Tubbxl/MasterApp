#ifndef DOWNLOADCATHREAD_H
#define DOWNLOADCATHREAD_H

#include <QThread>

class DownloadCAThread : public QThread
{
    Q_OBJECT
public:
    explicit DownloadCAThread(QObject *parent = 0);
    ~DownloadCAThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
    void closePromptSignal();

private slots:

    void downloadKeysTimeout();

private:

    bool mbExitCaCertsDownload;
};

#endif // DOWNLOADCATHREAD_H
