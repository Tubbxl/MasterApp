#ifndef UPDATEBOOTTHREAD_H
#define UPDATEBOOTTHREAD_H

#include <QThread>

class UpdateBootThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateBootThread(QString strFilePath, QObject *parent = 0);
    ~UpdateBootThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

private:
    QString mFilePath;
};

#endif // UPDATEBOOTTHREAD_H
