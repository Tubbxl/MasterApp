#include "sslcommpacket.h"

#include <QString>
#include "mappdefines.h"

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
 #include<fcntl.h>

extern "C" {
#include "include/SSLComm.h"
}

SSLCommPacket::SSLCommPacket()
{

}

SSLCommPacket::~SSLCommPacket()
{

}

int SSLCommPacket::sslInit()
{
     return DLL_SSLInit(TERM_TYPE_MF2352);
}

int SSLCommPacket::sslConnect(int iCAType, int iFuncType)
{
    return DLL_TermSSLConnect(TERM_TYPE_MF2352, iCAType, iFuncType);
}

int SSLCommPacket::sslFree()
{
    return DLL_SSLFree(TERM_TYPE_MF2352);
}

int SSLCommPacket::crc16SendPacket(unsigned char* send, short length,unsigned char  byModules,unsigned char  byFunction)
{
            unsigned char sendBuffer[1024] = {0};
            unsigned char crcdata[1024]  = {0};
            unsigned char crc[4] = {0};

            sendBuffer[0] = 0x02;
            sendBuffer[1] = byModules;
            sendBuffer[2] = byFunction;
            sendBuffer[3] = (length/256);
            sendBuffer[4] = (length%256);

            if(length!=0)
            {
                memcpy(&sendBuffer[5], send, length);
            }

            memcpy(crcdata, &sendBuffer[1], length + 4);

            DLL_Crc16CCITT(crcdata, length + 4, crc);

            memcpy(&sendBuffer[length+5], crc, 2);


            DLL_SSLSend(sendBuffer, length + 7);
            return  0 ;
}

int SSLCommPacket::crc16RecvPacket(unsigned char* pbyRecvData, short* pwPacketetLen, int dwTimeoutMs)
{
        int byRet;
        unsigned char abyCrc16[2] = {0};
        int i;
        unsigned char ch;
        short wPacketLen = 0;

            for(i=0;i<dwTimeoutMs;i+=1000)
            {
                byRet = DLL_SSLRecv(&ch, 1);
                if ((1==byRet) && (0x02==ch))
                {
                    pbyRecvData[0] = ch;
                    break;
                }

                usleep(100*1000);
            }

            for(i=1;i<5;i++){
                byRet = DLL_SSLRecv(&ch, 1); // timeout = 100ms
                if (1 != byRet)
                {
                    return 101;
                }
                pbyRecvData[i] = ch;
            }
            wPacketLen = (short) (pbyRecvData[3]*256 + pbyRecvData[4]);
           *pwPacketetLen = (short) (wPacketLen + 7);

            for (i=5; i<wPacketLen+7; i++)
            {
                byRet = DLL_SSLRecv(&ch, 1);
                if (1 != byRet)
                {
                    return 101;
                }
                pbyRecvData[i] = ch;
            }

            unsigned char crcdata[1024] = {0};
             unsigned char crcReturn[2] = {0};

             memcpy(crcReturn, &pbyRecvData[wPacketLen+5], 2);

             memcpy(crcdata, &pbyRecvData[1], wPacketLen+4);


            DLL_Crc16CCITT(crcdata, wPacketLen + 4, abyCrc16);

            if(0 == memcmp(crcReturn, abyCrc16, 2))
                return 0;

            return 102;
}
