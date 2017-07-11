#include "setdatetimedlg.h"
#include "ui_setdatetimedlg.h"

#include "mappdefines.h"

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <QTime>

extern "C" {
 #include "include/dll_api.h"
}

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

SetDateTimeDlg::SetDateTimeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDateTimeDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void SetDateTimeDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void SetDateTimeDlg::initViews()
{
    mpPromptMsgBox = new QMessageBox(this);
     mpPromptMsgBox->setWindowTitle(tr("Prompt"));
    mpPromptMsgBox->setStyleSheet("font: 14pt;background-color:rgb( 0,220, 0)");
     mpPromptMsgBox->setIcon(QMessageBox::Information);
     mpPromptMsgBox->setStandardButtons(0);
     mpPromptMsgBox->setModal(false);

    refreshDisplayDateTime();

     loadStyleSheet("uistyle1");
}

void SetDateTimeDlg::initData()
{
     QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );
}

void SetDateTimeDlg::uiTimeout()
{
    on_btnBack_clicked();
}

void SetDateTimeDlg::refreshDisplayDateTime()
{
    char szDate[24] = {0};
      char szTime[24] = {0};

    getCurrDateTime(szDate, szTime);

    ui->labelDate->setText(QString(QLatin1String(szDate)));
    ui->labelTime->setText(QString(QLatin1String(szTime)));
}

void SetDateTimeDlg::updatePromptMsg(QString strPrompt)
{
     if(NULL != mpPromptMsgBox)
     {
         mpPromptMsgBox->setText(strPrompt);
         mpPromptMsgBox->move(120, 230);
         mpPromptMsgBox->setFixedSize(200, 80);
          mpPromptMsgBox->show();

          QTimer::singleShot(1000,this, SLOT(closePromptMsg()) );
     }
}

void SetDateTimeDlg::closePromptMsg()
{
     if(NULL != mpPromptMsgBox)
     {
          mpPromptMsgBox->hide();
          mpPromptMsgBox->close();
     }
}


 void SetDateTimeDlg::setContainerPointer(QWidget* ptrContainer)
 {
    mPtrContainer = ptrContainer;
 }

SetDateTimeDlg::~SetDateTimeDlg()
{
    delete ui;
}

int SetDateTimeDlg::getCurrDateTime(char* pszDate,  char* pszTime)
{
    time_t timep;
    struct tm *p;

    time(&timep);
    p = localtime(&timep);

    sprintf(pszDate, "%04d-%02d-%02d", (1900 + p->tm_year), ( 1 + p->tm_mon), p->tm_mday);

    sprintf(pszTime, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
    return 0;
}

int SetDateTimeDlg::setDateTime(char* pszDate,  char* pszTime)
{
    struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
    int ret = 0;

    sscanf(pszDate, "%04d%02d%02d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday);
     sscanf(pszTime, "%02d%02d%02d",  &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
    time_tm.tm_year -= 1900;
    time_tm.tm_mon -= 1;
    time_tm.tm_wday = 0;
    time_tm.tm_yday = 0;
    time_tm.tm_isdst = 0;

    timep = mktime(&time_tm);
    time_tv.tv_sec = timep;
    time_tv.tv_usec = 0;

    ret = settimeofday(&time_tv, NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}


void SetDateTimeDlg::on_btnBack_clicked()
{
      mPtrContainer->close();
}

void SetDateTimeDlg::stringToHex(unsigned char *pbyInData, int iInLen, unsigned char *pbyOut)
{
    int i;

    if (0 != (iInLen%2))
    {
        pbyInData[iInLen] = 0;
        iInLen++;
    }
    for (i=0; i<iInLen; i+=2)
    {
        if ((pbyInData[i]>='A') && (pbyInData[i]<='F'))
        {
            pbyOut[i/2] = (unsigned char)((pbyInData[i]-'A'+10)*16);
        }
        else if ((pbyInData[i]>='a') && (pbyInData[i]<='f'))
        {
            pbyOut[i/2] = (unsigned char)((pbyInData[i]-'a'+10)*16);
        }
        else
        {
            pbyOut[i/2] = (unsigned char)((pbyInData[i]-'0')*16);
        }

        if ((pbyInData[i+1]>='A') && (pbyInData[i+1]<='F'))
        {
            pbyOut[i/2] |= (unsigned char)((pbyInData[i+1]-'A'+10)&0x0f);
        }
        else if ((pbyInData[i+1]>='a') && (pbyInData[i+1]<='f'))
        {
            pbyOut[i/2] |= (unsigned char)((pbyInData[i+1]-'a'+10)&0x0f);
        }
        else
        {
            pbyOut[i/2] |= (unsigned char)((pbyInData[i+1]-'0')&0x0f);
        }
    }
}

void SetDateTimeDlg::on_btnConfirm_clicked()
{
    QString strDate = ui->editDate->text();
    QString strTime = ui->editTime->text();

    int iRet = -1;
   iRet = setDateTime(strDate.toLocal8Bit().data(),  strTime.toLocal8Bit().data());
   if(0 == iRet)
   {
       QByteArray arrDate = strDate.toLatin1();
       char *pszDate = arrDate.data();

       QByteArray arrTime = strTime.toLatin1();
       char *pszTime = arrTime.data();

       unsigned char szDateTime[16] = {0};
       unsigned char ucDateTime[6] = {0};

       memcpy(szDateTime, &pszDate[2], 6);
       memcpy(&szDateTime[6], pszTime, 6);

      stringToHex(szDateTime, 12, ucDateTime);

       iRet = DLL_SysSetDateTime(ucDateTime);
       if(0 != iRet)
       {
           updatePromptMsg("Sync 32550 time failed");
       }
       else
       {
           updatePromptMsg("Modify date and time succeeded");
       }

       refreshDisplayDateTime();
   }
   else
   {
       updatePromptMsg("Modify date and time failed");
   }





}
