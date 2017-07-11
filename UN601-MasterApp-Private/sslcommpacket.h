#ifndef SSLCOMMPACKET_H
#define SSLCOMMPACKET_H

#include <QString>


class SSLCommPacket
{
public:
    SSLCommPacket();
    ~SSLCommPacket();
    int sslInit();
    int sslConnect(int iCAType, int iFuncType);
    int sslFree();
    int crc16SendPacket(unsigned char* send, short length,unsigned char byModules,unsigned char byFunction);
    int crc16RecvPacket(unsigned char* pbyRecvData, short* pwPacketetLen, int dwTimeoutMs);
};

#endif // SSLCOMMPACKET_H
