#ifndef UPDATEKERNELTHREAD_H
#define UPDATEKERNELTHREAD_H

#include <QThread>


class UpdateKernelThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateKernelThread(QString strFilePath, QObject *parent = 0);
    ~UpdateKernelThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

private:
    QString mFilePath;
};

#endif // UPDATEKERNELTHREAD_H
