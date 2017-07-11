#include "downloadfmsignkey.h"
#include "mappdefines.h"

extern "C" {
#include "include/SSLComm.h"
 #include "include/dll_api.h"
}

#include "mappdefines.h"

#include <QTimer>
#include <stdio.h>


DownloadFmSignKey::DownloadFmSignKey(QObject *parent) :
    QThread(parent)
{
        mpSSLCommPacket = new SSLCommPacket();
        mbExitSSLConnect = false;
         QTimer::singleShot( DOWNLOAD_KEYS_TIME_OUT, this, SLOT(downloadKeysTimeout()) );
}

DownloadFmSignKey::~DownloadFmSignKey()
{

}

void DownloadFmSignKey::downloadKeysTimeout()
{
   mbExitSSLConnect = true;
}

void DownloadFmSignKey::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void DownloadFmSignKey::run()
{
    int iRet = -1;

     refreshPromptSignal("Please connect Sign-Trsm to download firmware sign key");

    mpSSLCommPacket->sslInit();

    qDebug("DownloadFmSignKey---sslInit");

    msleep(500);

    unsigned char pbyRecv[1024] = {0};
    unsigned char pbySend[1024] = {0};
    short wPackLen = 0;

    qDebug("DownloadFmSignKey---ready sslConnect");

    iRet = mpSSLCommPacket->sslConnect(CA_TYPE_VENDOR,
            FUNC_TYPE_SIGN_FIRMWARE);
    qDebug("DownloadFmSignKey---Connect:[%d]", iRet);

    if (mbExitSSLConnect ||
            (0 != iRet))
    {
        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Connect Sign-trsm failed");
        return;
    }

    iRet = DLL_SysSoftReset(0);

    qDebug("DownloadFmSignKey---DLL_SysSoftReset:[%d]", iRet);

     msleep(2000);

    iRet = mpSSLCommPacket->crc16RecvPacket(pbyRecv, &wPackLen, 5000);
    qDebug("DownloadFmSignKey---Crc16RecvPacket:[%d]", iRet);
    if (0 == iRet) {

        if (pbyRecv[1] ==  0xb3 && pbyRecv[2] == 0x04) {

             unsigned char  fwrType = pbyRecv[5];
             unsigned char  keyData[264] = {0};
             unsigned char  signData[256] = {0};

             memcpy(keyData, &pbyRecv[5+1], 264);
             memcpy(signData, &pbyRecv[5+1+264], 256);


            for (int i = 0; i < 5; i++) {
                    iRet = DLL_PciWriteSignKey(fwrType,
                            keyData, signData);
                    if (-1002 != iRet)
                        break;

                msleep(50);
            }

           qDebug("DownloadFmSignKey---DLL_PciWriteSignKey iRet: [%d]", iRet);

            pbySend[0] = iRet/256;
            pbySend[1] = iRet%256;
           wPackLen = 2;

           qDebug("DownloadFmSignKey---crc16SendPacket");

            mpSSLCommPacket->crc16SendPacket(pbySend, wPackLen,
                   0xb3, 0x05);


            if (0 == iRet) {

                gb_RunMode = RELEASE_MODE;

      //           gb_RefreshMainUI = true;

                  int schipStat = DLL_PciCheckSecurityChipStatus();
                  if ((schipStat > 0) &&(SCHIP_HAVE_APP == (schipStat & SCHIP_HAVE_APP))) {
                      DLL_PciJumpToOs();
                  }

                  if (NULL != mpSSLCommPacket)
                      mpSSLCommPacket->sslFree();

                refreshPromptAndExit("Download Firmware sign key success");

                closeSettingDlgSignal();
                return;

            } else {
                refreshPromptSignal("Download Firmware sign key failed");
            }

            if (NULL != mpSSLCommPacket)
                mpSSLCommPacket->sslFree();

             sleep(2);
            closePromptSignal();

        }
    } else {

        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Recv Sign-trsm data failed");
    }

}

