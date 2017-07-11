#ifndef UpdateSubAppThread_H
#define UpdateSubAppThread_H

#include <QThread>

#include "updatesubapps.h"
#include "apppermsfile.h"

#include "mfile.h"

class UpdateSubAppThread : public QThread
{
    Q_OBJECT
public:
    explicit UpdateSubAppThread(QString strFilePath, QObject *parent = 0);
    ~UpdateSubAppThread();
    void refreshPromptAndExit(QString strPrompt);
  //  void* getFilename(unsigned char* pathName);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();


private:
    QString mFilePath;
    UpdateSubApps *mUpdateSubApps;
    AppPermsFile* mpAppPermsFile;
    MFile *mMFile;
};

#endif // UpdateSubAppThread_H
