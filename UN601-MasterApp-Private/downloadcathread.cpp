#include "downloadcathread.h"
#include "mappdefines.h"

extern "C" {
#include "include/SSLComm.h"
}

#include <QTimer>
#include <stdio.h>


DownloadCAThread::DownloadCAThread(QObject *parent) :
    QThread(parent)
{
        mbExitCaCertsDownload = false;
         QTimer::singleShot( DOWNLOAD_KEYS_TIME_OUT, this, SLOT(downloadKeysTimeout()) );
}

DownloadCAThread::~DownloadCAThread()
{

}

void DownloadCAThread::downloadKeysTimeout()
{
   mbExitCaCertsDownload = true;
}

void DownloadCAThread::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}


void DownloadCAThread::run()
{
    int iRet = -1;

     refreshPromptSignal("Please connect CA-Trsm, download crt...");

    for(int i = 0; i  < 4; i++)
    {
        if(mbExitCaCertsDownload)
            break;

        iRet = DLL_TermSSLGetCA(CA_TYPE_VENDOR);
        qDebug("DLL_TermSSLGetCA iRet:[%d]", iRet);
        if (0 == iRet)
        {
            break;
        }

        msleep(500);
    }

    if (mbExitCaCertsDownload ||
            (0 != iRet)) {
        DLL_SSLSendExitCaTrsmCmd(iRet);
        refreshPromptAndExit("Download vendor ca failed!");
        return;
    }

     refreshPromptSignal("Download vendor ca success");

    for(int i = 0; i  < 4; i++)
    {
        if(mbExitCaCertsDownload)
            break;

        iRet = DLL_TermSSLUpdateVendorCACert(
                    TERM_TYPE_MF2352, FUNC_TYPE_SIGN_FIRMWARE);
        qDebug("DLL_TermSSLUpdateVendorCACert firmware iRet:[%d]", iRet);
        if (0 == iRet)
        {
            break;
        }

        msleep(500);
    }



    if (mbExitCaCertsDownload ||
            0 != iRet) {
        DLL_SSLSendExitCaTrsmCmd(iRet);
        refreshPromptAndExit("Download sign-trsm crt failed!");
        return;
    }

     refreshPromptSignal("Download sign-trsm crt success");

    for(int i = 0; i  < 4; i++)
    {
        if(mbExitCaCertsDownload)
            break;

        iRet = DLL_TermSSLUpdateVendorCACert(
                    TERM_TYPE_MF2352, FUNC_TYPE_SIGN_APP);
        if (0 == iRet)
        {
            break;
        }

        msleep(500);
    }


    if (mbExitCaCertsDownload ||
            0 != iRet) {
        DLL_SSLSendExitCaTrsmCmd(iRet);
        refreshPromptAndExit("Download app-trsm crt  failed!");
        return;
    }

    for(int i = 0; i  < 4; i++)
    {
        if(mbExitCaCertsDownload)
            break;

        iRet = DLL_TermSSLUpdateVendorCACert(
                    TERM_TYPE_MF2352, FUNC_TYPE_TEST_OP);

        if (0 == iRet)
        {
            break;
        }

        msleep(500);
    }

    if (0 != iRet) {
        DLL_SSLSendExitCaTrsmCmd(iRet);
        refreshPromptAndExit("Download op test crt failed!");
        return;
    }

    DLL_SSLSendExitCaTrsmCmd(iRet);
    refreshPromptAndExit("Download all crt success");

}

