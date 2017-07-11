#include "displayapplist.h"
#include "ui_displayapplist.h"

#include "mappdefines.h"

#include <QFile>
#include <QDir>

#include <QtGui>

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

DisplayAppList::DisplayAppList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayAppList)
{
    ui->setupUi(this);

    initViews();
    initData();
}

void DisplayAppList::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void DisplayAppList::initViews()
{

    ui->listApps->setFocusPolicy(Qt::NoFocus);
     ui->listApps->resize(220, 200);

    ui->listApps->clear();

     loadStyleSheet("uistyle1");
}

void DisplayAppList::initData()
{
    char szVerInfo[8] = {0};
    char szAppName[32] = {0};

      mpUpdateSubApps->loadAppsList(mAppsList);

    for(int i = 0; i < SUB_APP_MAX_NUMS; i++)
    {
           if(mAppsList[i].length() <=0)
               break;

           QByteArray arrAppName = mAppsList[i].toLatin1();
           memset(szAppName, 0x00, sizeof(szAppName));
            strcpy(szAppName, arrAppName.data());

             getAppVerInfo(szAppName, szVerInfo);

           ui->listApps->addItem(mAppsList[i] + " " +szVerInfo);
    }


}

int DisplayAppList::getAppVerInfo( char * ptrAppName,  char *ptrAppVer)
{
     unsigned char ucAppVer[VER_DATA_BYTE_LEN+1] = {0};

    int iRet = -1;
    iRet = mpUpdateSubApps->readAppfileVerInfo(ptrAppName, ucAppVer);
    qDebug("readAppfileVerInfo iRet:[%d]", iRet);
    if(0 != iRet)
        return iRet;

    qDebug("readAppfileVerInfo ucAppVer:[%02x][%02x][%02x]", ucAppVer[0], ucAppVer[1], ucAppVer[2]);

     char szAppVer[8] = {0};

    szAppVer[0] = 'v';
    szAppVer[1] = ucAppVer[0];
    szAppVer[2] = '.';
    szAppVer[3] = ucAppVer[1];
    szAppVer[4] = '.';
    szAppVer[5] = ucAppVer[2];

    strcpy(ptrAppVer, szAppVer);

     qDebug("readAppfileVerInfo szAppVer:[%s]", szAppVer);

    return 0;
}

 void DisplayAppList::setContainerPointer(QWidget* ptrContainer)
 {
    mPtrContainer = ptrContainer;
 }

DisplayAppList::~DisplayAppList()
{
    delete ui;
}

void DisplayAppList::on_btnBack_clicked()
{
    mPtrContainer->close();
}
