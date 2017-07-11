#include "downloadappsignkey.h"
#include "mappdefines.h"

extern "C" {
#include "include/SSLComm.h"
 #include "include/dll_api.h"
}

#include <QTimer>
#include <stdio.h>


DownloadAppSignKey::DownloadAppSignKey(QObject *parent) :
    QThread(parent)
{
        mpSSLCommPacket = new SSLCommPacket();
        mbExitSSLConnect = false;
         QTimer::singleShot( DOWNLOAD_KEYS_TIME_OUT, this, SLOT(downloadKeysTimeout()) );
}

DownloadAppSignKey::~DownloadAppSignKey()
{

}

void DownloadAppSignKey::downloadKeysTimeout()
{
   mbExitSSLConnect = true;
}

void DownloadAppSignKey::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void DownloadAppSignKey::run()
{
    int iRet = -1;

    refreshPromptSignal("Please connect App-Trsm to download app sign key...");

    mpSSLCommPacket->sslInit();

    qDebug("DownloadAppSignKey---sslInit");

    msleep(500);

    qDebug("DownloadAppSignKey---ready sslConnect");

    iRet = mpSSLCommPacket->sslConnect(CA_TYPE_VENDOR,
                                       FUNC_TYPE_SIGN_APP);
    qDebug("DownloadAppSignKey---Connect:[%d]", iRet);

    if (mbExitSSLConnect ||
            (0 != iRet))
    {
        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("download App sign key failed(connect timeout)");
        return;
    }

    //download acquire ca
    refreshPromptSignal("Download acquire ca……");

    if (0 != DLL_TermSSLGetAquireCA())
    {
        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Download acquire ca failed!");
        return;
    }


    refreshPromptSignal("Apply master key crt from app-trsm……");


    if (0 != DLL_TermSSLUpdateAquireCACert(TERM_TYPE_MF2352,
                                          FUNC_TYPE_DOWNLOAD_MK))
    {
        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Apply master key crt failed");
        return;
    }
    else
    {
        refreshPromptSignal("Apply master key crt success");
    }

    unsigned char pbyRecv[1024] = {0};
    unsigned char pbySend[1024] = {0};
    short wPackLen = 0;

    iRet = mpSSLCommPacket->crc16RecvPacket(pbyRecv, &wPackLen, 5000);
    qDebug("DownloadAppSignKey---Crc16RecvPacket:[%d]", iRet);
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

            iRet = 0;

            qDebug("DownloadAppSignKey---DLL_PciWriteSignKey iRet: [%d]", iRet);

            pbySend[0] = iRet/256;
            pbySend[1] = iRet%256;
            wPackLen = 2;

            qDebug("DownloadAppSignKey---crc16SendPacket");

            mpSSLCommPacket->crc16SendPacket(pbySend, wPackLen,
                                             0xb3, 0x05);


            if (0 == iRet) {

                refreshPromptSignal("Download app sign key success");
            } else {
                refreshPromptSignal("Download app sign key failed");
            }

            if (NULL != mpSSLCommPacket)
                mpSSLCommPacket->sslFree();

            sleep(2);
            closePromptSignal();

        }
    } else {

        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Recv App-trsm data failed");
    }

}

