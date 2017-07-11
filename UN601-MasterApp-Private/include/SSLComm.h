#ifndef __SSL_COMM_H__
#define __SSL_COMM_H__

int DLL_SSLInit(int iTermType);
int DLL_SSLConnect(int iTermType, int iCAType, int iFuncType);
int DLL_TermSSLConnect(int iTermType, int iCAType, int iFuncType);
int DLL_SSLSend(unsigned char *dataBuff,int dataLen);
int DLL_SSLRecv(unsigned char *dataBuff,int dataLen);
int DLL_SSLFree(int iTermType);
int DLL_SSLGetCA(int iCAType);
int DLL_TermSSLGetCA(int iCAType);
int DLL_SSLGetAquireCA();
int DLL_TermSSLGetAquireCA();
int DLL_SSLUpdateVendorCACert(int iTermType, int iFuncType);
int DLL_TermSSLUpdateVendorCACert(int iTermType, int iFuncType);
int DLL_SSLUpdateAquireCACert(int iTermType, int iFuncType);
int DLL_TermSSLUpdateAquireCACert(int iTermType, int iFuncType);
int DLL_SSLUpdateSignKey(unsigned char mode, unsigned char * signKey);
int DLL_SSLVendorCaCmd(int iTimeoutMs);
int DLL_SSLAquireCaCmd(int iTimeoutMs);
int DLL_SSLAppSign(unsigned char * appPerm, int iTimeoutMs);
int DLL_SSLFirmwareSign(int iTimeoutMs);
int DLL_SSLOPClient(int iPort, char* hostBuf, unsigned char* rand);
int DLL_SSLGetCACheckSum(int iCAType, unsigned char* checkSumBuff);
int DLL_SSLSendExitCaTrsmCmd(int iRet);
void DLL_SSLSendCaTrsmCancelCmd();
void DLL_SSLSendDisConnectCmd();
int DLL_TermSSLGetCACheckSum(int iCAType, unsigned char* checkSumBuff);
void DLL_Hash256Encrypt(unsigned char* dataIn, int dataInLen, unsigned char* dataOut);
int DLL_DelOpensslCertFilesDir();
void DLL_Crc16CCITT(unsigned char * buffer,int dataLen,unsigned char * crcBuff);

//int DLL_SerialOpen(unsigned char comPort);
//int DLL_SerialOpenEx(unsigned char comPort, unsigned char* comParam);
//int DLL_SerialSend(unsigned char comPort, unsigned char* sendBuff, int sendLen);
//int DLL_SerialRecv(unsigned char comPort, unsigned char* recvBuff, int maxRecvLen);
//int DLL_SerialSendCrc16Packet(unsigned char comPort, unsigned char *sendBuff, short sendLen, unsigned char cmd, unsigned char func);
//int DLL_SerialRecvCrc16Packet(unsigned char comPort, unsigned char *recvBuff, unsigned short *recvLen, int timeout);
//int DLL_SerialSendCrc32Packet(unsigned char  comPort, unsigned char *sendBuff, short sendLen, unsigned char cmd);
//int DLL_SerialRecvCrc32Packet(unsigned char comPort, unsigned char *recvBuff, unsigned short *recvLen, int timeout);
//int DLL_SerialClose(unsigned char comPort);


#endif
