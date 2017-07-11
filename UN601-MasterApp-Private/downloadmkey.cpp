#include "downloadmkey.h"
#include "mappdefines.h"
#include "mkey.h"
#include "dukpt.h"

#include "appnofile.h"

extern "C" {
#include "include/SSLComm.h"
 #include "include/dll_api.h"
}

#include <QTimer>
#include <stdio.h>


DownloadMKey::DownloadMKey(QObject *parent) :
    QThread(parent)
{
        mpSSLCommPacket = new SSLCommPacket();
        mbExitSSLConnect = false;
        mbExitDownloadMkeyThread = false;
         QTimer::singleShot( DOWNLOAD_KEYS_TIME_OUT, this, SLOT(downloadKeysTimeout()) );

         mpAppNoFile = new AppNoFile();

         mpUpdateSubApps = new UpdateSubApps();
}

DownloadMKey::~DownloadMKey()
{

}

void DownloadMKey::downloadKeysTimeout()
{
   mbExitSSLConnect = true;
}

void DownloadMKey::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}

int  DownloadMKey::hex2str(unsigned char *in, int inlen,  char *out)
{
    int i;
    char szbuf[1024];
    if(in == NULL || out == NULL)
        return -1;

    memset(szbuf, 0, sizeof(szbuf));

    for (i=0; i<inlen; i++)
        sprintf(szbuf+strlen(szbuf), "%02x", in[i]);

    strcpy((char *)out, szbuf);

    return 0;
}


