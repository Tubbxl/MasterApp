#include "checkstatusthread.h"

#include "mappdefines.h"

#include <stdio.h>

extern "C" {
 #include "include/dll_api.h"
#include "include/SSLComm.h"
}

CheckStatusThread::CheckStatusThread(QObject *parent) :
    QThread(parent)
{
    mMFile = new MFile();
}


CheckStatusThread::~CheckStatusThread()
{

}

void CheckStatusThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}

int CheckStatusThread::sync32550DateTime()
{
    int iRet = -1;
    unsigned char byDateTime[6] = {0};
    iRet = DLL_SysGetDateTime(byDateTime);
    qDebug("DLL_SysGetDateTime iRet: [%d]", iRet);
    if (0 != iRet)
        return iRet;

    char szDate[24] = {0};
    sprintf(szDate, "date -s 20%02x-%02x-%02x", byDateTime[0], byDateTime[1], byDateTime[2]);
    qDebug("szDate:[%s]", szDate);
    qDebug("set szDate iRet: [%d]", iRet);
    iRet = system(szDate);
    if (0 != iRet)
        return iRet;

    char szTime[24] = {0};
    sprintf(szTime, "date -s %02x:%02x:%02x", byDateTime[3], byDateTime[4], byDateTime[5]);
    qDebug("szTime:[%s]", szTime);
    iRet = system(szTime);

    qDebug("set szTime iRet: [%d]", iRet);
    return iRet;
}

void  CheckStatusThread::initShellScripts()
{
    system("chmod 700 /bin/nandwrite");
    system("chmod 700 /bin/kobs-ng");
    system("chmod 700 /bin/flash_erase");
    system("chmod 700 /sbin/pppd");
    system("chmod 700 /sbin/iw*");
    system("chmod 700 /sbin/chat");
    system("chmod 700 /sbin/ifconfig");
    system("chmod 700 /usr/bin/ts_*");
     system("chmod 700 /usr/sbin/wpa_*");
      system("chmod 700 /etc/services");

}


void CheckStatusThread::run()
{
   int schipStat = 0;

    refreshPromptSignal("Master app booting, please waiting...");

    initShellScripts();



     schipStat = DLL_PciCheckSecurityChipStatus();

     if(schipStat < 0)
     {
         refreshPromptSignal("Get 32550 status failed!");
         return;
     }

      if (SCHIP_HAVE_KEY == (schipStat & SCHIP_HAVE_KEY))
      {
          int iRet = -1;
          //sign verify
          unsigned char signBuf[SIGN_DATA_BYTE_LEN+1] = {0};
           char szFilePath[256] = {0};
          strcpy(szFilePath, "/home/root/masterapp/MasterApp");
           iRet = mMFile->getFileSignData(szFilePath, signBuf);
           if(0 != iRet)
           {
                refreshPromptSignal("Get masterapp sign data failed!");
               return ;
           }

           iRet = DLL_PciVerifySignature(0x00, signBuf);
           if(0 != iRet)
           {
                refreshPromptSignal("Master app sign verify failed!");
               return ;
           }
      }

     qDebug("DLL_PciCheckSecurityChipStatus schipStat:[%d]", schipStat);

     if (SCHIP_HAVE_APP == (schipStat & SCHIP_HAVE_APP)) {

         int iRet = -1;
         iRet = DLL_PciJumpToOs();
         qDebug("DLL_PciJumpToOs iRet:[%d]", iRet);
//         if(0 != iRet)
//         {
//             refreshPromptSignal("Start 32550 os failed!");
//             return;
//         }

         sleep(2);

        sync32550DateTime();

     }

     if (SCHIP_HAVE_KEY != (schipStat & SCHIP_HAVE_KEY)) {

         gb_RunMode = FACTORY_MODE;

        DLL_DelOpensslCertFilesDir();

     }

     if ((SCHIP_HAVE_KEY == (schipStat & SCHIP_HAVE_KEY))
             && (SCHIP_HAVE_APP == (schipStat & SCHIP_HAVE_APP))
             && (SCHIP_CHANGED_ADMINA_PWD ==  (schipStat & SCHIP_CHANGED_ADMINA_PWD))
             && (SCHIP_CHANGED_ADMINB_PWD == (schipStat & SCHIP_CHANGED_ADMINB_PWD))) {

          gb_RunMode = RELEASE_MODE;
     }

  //   gb_RunMode = RELEASE_MODE;

    closePromptSignal();


     if (SCHIP_HAVE_APP !=  (schipStat & SCHIP_HAVE_APP))
     {
           update32550OsPromptSignal();
     }
     else
     {
         qDebug("refreshMainUISignal");

         refreshMainUISignal();
     }

}

