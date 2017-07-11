#ifndef UPDATE32550OSTHREAD_H
#define UPDATE32550OSTHREAD_H

#include <QThread>

#include "mfile.h"

//#include "loadapp.h"

class Update32550OsThread : public QThread
{
    Q_OBJECT
public:
    explicit Update32550OsThread(QString strFilePath, QObject *parent = 0);
    ~Update32550OsThread();
    void refreshPromptAndExit(QString strPrompt);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();

private:
    QString mFilePath;

     MFile *mMFile;
public:
  //   LoadApp *mpLoadApp;
};

#endif // UPDATE32550OSTHREAD_H
