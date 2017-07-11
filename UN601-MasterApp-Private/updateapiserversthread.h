#ifndef UPDATEAPISERVERSTHREAD_H
#define UPDATEAPISERVERSTHREAD_H

#include <QThread>
#include <mfile.h>
class UpdateApiserversThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateApiserversThread(QString strFilePath, QObject *parent = 0);
    ~UpdateApiserversThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

private:
    QString mFilePath;

     MFile *mMFile;
};

#endif // UPDATEAPISERVERSTHREAD_H
