#include "common.h"
#include "ui_common.h"
#include "settingmenu.h"
#include <QThread>
#include <QFile>
#include <QDebug>
extern "C" {
#include "dll_api.h"
}
common::common(const QString *type,QObject *parent) :
    QThread(parent)
    //ui(new Ui::common)
{
    TYPE = *type;
    mIsRun = false;
   //mtimer = new QTimer(this);
  // connect(mtimer,SIGNAL(timeout()),this,SLOT(InitView()));
  // mtimer->start(500);
   if(TYPE==FUNC_TYPE_LED_TEST)
   {
       sendMessage("LED Tese Start");
   }
   else if(TYPE==FUNC_TYPE_BEEP_TEST)
   {
       sendMessage("Beep Tese Start");
   }
   else if(TYPE==FUNC_TYPE_BACKLIGHT_TEST)
   {
       sendMessage("Backlight Tese Start");
   }
   else if(TYPE==FUNC_TYPE_DATE_TEST)
   {
       sendMessage("Date/Time Tese Start");
   }
   else if(TYPE==FUNC_TYPE_DDR_TEST)
   {
       sendMessage("DDR INFO :");
   }
   //InitView();
}

common::~common()
{
    //delete ui;
}

void common::InitView()
{

    if(TYPE==FUNC_TYPE_LED_TEST)
    {
        mtimer->stop();
        ledtest();
        return;
    }
    else if(TYPE==FUNC_TYPE_BEEP_TEST)
    {
        mtimer->stop();
        beeptest();
        return;
    }
    else if(TYPE==FUNC_TYPE_BACKLIGHT_TEST)
    {
        int i;
        char str[100]={0};
        mtimer->stop();
        for(i=0;i<8;i++)
        {
            memset(str,0x00,sizeof(str));
             sprintf(str,"echo %d > /sys/class/backlight/backlight/brightness ",i);
             qDebug()<<str;
             system(str);
             QThread::msleep(500);
        }
        sendMessage("The Lcd Backlight Test Finish!!!");
        mIsRun = true;
        return;
    }
    else if(TYPE==FUNC_TYPE_DATE_TEST)
    {
        mtimer->stop();
        refreshDisplayDateTime();
        mtimer->start(1000);
        return;
    }
    else if(TYPE==FUNC_TYPE_DDR_TEST)
    {
        mtimer->stop();
        getDDRSize();
        return;
    }
}

void common::ledtest()
{
    int i=0;
    QString str;
    if(mIsRun == true)
    {
        //return ;
    }
    for(i =0 ; i < 4; i++)
    {
        str.clear();
        str.sprintf("LED%d open",i);
        sendMessage(str);
        DLL_SysLedControl(i,1);
        QThread::sleep(1);
    }
    DLL_SysLedControl(0,0);
    DLL_SysLedControl(1,0);
    DLL_SysLedControl(2,0);
    DLL_SysLedControl(3,0);
    sendMessage("Close All Led");
    mIsRun = true;
}



void common::getDDRSize()
{

    char buf[100]={0};
    FILE *fp;
    int i = 0;
    QString str;
    if(mIsRun == true);
    {
        qDebug("getDDRSize return");
        //return ;
    }
    system("cat /proc/meminfo > /tmp/disksize.dat");
    fp=fopen("/tmp/disksize.dat","r");
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp,"%s",buf)!=EOF)
    {
       // qDebug(buf);
        if(buf[0]<0x40&&buf[0]>0x29)
        {
            if(i == 0)
            {
                str.clear();
                str.sprintf("Total Size : %s Kb",buf);
                sendMessage(str);
                i++;
            }
           else if(i == 1)
            {
                str.clear();
                str.sprintf("Used Size : %s Kb",buf);
                sendMessage(str);
                i++;
            }
            else if(i== 2)
            {
                str.clear();
                str.sprintf("Available Size : %s Kb",buf);
                sendMessage(str);
                i++;
            }
            if(i>3)
                break;
        }
    }
    fclose(fp);
    //sprintf(buf,"rm /tmp/disksize.dat");
    system("rm /tmp/disksize.dat");
    mIsRun = true;
}

void common::beeptest()
{
    int i = 0;
    if(mIsRun == true)
    {
         qDebug("beeptest return");
       // return ;
    }
    for(i = 0 ; i < 7; i++)
    {
        DLL_SysBeef(i, 200);
        QThread::msleep(500);
    }
    mIsRun = true;
}

void common::backlight()
{
    int i;
    char str[100]={0};
    if(mIsRun == true)
    {
         qDebug("backlight return");
        //return;
    }
    for(i=0;i<8;i++)
    {
        memset(str,0x00,sizeof(str));
         sprintf(str,"echo %d > /sys/class/backlight/backlight/brightness ",i);
         qDebug()<<str;
         system(str);
         QThread::msleep(500);
    }
    sendMessage("The Lcd Backlight Test Finish!!!");
    mIsRun = true;
}


void common::refreshDisplayDateTime()
{
    char szDate[24] = {0};
     char szTime[24] = {0};
     QString str;
     if(mIsRun == true)
     {
         qDebug("refreshDisplayDateTime return");
         //return ;
     }
    getCurrDateTime(szDate, szTime);
    str.clear();
    str.sprintf("The Date is: %s \n",szDate);
    sendMessage(str);
    str.clear();
    str.sprintf("The Date is: %s \n",szTime);
    sendMessage(str);
    //sendMessage("The Date is:");
  //  sendMessage(QString(QLatin1String(szDate)));
    // sendMessage("The Time is:");
    //sendMessage(QString(QLatin1String(szTime)));
    mIsRun = true;
}

int common::getCurrDateTime(char *pszDate, char *pszTime)
{
    time_t timep;
    struct tm *p;

    time(&timep);
    p = localtime(&timep);

    sprintf(pszDate, "%04d-%02d-%02d", (1900 + p->tm_year), ( 1 + p->tm_mon), p->tm_mday);

    sprintf(pszTime, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
    return 0;
}


