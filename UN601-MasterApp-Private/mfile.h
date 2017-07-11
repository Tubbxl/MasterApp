#ifndef MFILE_H
#define MFILE_H

#include <QString>
#include <QFileInfoList>


class MFile
{
public:
    MFile();
    ~MFile();
    bool copyFile(QString sourceDir ,QString toDir);
    bool copyDir(QString fromDir, QString toDir);
    QFileInfoList getFileList(QString path);
    QFileInfoList getSpecifiedFileTypeList(QString path, int fileType);

    unsigned long getFileSize(const char *filePath);
    void* getFilename(char* pathName);
    int getFileSignData(char* filePath, unsigned char * signBuf);
};

#endif // MFILE_H