void DownloadMKey::run()
{
    int iRet = -1;

    mpSSLCommPacket->sslInit();
    refreshPromptSignal("Please connect MK-Trsm to download Master Key");

    unsigned char pbyRecv[1024] = {0};
    unsigned char pbySend[1024] = {0};

     char szLogBuf[1024] = {0};

    short pwPackLen = 0;

    iRet = mpSSLCommPacket->sslConnect(CA_TYPE_ACQUIRE, FUNC_TYPE_DOWNLOAD_MK);

    if (mbExitSSLConnect ||
            (0 != iRet)) {

                if (NULL != mpSSLCommPacket)
                    mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Download MKey connect failed");
        return;
}

    unsigned char  respBuff[2] = {0};
    int respLen = 2;
    iRet = DLL_SSLRecv(respBuff, respLen);
    qDebug("DLL_SSLRecv---iRet:[%d] ", iRet);
    if (iRet <= 0 || 0 != memcmp(respBuff, "OK", 2)) {

        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Recv ready info failed");
        return;
    }

    // sn
    unsigned char bySN[12] = {0};
   iRet = DLL_SysReadSN(bySN);
   int snLen = 12;

     //app info
     int byAppNums = 0;
     iRet = mpUpdateSubApps->readAppNumsFromFile(&byAppNums);
     qDebug("mpUpdateSubApps->readAppNumsFromFile iRet:[%d]", iRet);
     if(0 != iRet)
     {

         if (NULL != mpSSLCommPacket)
             mpSSLCommPacket->sslFree();

         refreshPromptAndExit("Read app nums failed!");
         return;
     }

     int appNums = byAppNums;
     unsigned char arrAppNames[SUB_APP_MAX_NUMS][33] = {0} ;

       iRet = mpUpdateSubApps->readAppInfosFromFile(arrAppNames);
     if(0 != iRet)
     {
         if (NULL != mpSSLCommPacket)
             mpSSLCommPacket->sslFree();

         refreshPromptAndExit("Read app info failed!");
         return;
     }

     unsigned char byAppInfo[1024]  = {0};

     for(int i = 0; i < appNums; i++)
     {
         byAppInfo[33*i] = arrAppNames[i][32];
         memcpy(&byAppInfo[33*i + 1 ], arrAppNames[i], 32);
     }

    // send buffer
    int sendBuffLen = 1 + snLen + 1 + 33 * appNums;
    int currDataLen = 0;
     unsigned char bySend[1024] = {0};

    bySend[0] = snLen;
    memcpy(&bySend[1], bySN, snLen);
    currDataLen += (1 + snLen);

    bySend[currDataLen] = appNums;
    currDataLen++;


    memcpy(&bySend[currDataLen], byAppInfo, 33 * appNums);
    currDataLen += (33 * appNums);

    hex2str(bySend, sendBuffLen, szLogBuf);
    qDebug("DLL_SSLSend---App info: [%s]", szLogBuf);

     iRet = DLL_SSLSend(bySend, sendBuffLen);
   qDebug("DLL_SSLSend---iRet: [%d]", iRet);
    if (iRet < 0) {

        if (NULL != mpSSLCommPacket)
            mpSSLCommPacket->sslFree();

        refreshPromptAndExit("Send App Info failed!");
        return;
    }

    while (true) {

        if(mbExitDownloadMkeyThread)
        {
            break;
        }

        // recv mkey
        iRet = mpSSLCommPacket->crc16RecvPacket(pbyRecv, &pwPackLen, 5000);
        qDebug("Crc16RecvPacket---iRet: [%d]",  iRet);
        if (0 == iRet) {

            if (pbyRecv[1] == 0xb3 && pbyRecv[2] == 0x00) {

                int iKeyLen = (pbyRecv[3] * 256 + pbyRecv[4] - 32);
                unsigned char byKeyData[40] = {0};
                memcpy(byKeyData,  &pbyRecv[5], iKeyLen);

                unsigned char byRecvHash[32] = {0};
                 memcpy(byRecvHash,  &pbyRecv[5+iKeyLen], 32);

                unsigned char byCalcHash[32] = {0};
                unsigned char byTemp[40] = {0};
                memcpy(byTemp,  byKeyData,  40);
                DLL_Hash256Encrypt(byTemp, iKeyLen, byCalcHash);

                if(0 != memcmp(byRecvHash, byCalcHash, 32))
                {
                    pbySend[0] = 0x00;
                    pbySend[1] = 0x01;
                    pwPackLen = 2;
                    mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                            0xb3, 0xff);

                    if (NULL != mpSSLCommPacket)
                        mpSSLCommPacket->sslFree();

                    refreshPromptAndExit("Recv Mkey data format error!");
                    return;
                }

                MKey mkey =  MKey();
                mkey.byAppNo = byKeyData[0];
                mkey.byKeyTpe = byKeyData[1];
                mkey.byKeyIndex = byKeyData[2];
                mkey.byKeyLen = byKeyData[3];

                 memcpy(mkey.byKeyData,  &byKeyData[4], mkey.byKeyLen);

                 qDebug("mkey.byAppNo: [%d]", mkey.byAppNo);
                 qDebug("mkey.byKeyTpe: [%d]", mkey.byKeyTpe);
                 qDebug("mkey.byKeyIndex: [%d]",  mkey.byKeyIndex);
                 memset(szLogBuf, 0x00, sizeof(szLogBuf));
                 hex2str(mkey.byKeyData, mkey.byKeyLen, szLogBuf);
                 qDebug("mkey.byKeyData: [%s]", szLogBuf);

                 mpAppNoFile->writeCurrAppNo(mkey.byAppNo);
               qDebug("DownloadMKeyThread---mkey.byAppNo: [%d]", mkey.byAppNo);

                iRet = DLL_PciWriteMKey(mkey.byKeyTpe,
                        mkey.byKeyIndex, mkey.byKeyLen, mkey.byKeyData,
                       0x00);

                if (0 != iRet) {

                    pbySend[0] =  iRet /256;
                    pbySend[1] = iRet %256;
                    pwPackLen = 2;
                    mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                           0xb3, 0xff);

                    if (NULL != mpSSLCommPacket)
                        mpSSLCommPacket->sslFree();

                    refreshPromptAndExit("Write MKey failed!");
                    return;
                }

                pbySend[0] = iRet /256;
                pbySend[1] =iRet %256;

                pwPackLen = 2;
                mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                     0xb3, 0x01);
            }

            // recv dukpt
            else if (pbyRecv[1] == 0xb3
                    && pbyRecv[2] == 0x02) {

                int iKeyLen = (pbyRecv[3] * 256 + pbyRecv[4] - 32);
                unsigned char byKeyData[48] = {0};
                memcpy(byKeyData,  &pbyRecv[5], iKeyLen);

                unsigned char byRecvHash[32] = {0};
                memcpy(byRecvHash,  &pbyRecv[5+iKeyLen], 32);

                unsigned char byCalcHash[32] = {0};
                unsigned char byTemp[48] = {0};
                 memcpy(byTemp,  byKeyData, 48);
                DLL_Hash256Encrypt(byTemp, iKeyLen, byCalcHash);

                if(0 != memcmp(byRecvHash, byCalcHash, 32))
                {
                    pbySend[0] = 0x00;
                    pbySend[1] = 0x01;
                    pwPackLen = 2;
                    mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                            0xb3, 0xff);

                    if (NULL != mpSSLCommPacket)
                        mpSSLCommPacket->sslFree();

                    refreshPromptAndExit("Recv Dukpt data format error!");
                    return;
                }

                unsigned char keyType = 0x07;
                Dukpt dukpt =  Dukpt();

                dukpt.byAppNo = byKeyData[0];
                dukpt.byKeyIndex = byKeyData[1];
                dukpt.byBDKLen = byKeyData[2];

                memcpy(dukpt.byBDKData,  &byKeyData[4], dukpt.byBDKLen);

                dukpt.byKSNLen = byKeyData[3];

                  memcpy(dukpt.byKSNData,  &byKeyData[4+24], dukpt.byKSNLen);

                  mpAppNoFile->writeCurrAppNo(dukpt.byAppNo);
                qDebug("DownloadMKeyThread---dukpt.byAppNo: [%d]", dukpt.byAppNo);

                qDebug("dukpt.byKeyIndex: [%d]", dukpt.byKeyIndex);
                qDebug("dukpt.byBDKLen: [%d]", dukpt.byBDKLen);
                memset(szLogBuf, 0x00, sizeof(szLogBuf));
                hex2str(dukpt.byBDKData, dukpt.byBDKLen, szLogBuf);
                qDebug("dukpt.byBDKData: [%s]", szLogBuf);
                qDebug("dukpt.byKSNLen: [%d]",  dukpt.byKSNLen);
                memset(szLogBuf, 0x00, sizeof(szLogBuf));
                hex2str(dukpt.byKSNData, dukpt.byKSNLen, szLogBuf);
                qDebug("dukpt.byKSNData: [%s]", szLogBuf);
                iRet = DLL_PciWriteDukptKey(keyType,
                        dukpt.byKeyIndex, dukpt.byBDKLen,
                        dukpt.byBDKData, dukpt.byKSNLen,
                        dukpt.byKSNData, 0x00);

                if (0 != iRet) {

                    pbySend[0] = iRet/256;
                    pbySend[1] = iRet%256;
                    pwPackLen = 2;

                    mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                           0xb3, 0xff);

                    if (NULL != mpSSLCommPacket)
                        mpSSLCommPacket->sslFree();

                    refreshPromptAndExit("Write Dukpt failed!");
                    return;
                }

                pbySend[0] = iRet /256;
                pbySend[1] = iRet %256;
                pwPackLen = 2;

                mpSSLCommPacket->crc16SendPacket(pbySend, pwPackLen,
                0xb3, 0x03);

            }
            else if(pbyRecv[1] == 0xb3
                    && pbyRecv[2] == 0xff)
            {
                pbySend[0] = 0x00;
                pbySend[1] = 0x00;

                iRet = mpSSLCommPacket->crc16SendPacket(pbySend, (short) 2,
                         0xb3, 0xff);

                if (NULL != mpSSLCommPacket)
                    mpSSLCommPacket->sslFree();

                refreshPromptAndExit("Load Mkey and dukpt success!");
                return;
            }
            else
            {
                pbySend[0] = 0x00;
                pbySend[1] = 0x01;
                iRet = mpSSLCommPacket->crc16SendPacket(pbySend, (short) 2,
                        0xb3, 0xff);

                if (NULL != mpSSLCommPacket)
                    mpSSLCommPacket->sslFree();

                refreshPromptAndExit("Recv undefine data package");
                return;
            }
        } else {

            pbySend[0] = 0x00;
            pbySend[1] = 0x01;
            iRet = mpSSLCommPacket->crc16SendPacket(pbySend, (short) 2,
                    0xb3, 0xff);


            if (NULL != mpSSLCommPacket)
                mpSSLCommPacket->sslFree();

            refreshPromptAndExit("Load Mkey and dukpt timeout!");
            return;
        }
    }

}

