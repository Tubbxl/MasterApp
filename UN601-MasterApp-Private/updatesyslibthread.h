#ifndef UPDATESYSLIBTHREAD_H
#define UPDATESYSLIBTHREAD_H

#include <QThread>

class UpdateSysLibThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateSysLibThread(QString strFilePath, QObject *parent = 0);
    ~UpdateSysLibThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

private:
    QString mFilePath;
};

#endif // UPDATESYSLIBTHREAD_H
