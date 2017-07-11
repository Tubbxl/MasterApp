#include "optestthread.h"
#include "mappdefines.h"

extern "C" {
#include "include/SSLComm.h"
#include "include/dll_api.h"
}

#include "mappdefines.h"

#include <QTimer>
#include <stdio.h>


OPTestThread::OPTestThread(char* pszIP, int iPort, QObject *parent) :
    QThread(parent)
{
   qDebug("OPTestThread---pszIP: [%s]", pszIP);
    memset(mszIP, 0x00, sizeof(mszIP));
    strcpy(mszIP, pszIP );
    miPort = iPort;
}

OPTestThread::~OPTestThread()
{

}

void OPTestThread::refreshPromptAndExit(QString strPrompt)
{
    refreshPromptSignal(strPrompt);
    sleep(2);
    closePromptSignal();
}


void OPTestThread::run()
{
    int iRet = -1;

    refreshPromptSignal("OP Testing, please waiting...");

    unsigned char byRand[32] = {0};
    unsigned char byTemp[8] = {0};

    for(int i=0;i<4;i++)
    {

        iRet = DLL_PciGetRnd(byTemp);
        qDebug("DLL_PciGetRnd iRet:[%d]", iRet);
        if(0 != iRet)
        {
            refreshPromptAndExit("Get Random number failed!");
            return;
        }

        memcpy(&byRand[i*8], byTemp, 8);
    }

    qDebug("DLL_SSLOPClient miPort:[%d]", miPort);
    qDebug("DLL_SSLOPClient mpszIP:[%s]", mszIP);

    iRet = DLL_SSLOPClient(miPort, mszIP, byRand);
    qDebug("DLL_SSLOPClient iRet:[%d]", iRet);
    if(0 == iRet)
    {
        refreshPromptAndExit("OP test success!");
    }
    else
    {
        refreshPromptAndExit("OP test failed!");

    }

}

