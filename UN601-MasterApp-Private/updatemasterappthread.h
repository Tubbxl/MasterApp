#ifndef UPDATEMASTERAPPTHREAD_H
#define UPDATEMASTERAPPTHREAD_H

#include <QThread>

#include "mfile.h"

class UpdateMasterAppThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateMasterAppThread(QString strFilePath, QObject *parent = 0);
    ~UpdateMasterAppThread();
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

#endif // UPDATEMASTERAPPTHREAD_H
