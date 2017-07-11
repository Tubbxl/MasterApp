#include "mfile.h"
#include "mappdefines.h"

#include <QFile>
#include <QDir>


#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

MFile::MFile()
{

}

MFile::~MFile()
{

}

bool MFile::copyFile(QString srcFile ,QString destFile)
{
    destFile.replace("\\","/");
    if (srcFile == destFile){
        return true;
    }

    if (!QFile::exists(srcFile)){
        return false;
    }

    QDir *createfile     = new QDir;
    bool exist = createfile->exists(destFile);
    if (exist)
    {
        createfile->remove(destFile);
    }//end if

    if(!QFile::copy(srcFile, destFile))
    {
        return false;
    }
    return true;
}

bool MFile::copyDir(QString srcDir, QString destDir)
{
    QDir sourceDir(srcDir);
    QDir targetDir(destDir);
    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyFile(fileInfo.filePath(),
                         targetDir.filePath(fileInfo.fileName())))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}

//递归遍历文件夹
QFileInfoList MFile::getFileList(QString path)
{
    QDir dir(path);

    //列出dir(path)目录文件下所有文件和目录信息，存储到file_list容器
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    //列出dir(path)目录下所有子文件夹
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    //进行子文件夹folder_list递归遍历，将内容存入file_list容器
    for(int i= 0; i != folder_list.size(); i++)
    {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = getFileList(name);
        file_list.append(child_file_list);
    }
    return file_list;
}

QFileInfoList MFile::getSpecifiedFileTypeList(QString path, int fileType)
{
    QFileInfoList fileList;
    QString filter;

    //遍历文件夹获取文件信息，存入到容器file_info_list
    QFileInfoList file_info_list = getFileList(path);     //使用File相对路径
     qDebug("getFileList---file_info_list size:[%d]", file_info_list.size());
    if(file_info_list.size() <= 0)
        return fileList;


    //foreach遍历容器file_in_list，进行过滤筛选，将过滤结果输入到fileList容器
    //筛选PPT文件
    /*foreach(variable, container)是Qt定义的一个宏，
         它的作用是遍历QList容器类变量container，并依次把列表项存放在变量variable中。
       */
    QString fileName ;
    QString prefixFileName;

    if(FILE_TYPE_BOOT == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
            filter = fileinfo.suffix();    //后缀名
            if(filter == FILE_TYPE_FIRMWARE_SUFFIX )
            {
                fileName = fileinfo.fileName();
                prefixFileName = fileName.left(FILE_TYPE_BOOT_PREFIX.length());
                if(0 == QString::compare(FILE_TYPE_BOOT_PREFIX, prefixFileName))
                {
                    fileList.append(fileinfo);
                }
            }
        }
    }
    else if(FILE_TYPE_KERNEL == fileType)
    {

        foreach(QFileInfo fileinfo, file_info_list)
        {
            filter = fileinfo.suffix();    //后缀名

            qDebug("filter = %s",qPrintable(filter));

            if(filter == FILE_TYPE_FIRMWARE_SUFFIX )
            {

                fileName = fileinfo.fileName();
                prefixFileName = fileName.left(FILE_TYPE_KERNEL_PREFIX.length());

                   qDebug("fileName = %s",qPrintable(fileName));

                if(0 == QString::compare(FILE_TYPE_KERNEL_PREFIX, prefixFileName))
                {

                    fileList.append(fileinfo);
                }
            }
        }
    }
    else if(FILE_TYPE_SYSTEM_LIB == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
            filter = fileinfo.suffix();    //后缀名
            if((filter == FILE_TYPE_SYSTEM_LIB_SUFFIX))
            {
                fileList.append(fileinfo);
            }
        }
    }
    else if(FILE_TYPE_32550_OS == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
            filter = fileinfo.suffix();    //后缀名
            if((filter == FILE_TYPE_32550_OS_SUFFIX))
            {
                fileList.append(fileinfo);
            }
        }
    }
    else if(FILE_TYPE_MASTER_APP == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
                fileName = fileinfo.fileName();
                prefixFileName = fileName.left(FILE_TYPE_MASTER_APP_PREFIX.length());

                   qDebug("fileName = %s",qPrintable(fileName));

                if(0 == QString::compare(FILE_TYPE_MASTER_APP_PREFIX, prefixFileName))
                {

                    fileList.append(fileinfo);
                }
        }
    }
    else if(FILE_TYPE_API_SERVER == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
                fileName = fileinfo.fileName();
                prefixFileName = fileName.left(FILE_TYPE_API_SERVER_PREFIX.length());

                   qDebug("fileName = %s",qPrintable(fileName));

                if(0 == QString::compare(FILE_TYPE_API_SERVER_PREFIX, prefixFileName))
                {

                    fileList.append(fileinfo);
                }
        }
    }
    else if(FILE_TYPE_SUB_APP == fileType)
    {
        foreach(QFileInfo fileinfo, file_info_list)
        {
            filter = fileinfo.suffix();    //后缀名
            if(0 == filter.length())
            {
                fileList.append(fileinfo);
            }
        }
    }

    return fileList;

}

unsigned long MFile::getFileSize(const char *filePath)
{
    unsigned long fileSize = -1;
    struct stat statBuff;
    if(stat(filePath, &statBuff) < 0){
        return fileSize;
    }else{
        fileSize = statBuff.st_size;
    }
    return fileSize;

}

void* MFile::getFilename(char* pathName)
{
    char* fileName, *pos;
    int nameLen;
    nameLen = strlen(pathName);
    pos = pathName + nameLen;
    while(*pos != '/' && pos != pathName)
        pos --;

    if(pos == pathName)
    {
        fileName = pathName+1;
        return fileName;
    }

    nameLen = nameLen-(pos-pathName);
    fileName = (char*) malloc(nameLen+1);
    memcpy(fileName,pos+1,nameLen);
    return fileName;
}

int MFile::getFileSignData(char* filePath, unsigned char * signBuf)
{
    unsigned char tempBuf[SIGN_DATA_BYTE_LEN+1]={0};

    int fileSize = -1;
    fileSize = getFileSize(filePath);
    if(fileSize < SIGN_DATA_BYTE_LEN)
        return -1;

    int fd = open(filePath,O_RDONLY);
    if(fd<0) return -2;

    lseek(fd,(fileSize-SIGN_DATA_BYTE_LEN),SEEK_SET);


    if(SIGN_DATA_BYTE_LEN != read(fd,tempBuf,SIGN_DATA_BYTE_LEN))
    {
        close(fd);
        return -3;
    }

    memcpy(signBuf, tempBuf,   SIGN_DATA_BYTE_LEN);

    return 0;
}

