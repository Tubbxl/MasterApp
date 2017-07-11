#ifndef LOADAPP_H
#define LOADAPP_H

#include <QObject>
#include <QString>

class LoadApp : public QObject
{
    Q_OBJECT
public:
    LoadApp();
    ~LoadApp();

    int Dll_SysDownloadApp();
    int Dll_SysReboot();
    int HandShake();
    unsigned long get_file_size(const char *path);
    int DownloadFile(char* filePath);
    int DownloadApp(char* filePath);
    int Dll_SysRunApp();

signals:
    void refreshPromptSignal(QString strPrompt);

};

#endif // LOADAPP_H

