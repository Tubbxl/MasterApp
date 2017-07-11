#ifndef _GET_FONT_H_
#define _GET_FONT_H_


#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef   uchar
#define   uchar   unsigned char
#endif

#define ASCII_FONT   0
#define GB2312_FONT  1


#define GB2312_INDEX_LENGTH 8599

//========================================================
//                   Local Download API
//========================================================== 
#define  DOWN_RECV_BUFFLEN      (16*1024)
#define  DOWN_SEND_BUFFLEN      (8*1024) 

#define  USBD_DOWN_MODE     0
#define  UART_DOWN_MODE     1

int  Down_RecvByte(uchar *recv_byte,int waitms);
int  Down_Recv(uchar *recv_data,int max_len,int *recv_len,int waitms);
int  Down_SendByte(uchar send_byte);
int  Down_Send(uchar *send_data,int send_len);
int  Down_Reset(void);
int  Down_Open(void);
int  Down_Close(void); 

#define FONT_NOEXISTERR        (-1600)
#define FONT_NOSUPPORTERR      (-1601)

//font api
typedef struct //__attribute__ ((__packed__))
{
    BYTE byValidFlag;        // ��Ч��ʶ����ʾ�ýṹ�Ƿ���Ч��0��ʾ��Ч��1��ʾ��Ч
    BYTE byCharSet;          // �ַ����뼯��0��ʾASCII���룬1��ʾGB2312���룬2��ʾGBK
                             // ���룬3��ʾGBK18030��4��ʾBIG5�룬5��ʾCJK����

    BYTE byItalicAndWeight;  // �Ƿ��б�壬0���棬1�Ǵ��壬2��б�壬3�Ǵ�б�塣
    BYTE byHeightNumber;     // ��������ֺŵĸ�����ȡֵ��Χ1~8������ֵ�Ƿ�
    BYTE abyHeight[8];       // ��Ӧ�ֺŵĸ߶ȡ�
                             //  8��ʾ 6X8 ASCII���壻
                             // 12��ʾ                  12X12�������壻
                             // 16��ʾ 8X16ASCII�������16X16�������壻
                             // 24��ʾ12X24ASCII�������24X24�������壻
	                         // 32��ʾ16X32ASCII�������32X32�������壬Ŀǰ��֧�֣�
	                         // 48��ʾ24X48ASCII�������48X48�������壬Ŀǰ��֧�֣�

    BYTE abyHeightByte[8];   // ��Ӧ�ֺŵ�����ռ���ֽ���
    BYTE abyOffsetAddr[4];   // ���������ֿ��е�ƫ�Ƶ�ַ�����ֽ���ǰ
    BYTE abyFaceName[32];    // �������֣�����"����"
    BYTE byFontTypeFlag;     // ���ּ����־��0��ʾ���庺�֣�1��ʾ���庺��
    BYTE abyStepLength[2];   // ���������ַ���ĵ��󲽳�

    BYTE abyRev[5];          // ����
}ChineseFontEncode;  // 64bytes

typedef struct //__attribute__ ((__packed__))
{
    BYTE byValidFlag;        // ��Ч��ʶ����ʾ�ýṹ�Ƿ���Ч��0��ʾ��Ч��1��ʾ��Ч
    BYTE byCharSet;          // �ַ����뼯��0��ʾASCII���룬6��ʾUnicode����

    BYTE byItalicAndWeight;  // �Ƿ��б�壬0���棬1�Ǵ��壬2��б�壬3�Ǵ�б�塣
    BYTE byHeightNumber;     // ��������ֺŵĸ�����ȡֵ��Χ1~8������ֵ�Ƿ�
    BYTE abyHeight[8];       // ��Ӧ�ֺŵĸ߶ȡ�
                             //  8��ʾ 6X8 ASCII���壻
                             // 16��ʾ 8X16ASCII����
                             // 24��ʾ12X24ASCII����
	                         // 32��ʾ16X32ASCII���壬Ŀǰ��֧�֣�
	                         // 48��ʾ24X48ASCII���壬Ŀǰ��֧�֣�

    BYTE abyHeightByte[8];   // ��Ӧ�ֺŵ�����ռ���ֽ���
    BYTE abyOffsetAddr[4];   // ���������ֿ��е�ƫ�Ƶ�ַ�����ֽ���ǰ
    BYTE abyFaceName[32];    // �������֣�����"Arial Unicode MS"
    BYTE abyStepLength[2];   // ���������ַ���ĵ��󲽳�

    BYTE abyRev[6];          // ����
}EnglishFontEncode; // 64bytes
typedef struct //__attribute__ ((__packed__))
{
    BYTE abyVFontFlag[12];      // ���ڴ��ֿ��־���洢����Ϊ"ViewAt Font"
    BYTE byVersion[3];          // �ֿ�汾�š������������ֽڵ�ֵ�ֱ�Ϊ3��1��5���ʾ��
                                // ����Ϊ3.1.5��

    BYTE byLangeType;           // �������ͣ�0Ӣ�1�������ֵ�Ժ����ʹ�õ��ٶ���
    union {
        ChineseFontEncode sCFontEncode[6];  // ���ֽṹ����
        EnglishFontEncode sEFontEncode[6];  // Ӣ�Ľṹ����
    }tUnion;
    BYTE byLangeCodeType;  // ���Ա������ͣ�������������ء�
                      //   1������������byLangeType=1ʱ��
                      //      byLangeCodeType=0��ʾ�ֿ�֧��ASCII��GB2312���ֱ��루������
                      //      byLangeCodeType=1��ʾ�ֿ�֧��ASCII��GBK18030���ֱ���
                      //   2������������byLangeType=0��
                      //      byLangeCodeType=0���ʾ�ֿ����ASCII����洢
                      //      byLangeCodeType=1���ʾ�ֿ����Unicode����洢

    BYTE abyRev[111];
}VFONT_LIB_STRUCT;   // 512�ֽ�

void InitFontLib(void);
int  Lib_FontFileCheck(void);
int  Lib_FontGetCharSet(BYTE *bCharSet, BYTE *bNum);
int  Lib_FontGetHeight(BYTE bCharSet, BYTE *bHeight, BYTE * bHeightNum);
int  Lib_FontGetStyle(VFONT_LIB_STRUCT *style_info); 

extern int GetCharDotMatrix(const BYTE *str, BYTE byCharSet, BYTE byFontHeight,
                     BYTE *pbyCharDotMatrix);
extern void InitFontLib();
extern int GetPrnCharDotMatrix(const BYTE *str, BYTE byCharSet, BYTE byFontHeight,
                     BYTE *pbyCharDotMatrix, BYTE byZoom);

extern int Lib_FontFileCheck(void);

extern int Lib_FontGetCharSet(BYTE *bCharSet, BYTE *bNum);
extern int Lib_FontGetHeight(BYTE bCharSet, BYTE *bHeight, BYTE * bHeightNum);
//extern int Lib_FontGetStyle(VFONT_LIB_STRUCT *style_info);


#endif
