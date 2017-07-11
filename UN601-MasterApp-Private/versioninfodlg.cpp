#include "versioninfodlg.h"
#include "ui_versioninfodlg.h"

#include "sdcardupdatedlg.h"

#include "mappdefines.h"

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

extern "C" {
#include "include/dll_api.h"
}

#include <QVBoxLayout>

VersionInfoDlg::VersionInfoDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionInfoDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void VersionInfoDlg::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void VersionInfoDlg::initViews()
{
    qDebug("VersionInfoDlg---initViews");

    int iRet = -1;

    char szTotalVer[48] = {0};
    iRet = getTotalFmVer(szTotalVer);
    if(0 == iRet)
    {
        ui->labelTotal->setText(QString(QLatin1String(szTotalVer)));
    }

    char szBootVer[8] = {0};
    iRet = getSecurityChipBootVer(szBootVer);
    if(0 == iRet)
    {
        ui->labelBoot->setText(QString(QLatin1String(szBootVer)));
    }

    qDebug("pSchipBootVer:[%s]", szBootVer);

    char szOsVer[8] = {0};
     iRet = getSecurityChipOsVer(szOsVer);
     if(0 == iRet)
     {
         ui->labelOs->setText(QString(QLatin1String(szOsVer)));
     }
     qDebug("pSchipOsVer:[%s]", szOsVer);





//    setLayout(pVLayout);

    loadStyleSheet("uistyle1");
}

void VersionInfoDlg::initData()
{

}

int VersionInfoDlg::getSecurityChipBootVer(char * bootVer)
{
    unsigned char byVerBuf[8] = {0};
    int iRet = -1;

    iRet = DLL_SysGetVersion(byVerBuf);
      qDebug("DLL_SysGetVersion iRet:[%d]", iRet);
    if(0 != iRet)
        return iRet;

    char byBootVerBuf[6] = {0};

    byBootVerBuf[0] =  (byVerBuf[0] + 0x30);
    byBootVerBuf[1] = '.';
    byBootVerBuf[2] = (byVerBuf[1] + 0x30);
    byBootVerBuf[3] = '.';
    byBootVerBuf[4] = (byVerBuf[2] + 0x30);

    strcpy(bootVer, byBootVerBuf);

    qDebug("getSecurityChipBootVer:[%s]", byBootVerBuf);

    return 0;

}


int VersionInfoDlg::getSecurityChipOsVer(char * osVer)
{
    unsigned char byVerBuf[8] = {0};
    int iRet = -1;

    iRet = DLL_SysGetVersion(byVerBuf);
       qDebug("DLL_SysGetVersion iRet:[%d]", iRet);
    if(0 != iRet)
        return iRet;

    char byOsVerBuf[6] = {0};

    byOsVerBuf[0] = (byVerBuf[3] + 0x30);
    byOsVerBuf[1] = '.';
    byOsVerBuf[2] = (byVerBuf[4] + 0x30);
    byOsVerBuf[3] = '.';
    byOsVerBuf[4] = (byVerBuf[5] + 0x30);

    strcpy(osVer, byOsVerBuf);

     qDebug("getSecurityChipOsVer:[%s]", byOsVerBuf);

    return 0;
}

int VersionInfoDlg::getTotalFmVer(char * totalVer)
{
       int iRet = -1;
       char bootVer[8] = {0};
       iRet = getSecurityChipBootVer(bootVer);
       if(0 != iRet)
           return iRet;

       char osVer[8] = {0};
       iRet = getSecurityChipOsVer(osVer);
       if(0 != iRet)
           return iRet;

       char tempVer[48] = {0};

       strcpy(tempVer, bootVer);
       strcat(tempVer, "_");
       strcat(tempVer, osVer);
       strcat(tempVer, "_");

       QByteArray ba = UBOOT_VER.toLatin1();
       char *ubootVer = ba.data();

       strcat(tempVer, ubootVer);
       strcat(tempVer, "_");

        ba = MASTERAPP_VER.toLatin1();
       char *masterappVer = ba.data();

       strcat(tempVer, masterappVer);

       strcpy(totalVer, tempVer);

       return 0;

}

// unsigned char*  VersionInfoDlg::getLinuxKernelVer()
//{
//    unsigned char szLinuxKernelVer[12]= {0};

//    strcpy(szLinuxKernelVer, "4.1.15");

//    return szLinuxKernelVer;
//}

// unsigned char* VersionInfoDlg::getUbootVer()
// {
//     unsigned char szUbootVer[12]= {0};

//     strcpy(szUbootVer, "1.0.0");

//     return szUbootVer;
// }

// unsigned char* VersionInfoDlg::getMasterAppVer()
// {
//     unsigned char szMasterAppVer[12]= {0};

//     strcpy(szMasterAppVer, "1.0.0");

//     return szMasterAppVer;
// }

void VersionInfoDlg::setContainerPointer(QWidget* ptrContainer)
{
    mPtrContainer = ptrContainer;
}

VersionInfoDlg::~VersionInfoDlg()
{
    delete ui;
}

void VersionInfoDlg::on_btnBack_clicked()
{
    mPtrContainer->close();
}
