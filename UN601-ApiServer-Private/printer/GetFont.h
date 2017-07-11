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
    BYTE byValidFlag;        // 有效标识，表示该结构是否有效。0表示无效，1表示有效
    BYTE byCharSet;          // 字符编码集。0表示ASCII编码，1表示GB2312编码，2表示GBK
                             // 编码，3表示GBK18030，4表示BIG5码，5表示CJK编码

    BYTE byItalicAndWeight;  // 是否粗斜体，0常规，1是粗体，2是斜体，3是粗斜体。
    BYTE byHeightNumber;     // 字体包含字号的个数，取值范围1~8，其他值非法
    BYTE abyHeight[8];       // 相应字号的高度。
                             //  8表示 6X8 ASCII字体；
                             // 12表示                  12X12汉字字体；
                             // 16表示 8X16ASCII字体或者16X16汉字字体；
                             // 24表示12X24ASCII字体或者24X24汉字字体；
	                         // 32表示16X32ASCII字体或者32X32汉字字体，目前不支持；
	                         // 48表示24X48ASCII字体或者48X48汉字字体，目前不支持；

    BYTE abyHeightByte[8];   // 相应字号点阵所占用字节数
    BYTE abyOffsetAddr[4];   // 该字体在字库中的偏移地址，高字节在前
    BYTE abyFaceName[32];    // 字体名字，比如"宋体"
    BYTE byFontTypeFlag;     // 汉字简繁体标志。0表示简体汉字，1表示繁体汉字
    BYTE abyStepLength[2];   // 相邻两个字符间的点阵步长

    BYTE abyRev[5];          // 保留
}ChineseFontEncode;  // 64bytes

typedef struct //__attribute__ ((__packed__))
{
    BYTE byValidFlag;        // 有效标识，表示该结构是否有效。0表示无效，1表示有效
    BYTE byCharSet;          // 字符编码集。0表示ASCII编码，6表示Unicode编码

    BYTE byItalicAndWeight;  // 是否粗斜体，0常规，1是粗体，2是斜体，3是粗斜体。
    BYTE byHeightNumber;     // 字体包含字号的个数，取值范围1~8，其他值非法
    BYTE abyHeight[8];       // 相应字号的高度。
                             //  8表示 6X8 ASCII字体；
                             // 16表示 8X16ASCII字体
                             // 24表示12X24ASCII字体
	                         // 32表示16X32ASCII字体，目前不支持；
	                         // 48表示24X48ASCII字体，目前不支持；

    BYTE abyHeightByte[8];   // 相应字号点阵所占用字节数
    BYTE abyOffsetAddr[4];   // 该字体在字库中的偏移地址，高字节在前
    BYTE abyFaceName[32];    // 字体名字，比如"Arial Unicode MS"
    BYTE abyStepLength[2];   // 相邻两个字符间的点阵步长

    BYTE abyRev[6];          // 保留
}EnglishFontEncode; // 64bytes
typedef struct //__attribute__ ((__packed__))
{
    BYTE abyVFontFlag[12];      // 视融达字库标志，存储内容为"ViewAt Font"
    BYTE byVersion[3];          // 字库版本号。假设这三个字节的值分别为3、1、5则表示版
                                // 本号为3.1.5。

    BYTE byLangeType;           // 语言类型，0英语，1汉语，其他值以后如果使用到再定义
    union {
        ChineseFontEncode sCFontEncode[6];  // 汉字结构对象
        EnglishFontEncode sEFontEncode[6];  // 英文结构对象
    }tUnion;
    BYTE byLangeCodeType;  // 语言编码类型，和语言类型相关。
                      //   1、当语言类型byLangeType=1时：
                      //      byLangeCodeType=0表示字库支持ASCII和GB2312两种编码（包括）
                      //      byLangeCodeType=1表示字库支持ASCII和GBK18030两种编码
                      //   2、当语言类型byLangeType=0：
                      //      byLangeCodeType=0则表示字库采用ASCII编码存储
                      //      byLangeCodeType=1则表示字库采用Unicode编码存储

    BYTE abyRev[111];
}VFONT_LIB_STRUCT;   // 512字节

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
