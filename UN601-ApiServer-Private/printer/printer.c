#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <linux/fb.h>    
#include <sys/mman.h>    
#include <sys/ioctl.h> 
#include <pthread.h>

#include "GetFont.h"
#include "user.h"
#include "unitogb2312.h"
#include "Dll_api.h"

extern BYTE CurAppNo;

static BYTE  k_AscCode;             // ȡֵΪ1ʱ��ʾ���ַ��GB2312�����ַ�ȡֵΪ0ʱ��ʾ���ַ�ΪGB2312�����ַ�
extern BYTE g_uWidth;
extern BYTE g_uHeigh;
int g_iUnicodePrinterFlag = 1;//1 ��unicode�ֿⷽʽ��ӡ��0���Է�unicode�ֿⷽʽ��ӡ��
static volatile int g_BalticStrFlag=0;
int g_iPrinterUnicode=0;
extern int g_iUnicodeFontFlag;


unsigned char ** malloc_Array2D(int row, int col)  
{  
    int size = sizeof(unsigned char);  
    int point_size = sizeof(unsigned char*);  
    //�������ڴ棬����point_size * row��ʾ���row����ָ��  
    unsigned char **arr = (unsigned char **) malloc(point_size * row + size * row * col);  
    if (arr != NULL)  
    {     
        memset(arr, 0, point_size * row + size * row * col);  
        unsigned char *head = (unsigned char*)((int)arr + point_size * row);  
        while (row--)  
            arr[row] = (unsigned char*)((int)head + row * col * size);  
    }  
    return (unsigned char**)arr;  
}  


static unsigned char k_DotBuf[5000][48];  // ÿ��48���ֽ��൱��ÿ��384������


#define CHINESE_LAN	     1
#define ENGLISH_LAN      0

#define MAX_DOT_LINE     5000//5000
#define PAGE_LINES       200

#define PRN_MODE_REVERSE		0x01
#define PRN_MODE_UNDERLINE_1	0x04 //0x02
#define PRN_MODE_UNDERLINE_2    0x08  //0x04
#define PRN_MODE_ITALIC         0x08  //0x04


#define PRN_MODE_MIDDLE			0x10
#define PRN_MODE_LEFT			0x20
#define PRN_MODE_RIGHT			0x40
#define PRN_MODE_BOLD           0x80


#define PRN_BUSY              (-4001)
#define PRN_NOPAPER           (-4002)
#define PRN_DATAERR           (-4003)
#define PRN_FAULT             (-4004)
#define PRN_TOOHEAT           (-4005) 
#define PRN_UNFINISHED        (-4006)
#define PRN_NOFONTLIB         (-4007)
#define PRN_BUFFOVERFLOW      (-4008)
#define PRN_SETFONTERR        (-4009)
#define PRN_GETFONTERR        (-4010)
#define PRN_OK                (0)

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char Boolean ;
typedef int int32 ;
typedef short int16 ;

unsigned char gDecompressBuf[32*1024];
//#define DEBUG_TRACE_ENABLE 1


extern int g_iHaveFontFlag;
extern VFONT_LIB_STRUCT g_sVFontLibHead;

static int g_iAsciiFontHeight;  // ASCII�ַ�Ĵ�С���ã��?6��ʾ8X16����ȡ24��ʾ12X24����
static int g_iExternFontHeight; // ��չ�ַ�Ĵ�С���ã��?6��ʾ16X16����ȡ24��ʾ24X24����
static int g_iAsciiFontWidthZoom;    // ASCII�ַ���X2��־��0���Ŵ�1�Ŵ�2��
static int g_iAsciiFontHeightZoom;   // ASCII�ַ�߶�X2��־��0���Ŵ�1�Ŵ�2��
static int g_iExternFontWidthZoom;   //  �����ַ���X2��־��0���Ŵ�1�Ŵ�2��
static int g_iExternFontHeightZoom;  //  �����ַ�߶�X2��־��0���Ŵ�1�Ŵ�2��
static int g_FirstSpaceFlag = 0;   //PrnStr�е�һ���ַ�ǿո��ʶ�� 1�� ��ǰ�ַ��Ƿǿո�

static volatile int k_PrnStatus;     
static int k_CurDotColumn; //  ��ǰ�����к�columns
static int k_CurHeight;
static BYTE k_CurGray = 3;
static int k_CurDotLine;   // ��ǰ���ڴ�ӡ����
static int k_CharSpace;  // �ַ��ļ�࣬ȡֵ��Χ��?��255��Ĭ��ֵΪ0
static int k_LineSpace;  // �м��࣬ȡֵ��Χ��0��255��Ĭ��ֵΪ0
static int k_LeftIndent;   //  ��߽߱�հ׵���ȡֵ��Χ��0��336
static int k_FontDotLine;  //
static int k_AllDotLine;   // Ҫ��ӡ��������
static BYTE  k_CodeBytes; 

static int g_iPrintStrFlag = 0; // ��ӡ�ַ��־��?û�д�ӡ�ַ�1�д�ӡ�ַ�
static          int g_PrnMode = 0;   //  1:reverse 0:normal
static          int g_PrnModeFlag=0;   // �����ӡҪ���־�����ڴ�ӡ���к��ҿ���
static          int g_widthNum = 0;   //  �������һ�δ�ӡ�ַ�Ŀ�ȣ����ڴ�ӡ���к��ҿ�?
static          int g_Spacewidth = 0;   // �����ӡ��ǰ�пո��ַ�Ŀ��?
static          int g_CharSpace_C = 0;   //���ڴ�ӡ���к��ҿ�ʱ,�����ַ���, (X����Ŵ���һ��?�ַ���Ҳ�Ŵ�һ��????????)
static unsigned char special_mode = 0;
#define PRN_MODE_REVERSE		0x01
#define PRN_MODE_UNDERLINE		0x02

#define PAPER_OK  0   //��ֽ
#define PAPER_END 1   //ȱֽ

#define SUCCESS     0 
#define ERR_DEVICE  -1
#define ERR_PARAM   -2
#define ERR_MEM			-3

static DWORD SymFreq[320], SymCum[320], PosCum[4100];
static DWORD SymChar[320];
static BYTE DataBuf[4200];

static BYTE strBuf[2000] = {0};
static int strLen = 0;


static DWORD GetSym(DWORD x)
{
    DWORD i, j, k;

    i = 1;
    j = 314;
    while (i < j)
    {
        k = (i + j) / 2;
        if (SymCum[k] > x)
            i = k + 1;
        else
            j = k;
    }
    return i;
}
static void UpdateNode(DWORD sym)
{
    DWORD i, j, k, ch;

    if (SymCum[0] >= 0x7FFF)
    {
        j = 0;
        for (i = 314; i > 0; i--)
        {
            SymCum[i] = j;
            j += (SymFreq[i] = (SymFreq[i] + 1) >> 1);
        }
        SymCum[0] = j;
    }
    for (i = sym; SymFreq[i] == SymFreq[i - 1]; i--) ;
    if (i < sym)
    {
        k = SymChar[i];
        ch = SymChar[sym];
        SymChar[i] = ch;
        SymChar[sym] = k;
    }
    SymFreq[i]++;
    while (--i > 0)
        SymCum[i]++;
    SymCum[0]++;
}
DWORD GetPos(DWORD x)
{
    DWORD i, j, k;

    i = 1;
    j = 4096;
    while (i < j)
    {
        k = (i + j) / 2;
        if (PosCum[k] > x)
            i = k + 1;
        else
            j = k;
    }
    return (i - 1);
}

int Decompress(BYTE *srcbuf, BYTE *dstbuf, DWORD srclen, DWORD *dstlen)
{
	BYTE ch, mask, *srcend, *dstend, *sp, *dp;
	DWORD i, r, j, k, c, low, high, value, range, sym;

	ch = 0;
	mask = 0;
	low = 0;
	value = 0;
	high = 0x20000;
	sp = srcbuf;
	dp = dstbuf;

	i = *sp++;
	i <<= 8;
	i += *sp++;
	i <<= 8;
	i += *sp++;
	i <<= 8;
	i += *sp++;
	*dstlen=i;

	srcend = srcbuf + srclen;
	dstend = dstbuf + i;
	for (i = 0; i < 15 + 2; i++)
	{
		value *= 2;
		if ((mask >>= 1) == 0)
		{
			ch = (sp >= srcend) ? 0 : *(sp++);
			mask = 128;
		}
		value += ((ch & mask) != 0);
	}

	SymCum[314] = 0;
	for (k = 314; k >= 1; k--)
	{
		j = k - 1;
		SymChar[k] = j;
		SymFreq[k] = 1;
		SymCum[k - 1] = SymCum[k] + SymFreq[k];
	}
	SymFreq[0] = 0;
	PosCum[4096] = 0;
	for (i = 4096; i >= 1; i--)
		PosCum[i - 1] = PosCum[i] + 10000 / (i + 200);

	for (i = 0; i < 4096 - 60; i++)
		DataBuf[i] = ' ';
	r = 4096 - 60;
	while (dp < dstend)
	{
		range = high - low;
		sym = GetSym((DWORD)(((value - low + 1) * SymCum[0] - 1) / range));
		high = low + (range * SymCum[sym - 1]) / SymCum[0];
		low += (range * SymCum[sym]) / SymCum[0];
		while(1)
		{
			if (low >= 0x10000)
			{
				value -= 0x10000;
				low -= 0x10000;
				high -= 0x10000;
			}
			else if (low >= 0x8000 && high <= 0x18000)
			{
				value -= 0x8000;
				low -= 0x8000;
				high -= 0x8000;
			}
			else if (high > 0x10000)
				break;
			low += low;
			high += high;
			value *= 2;
			if ((mask >>= 1) == 0)
			{
				ch = (sp >= srcend) ? 0 : *(sp++);
				mask = 128;
			}
			value += ((ch & mask) != 0);
		}
		c = SymChar[sym];
		UpdateNode(sym);
		if (c < 256)
		{
			if (dp >= dstend)
				return -1;
			*(dp++) = (BYTE)c;
			DataBuf[r++] = (BYTE)c;
			r &= 4095;
		}
		else
		{
			j = c - 255 + 2;
			range = high - low;
			i = GetPos((DWORD)
				(((value - low + 1) * PosCum[0] - 1) / range));
			high = low + (range * PosCum[i	  ]) / PosCum[0];
			low += (range * PosCum[i + 1]) / PosCum[0];
			while(1)
			{
				if (low >= 0x10000)
				{
					value -= 0x10000;
					low -= 0x10000;
					high -= 0x10000;
				}
				else if (low >= 0x8000 && high <= 0x18000)
				{
					value -= 0x8000;
					low -= 0x8000;
					high -= 0x8000;
				}
				else if (high > 0x10000)
					break;
				low += low;
				high += high;
				value *= 2;
				if ((mask >>= 1) == 0)
				{
					ch = (sp >= srcend) ? 0 : *(sp++);
					mask = 128;
				}
				value += ((ch & mask) != 0);
			}
			i = (r - i - 1) & 4095;
			for (k = 0; k < j; k++)
			{
				c = DataBuf[(i + k) & 4095];
				if (dp >= dstend)
					return -1;
				*(dp++) = (BYTE)c;
				DataBuf[r++] = (BYTE)c;
				r &= 4095;
			}
		}
	}
	return 0;
}

int s_GetPrnFontDot(const BYTE *str, BYTE *OutDot, int *DotWidth,
                    int *DotHeight, int *DotNibble, BYTE *pbyOutAsciiFlag,
                    BYTE *pbyCharLen)
{
    BYTE byZoom = 0;
    int iRet = 0;
    int i = 0;

    if(0 == g_iHaveFontFlag)
    {
        //return 3;
    }
    *pbyOutAsciiFlag = 0;
    if ((str[0]<0x80) || (g_sVFontLibHead.byLangeType==ENGLISH_LAN))
    {
        *pbyOutAsciiFlag = 1;
    }
    else if (g_sVFontLibHead.byLangeType == CHINESE_LAN)
    {
        if ((str[0]<0x81) || (str[0]==0xff) || (str[1]<0x40) || (str[1]==0xff))
        {
            *pbyOutAsciiFlag=1;
        }
        else if ((str[0]==0xd7) && (str[1]>=0xfa))
        {
            *pbyOutAsciiFlag=1;
        }
    }

    if (*pbyOutAsciiFlag)
    {
        if (24 == g_iAsciiFontHeight)
        {
            *DotWidth = 2;
            *DotHeight = 24; // 12X24����
            *DotNibble = 4;  // ���ڿ���������8��������ַ����Ŀǰֻ����12X24�������ȡֵ��?

            if (0 != g_iAsciiFontWidthZoom)
            {
                *DotWidth = 3; // ˮƽ����Ŵ�?
                byZoom |= 0x01;
                *DotNibble = 0;
            }
            if (0 != g_iAsciiFontHeightZoom)
            {
                *DotHeight = 48; // ��ֱ����Ŵ�?
                byZoom |= 0x02;
            }
            memset(OutDot, 0x55, (*DotHeight)*(*DotWidth));
//			logs();
            iRet = GetPrnCharDotMatrix(str, 0, 24, OutDot, byZoom);
            //for(i = 0 ; i < 48; i++) printf("%02x ",OutDot[i]);
        }
        else
        {
            *DotWidth = 1;
            *DotHeight = 16;  // 8X16����
            *DotNibble = 0;
            if (0 != g_iAsciiFontWidthZoom)
            {
                *DotWidth = 2;  //  ˮƽ����Ŵ�?
                byZoom |= 0x01;
            }
            if (0 != g_iAsciiFontHeightZoom)
            {
                *DotHeight = 32; // ��ֱ����Ŵ�?
                byZoom |= 0x02;
            }
            memset(OutDot, 0x55, (*DotHeight)*(*DotWidth));
		//	logs();
            iRet = GetPrnCharDotMatrix(str, 0, 16, OutDot, byZoom);
        }
        *pbyCharLen = 1;
		k_CodeBytes = 1;
    }
    else if(g_sVFontLibHead.byLangeType == CHINESE_LAN)
    {
        *DotNibble = 0;
        *pbyCharLen = 2;
        k_CodeBytes = 2;
        if(24 == g_iExternFontHeight)
        {
            *DotWidth = 3;
            *DotHeight = 24;  // 24X24����
            if (0 != g_iExternFontWidthZoom)
            {
                *DotWidth = 6; // ˮƽ����Ŵ�?
                byZoom |= 0x01;
            }
            if (0 != g_iExternFontHeightZoom)
            {
                *DotHeight = 48; // ��ֱ����Ŵ�?
                byZoom |= 0x02;
            }
            memset(OutDot, 0x55, (*DotHeight)*(*DotWidth));

            iRet = GetPrnCharDotMatrix(str, 1, 24, OutDot, byZoom);
        }
        else
        {

            *DotWidth = 2;
            *DotHeight = 16;  // 16X16����
            if (0 != g_iExternFontWidthZoom)
            {
                *DotWidth = 4; // ˮƽ����Ŵ�?
                byZoom |= 0x01;
            }
            if (0 != g_iExternFontHeightZoom)
            {
                *DotHeight = 32; // ��ֱ����Ŵ�?
                byZoom |= 0x02;
            }
            memset(OutDot, 0x55, (*DotHeight)*(*DotWidth));

            iRet = GetPrnCharDotMatrix(str, 1, 16, OutDot, byZoom);
        }
    }
    return iRet;
}

static int s_NewLine(void)
{
    if (k_PrnStatus == PRN_BUFFOVERFLOW)
    {
        return -1;
    }
    if (k_CurDotColumn == k_LeftIndent)  // ��ǰ���ǿհ���
    {
        k_CurHeight = 16;  // ֱ�ӻ���
        if ((k_CurDotLine+16+k_LineSpace) > MAX_DOT_LINE)
        {
            k_PrnStatus = PRN_BUFFOVERFLOW;
            k_CurDotLine = MAX_DOT_LINE;
            return -1;
        }
        k_CurDotLine += (16 + k_LineSpace);
        return 0;
    }
    k_CurDotLine += k_CurHeight;
    k_CurDotColumn = k_LeftIndent;
    k_CurHeight = 16;  // ���к�ȡ��Сֵ
    if ((k_CurDotLine+k_LineSpace) > MAX_DOT_LINE)
    {
        k_PrnStatus = PRN_BUFFOVERFLOW;
        k_CurDotLine = MAX_DOT_LINE;
        return -1;  // �������?
    }
    k_CurDotLine += k_LineSpace;
//	logs();
    return 0;
}


static void s_ProcChar_back(const BYTE *str, BYTE *pbyCharLen)
{
    int i, j, shift;
    int width, height, nibble;
    BYTE ch, ch1, *tmPtr, *tDotPtr, *Font, byAsciiFlag;
    BYTE tmpBuf[300];
	int nSize;

    i = s_GetPrnFontDot(str,tmpBuf,&width,&height,&nibble, &byAsciiFlag, pbyCharLen);
	//memset(tmpBuf,0xff,sizeof(tmpBuf));
	if (0 != i)
    {
        k_PrnStatus = PRN_NOFONTLIB;
    }
    //printf("nibble is %d,width is %d\r\n",nibble,width);
    if (nibble)
    {
        i = 12;  // 12X24��������ķֱ��ʲ������nibble!=0�����?
    }
    else
    {
        i = width*8;   // ��λ���㣬���ֽ�ת���ɱ���λ
    }
    //printf("cur dot column is %d\r\n",k_CurDotColumn);
    if ((k_CurDotColumn+i) > 384)
    {
        if (s_NewLine())
        {
            return;  // ����˳�?
        }
    }
		if((g_PrnMode&PRN_MODE_REVERSE) && g_FirstSpaceFlag==1)
		{	
			for(nSize = 0;nSize<=width*height; nSize++)
			{
				if((*str == '\n') || (*str=='\f'))
					continue;
				tmpBuf[nSize] ^=0xff;
			}
	
		}
	
		if ((g_PrnMode&PRN_MODE_UNDERLINE) && g_FirstSpaceFlag==1)
		{//�»���
			//s_UartPrint(COM_DEBUG, "l");
			for(nSize = width*height-width; nSize<width*height; nSize++)
			{
				if((*str == '\n') || (*str=='\f'))
					continue;
				tmpBuf[nSize] = 0xff;
			}
		}
	
		///*int s_ReverseProc(const BYTE* str,BYTE* tmpBuf,uchar ucCmd)
		{
			if((g_PrnMode & (PRN_MODE_REVERSE)) )
			{
	
				if((g_iAsciiFontHeight == 24)&& (*str < 0x80))
				{
					//s_UartPrint(COM_DEBUG,"\r\n\r\nASCII\r\n\r\n" );
					for(i=7;i<(width*height)-2;i+=2)
					{
						if(*str++ == '\0')
							break;;
						if((*str == ' ')||(*str  == '\n')||(*str  == '\f'))
							continue;
						tmpBuf[i] &=0xf0;
	
					}
				}
			for(i=0;i<(width*height);i++)
			{
				//s_UartPrint(COM_DEBUG,"%02x ", tmpBuf[i]);
			}
			//s_UartPrint(COM_DEBUG,"\r\n");
	
			}
	
		}
    //printf("curh is %d,height is %d\r\n",k_CurHeight,height);
    if(k_CurHeight < height)
        k_CurHeight = height;
    tDotPtr = k_DotBuf[k_CurDotLine] + k_CurDotColumn/8;

    shift = k_CurDotColumn%8;
    Font = tmpBuf;
    for(i=0;i<height;i++)
    {
        tmPtr=tDotPtr;
        for(j=0;j<width;j++)
        {
            ch = *Font++;
            ch1 = ch>>shift;
            *tmPtr++ |= ch1;
            ch <<= (8-shift);
            if(ch)
                *tmPtr = ch;
        }
        tDotPtr += 48;
    }

    k_CurDotColumn += (width*8-nibble);
    if (byAsciiFlag && g_iAsciiFontWidthZoom
        || !byAsciiFlag && g_iExternFontWidthZoom)
    {
        k_CurDotColumn += k_CharSpace*2;
    }
    else
    {
        k_CurDotColumn += k_CharSpace;
    }
}


#define isSpecialCode(a) 0
#define GetBalticCharDot(a,b,c,d,e,f) 0

int OffSet(unsigned char *dat,unsigned char len,int offset)
{
	int i,j;
	unsigned char temp[10];
	unsigned char bitDat[80]={0};
	for(i=0;i<len;i++)
	{
		for(j=0;j<8;j++)
		if(dat[i]&(1<<(7-j)))
			bitDat[8*i+j] = 1;
		else bitDat[8*i+j] = 0;
	}
	memset(dat,0,len);
	if(offset>=0)
	{

		for(i=0;i<len*8-offset;i++)
		{
			if(bitDat[i])
				dat[(i+offset)/8] |= 1<<(7-(i+offset)%8);
		}
		
	}
	else
	{
		offset = 0-offset;
		for(i=0;i<len*8+offset;i++)
		{
			if(bitDat[i])
				dat[(i-offset)/8] |= 1<<(7-(i-offset)%8);
		}
		//for(i = len*8-offset;i<len*8;i++)
		//	if(bitDat[i])
		//		dat[(i)/8] |= 1<<(7-(i)%8);
	}
	
	
}


/****************************************************************************
  ������     :  static void s_ProcChar(const BYTE *str, BYTE *pbyCharLen)
  ����       :  ��������ַ�ת���ɵ��󲢱��浽��ӡ�������?
  �������?  :  1��const BYTE *str���ַ�����
  �������?  :  2��BYTE *pbyCharLen���ַ�ĳ��ȣ�Ŀǰȡֵֻ����?����2��
                   1��ʾ���ֽ��ַ���ASCII�����ַ�
                   2��ʾ˫�ֽ��ַ���GB2312�ַ�
                   �����ܻ�����3����4�ֽڳ��ȵ��ַ����UNICODE�ַ�
  ����ֵ     :  ��
  �޸���ʷ   :
      �޸���     �޸�ʱ��    �޸İ汾��   �޸�ԭ��
  1��
  2�� �ƿ���     2008-01-04  V1.1         ��VOS302��ֲ��VOS305
****************************************************************************/
static void s_ProcChar(/*const*/ BYTE *str)
{
	int i, j, shift,k;
	int width, height, nibble;
	BYTE ch, ch1, *tmPtr, *tDotPtr, *Font;
	BYTE tmpBuf[300];
	int nSize;
	BYTE pbyOutAsciiFlag;
    BYTE pbyCharLen;
	//LOGD("s_ProcChar str = %02x %d", *(str),g_BalticStrFlag);

	if (g_BalticStrFlag)
		//i =	s_GetBalticPrnFontDot(str,tmpBuf,&width,&height,&nibble);
		i = 0;
		
	else	
		i = s_GetPrnFontDot(str,tmpBuf,&width,&height,&nibble, &pbyOutAsciiFlag, &pbyCharLen);
	if (i<0)
		return;

	for(j=0;j<(width*height);j++)
	{
	}
	if (i)
	{
		k_PrnStatus = PRN_NOFONTLIB;
	}
	else
	{
	}

	
	if (nibble == 0)
	{
		i = width*8;
	}
	else
	{
		if (!g_BalticStrFlag)
		{
			i = 12;  // 12X24��������ķֱ��ʲ������nibble!=0�����?
		}
		else
		{
			if (nibble==6 && height==16)
				i = 10;
			else if (nibble==4 && height==24)
				i = 12;
			else if (nibble==6 && height==32 )
				i = 18;
			else
				i = 12;
			if (isSpecialCode(*str))
				i = i*2;
		}
	}
	if ((k_CurDotColumn+i) > 384)
	{
		if (s_NewLine())
		{
			return;  // ����˳�?
		}
	}

	if((g_PrnMode&PRN_MODE_REVERSE) && g_FirstSpaceFlag==1)//�ַ�ǰ���пո�Ҳ����
	{
		for(nSize = 0;nSize<=width*height; nSize++)
		{
			if((*str == '\n') || (*str=='\f'))
			{
				continue;
			}
			tmpBuf[nSize] ^=0xff;	
		}

	}
	if ((g_PrnMode&PRN_MODE_ITALIC) && g_FirstSpaceFlag==1)
	{//�»���
		for(i=0;i<height;i++)
			OffSet(&tmpBuf[width*i],width,height/6-i/3);
	}

	if ((g_PrnMode&PRN_MODE_UNDERLINE_1) && g_FirstSpaceFlag==1)
	{//�»���
		//dbg_printf( "l");
		for(nSize = width*height-width; nSize<width*height; nSize++)
		{
			if((*str == '\n') || (*str=='\f'))
				continue;
			tmpBuf[nSize] = 0xFF;	
		}
	}
	if ((g_PrnMode&PRN_MODE_BOLD) && g_FirstSpaceFlag==1)
	{
		//LOGD("PRN_MODE_BOLD");
		for(nSize = 0; nSize<width*height; nSize++)
		{
			if((*str == '\n') || (*str=='\f'))
				continue;
			for(i=0;i<8;i++)
			{
				if(tmpBuf[nSize]&(1<<(7-i)))
				{
					if(nSize%width==0&&i==0) continue;
					else if(i==0) tmpBuf[nSize-1] |= 1<<7;
					else tmpBuf[nSize] |= 1<<(8-i);
				}
			}
		}
	}

	//03�����?30���?33������,00����
	if((g_PrnMode & PRN_MODE_REVERSE)) 
	{
		if ((((g_iExternFontHeightZoom || g_iExternFontWidthZoom) && (!g_iAsciiFontWidthZoom || !g_iAsciiFontHeightZoom)))
			|| (!g_iAsciiFontWidthZoom&&!g_iAsciiFontHeightZoom&&!g_iExternFontWidthZoom&&!g_iExternFontHeightZoom))

		{
			if((g_iAsciiFontHeight == 24)&& (*str < 0x80))
			{
				for(i=7;i<(width*height)-2;i+=2)
				{
					if (!g_iUnicodePrinterFlag)
					{
						if(*(str+1) == '\0')
						{
							break;
						}
					}
					else
					{
						if(*str++ == '\0')
						{
							break;
						}
					}
					if (g_iUnicodePrinterFlag)
					{
						if((*str == ' ')||(*str  == '\n')||(*str  == '\f'))//
						{
							continue;
						}
					}
					else
					{
						if((*(str+2)  == ' ')||(*(str+2)  == '\n')||(*(str+2) == '\f'))//
						{
							continue;
						}
					}
					tmpBuf[i] &=0xf0;
				}
			}
		}
	}


	if(k_CurHeight < height)
			k_CurHeight = height;
	
	tDotPtr = k_DotBuf[k_CurDotLine] + k_CurDotColumn/8;
	
	shift = k_CurDotColumn%8;
	Font = tmpBuf;
	for(i=0;i<height;i++)
	{
		tmPtr=tDotPtr;
		for(j=0;j<width;j++)
		{
			ch = *Font++;
			ch1 = ch>>shift;
			*tmPtr++ |= ch1;
			ch <<= (8-shift);
			/*if(ch)
				*tmPtr = ch;*/
			if(ch)
			{
				if(0 == (((uint)tmPtr-(uint)k_DotBuf)%48))
					;//     *(tmPtr+48*width) = ch;
				else
					*(tmPtr) = ch;
			}
		}
		tDotPtr += 48;
	}

	k_CurDotColumn += (width*8-nibble);
	if(g_PrnMode & (PRN_MODE_REVERSE|PRN_MODE_UNDERLINE_1|PRN_MODE_UNDERLINE_2))
	{
		return;
	}

	if(k_AscCode && g_iAsciiFontWidthZoom || !k_AscCode && g_iExternFontWidthZoom)
	{
//		s_UartPrint(COM_DEBUG,"k_CharSpace1 is %d\r\n",k_CharSpace);
		k_CurDotColumn += k_CharSpace*2;
	}
	else
	{
//		s_UartPrint(COM_DEBUG,"k_CharSpace2 is %d\r\n",k_CharSpace);
		k_CurDotColumn += k_CharSpace;
	}
}


/*==================================================================
* Function	: Prn_SetGray
* Description	:  ���ô�ӡ��Ҷȣ����ٶȳɷ���?
* Input Para	: �Ҷȵȼ�
* Return Value: 
* date			: ---�����?20121121
==================================================================*/
int Lib_PrnSetGray(BYTE nLevel)
{
	if (nLevel > 7)
		nLevel = 7;
 	k_CurGray = nLevel;	
	return SUCCESS;
}

void Lib_PrnStep(int pixel)
{
    if ((!pixel) || ((k_CurDotLine+pixel)<0))
    {
        return;
    }
    if (k_PrnStatus == PRN_BUFFOVERFLOW)
    {
        return;
    }
    if (k_CurDotColumn != k_LeftIndent)
    {
        if (s_NewLine())
        {
            return;
        }
    }
    if ((k_CurDotLine+pixel) > MAX_DOT_LINE)
    {
        k_PrnStatus = PRN_BUFFOVERFLOW;
        pixel = MAX_DOT_LINE - k_CurDotLine;
    }
    k_CurDotLine += pixel;
}

int ustrlen(char *str) 
{ 
	char* p = str;
//	while(!(*p == 0 && *(p+1) == 0)) p += 2;
	while((*p != 0 || *(p+1) != 0)) p += 2;

	return (int)(p-str);// /2
}  

//����F8
int gustrlen(char *str)
{
	int i = 0;
	char *p = str;
	while(!(*p == 0 && *(p+1) == 0)) 
	{
		p += 2;
		if (*p == 0 && *(p+1) != 0)
		{
			i++;
		}
	}
	return (int)(p-str);
}

ushort strlen_u(uchar *pData,ushort size)
{
	  ushort i;
	 

	  i= 0;
	  
	  while (1)
	  {
           if ((pData[i]==0) && (pData[i+1]==0))
           {
			   return i;
           }
		   else
		   {
                  i=i+2;

				  if (i>=size-1)
				  {
					  return i;
				  }
		   }
	  }
}


void Lib_PrnSetLeftIndent(int x)
{
    if (x > 336)
    {
        x = 336;
    }
    /*else if (x < 0)
    {
        return;
    }*/
    if (k_LeftIndent != k_CurDotColumn)
    {
        (void)s_NewLine();
    }
    k_LeftIndent = x;
    k_CurDotColumn = x;
}


static void s_ProcChar_u(BYTE *str)
{
     int i, j, shift;
	int width, height, nibble;
	BYTE ch, ch1, *tmPtr, *tDotPtr, *Font;
	BYTE tmpBuf[300];
	BYTE WidthByte;
	int nSize;

//	LOG("s_ProcChar_u = %02x %02x", *(str), *(str+1));
//	s_UartPrint(COM_DEBUG,"s_ProcChar_u\r\n");
	i = s_GetPrnFontDot_u(str,tmpBuf,&width,&height);
//	LOG("s_GetPrnFontDot_u ret = %d", i);
	if (i<0){
		//LOGE("s_GetPrnFontDot_u ret = %d", i);
		return;
	}
	//for(j=0;j<(width*height);j++)
	//{
	//	//dbg_printf("tmpBuf[j]=%02x " ,tmpBuf[j]);
	//}
	if (i)
	{
		k_PrnStatus = PRN_NOFONTLIB;
		//LOGE("PRN_NOFONTLIB");
	}
	else
	{
	//	LOG("PRN_FONTLIB");
	}

	if ((k_CurDotColumn + width) > 384)
	{
		//s_NewLine();
		if (s_NewLine())
		{
			return;  // ����˳�?
		}
	}

	WidthByte = width/8;
	if(width%8 != 0)
		WidthByte += 1;


	if((g_PrnMode&PRN_MODE_REVERSE))// && g_FirstSpaceFlag==1)�ַ�ǰ���пո�Ҳ����
	{
		for(nSize = 0;nSize<=WidthByte*height; nSize++)
		{
			//if((*str == '\n') || (*str=='\f'))
			if(((str[0]==0x00)&&(str[1]==0x0A)) || ((str[0]==0x00)&&(str[1]==0x0C)))
				continue;
			//tmpBuf[nSize] &=0xff;	
			tmpBuf[nSize] = ~(tmpBuf[nSize] & 0xff);
		}
	}
	if ((g_PrnMode&PRN_MODE_ITALIC) && g_FirstSpaceFlag==1)
	{//�»���
		for(i=0;i<height;i++)
			OffSet(&tmpBuf[WidthByte*i],WidthByte,height/6-i/3);
	}

	if ((g_PrnMode&PRN_MODE_UNDERLINE_1) && g_FirstSpaceFlag==1)
	{//�»���
		//dbg_printf( "l");
		for(nSize = WidthByte*height-WidthByte; nSize<WidthByte*height; nSize++)
		{
		//	if((*str == '\n') || (*str=='\f'))
			if(((str[0]==0x00)&&(str[1]==0x0A)) || ((str[0]==0x00)&&(str[1]==0x0C)))
				continue;
			tmpBuf[nSize] = 0xFF;	
		}
	}
/*
	if ((g_PrnMode&PRN_MODE_UNDERLINE_2) && g_FirstSpaceFlag==1)
	{//�»���
		//dbg_printf( "l");
		for(nSize = WidthByte*height-WidthByte; nSize<WidthByte*height; nSize++)
		{
			//if((*str == '\n') || (*str=='\f'))
			if(((str[0]==0x00)&&(str[1]==0x0A)) || ((str[0]==0x00)&&(str[1]==0x0C)))
				continue;
			tmpBuf[nSize] = 0xFF;	
			tmpBuf[nSize-WidthByte] = 0xFF;
		//	tmpBuf[nSize-2*width] = 0xFF;
		//	tmpBuf[nSize-3*width] = 0xFF;
		}
	}
*/	
	if ((g_PrnMode&PRN_MODE_BOLD) && g_FirstSpaceFlag==1)
	{
		//LOGD("size:%d",WidthByte*height);
		for(nSize = 0; nSize<WidthByte*height; nSize++)
		{
			if(((str[0]==0x00)&&(str[1]==0x0A)) || ((str[0]==0x00)&&(str[1]==0x0C)))
				continue;
			//LOGD("nSize:%d",nSize);
			for(i=0;i<8;i++)
			{
				if(tmpBuf[nSize]&(1<<(7-i)))
				{
					if(nSize%WidthByte==0&&i==0) continue;
					else if(i==0) tmpBuf[nSize-1] |= 0x1;
					else tmpBuf[nSize] |= (unsigned char)1<<(8-i);
				}
			}
		}
	}
	
	
	if(k_CurHeight < height)
		k_CurHeight = height;

//	s_UartPrint(COM_DEBUG,"k_CurDotColumn is %d\r\n",k_CurDotColumn);
	tDotPtr = k_DotBuf[k_CurDotLine] + k_CurDotColumn/8;

	shift = k_CurDotColumn%8;
//	s_UartPrint(COM_DEBUG,"shift is %d\r\n",shift);
	Font = tmpBuf;

	for(i=0;i<height;i++)
	{
		tmPtr=tDotPtr;
		for(j=0;j<WidthByte;j++)
		{
			ch = *Font++;
			ch1 = ch>>shift;
			*tmPtr++ |= ch1;
			ch <<= (8-shift);
			/*if(ch)
				*tmPtr = ch;*/
			if(ch)
			{
				if(0 == (((uint)tmPtr-(uint)k_DotBuf)%48))
					;//     *(tmPtr+48*width) = ch;
				else
					*(tmPtr) = ch;
			}
		}
		tDotPtr += 48;
	}
//	s_UartPrint(COM_DEBUG,"width is %d\r\n",width);
//	if(g_PrnMode&PRN_MODE_REVERSE)
//		k_CurDotColumn += WidthByte*8;
//	else
	k_CurDotColumn += width;
	if(g_PrnMode & (PRN_MODE_REVERSE|PRN_MODE_UNDERLINE_1|PRN_MODE_UNDERLINE_2))
	{
		return;
	}

	if(k_AscCode && g_iAsciiFontWidthZoom || !k_AscCode && g_iExternFontWidthZoom)
		k_CurDotColumn += k_CharSpace*2;
	else
		k_CurDotColumn += k_CharSpace;	

}



int s_GetPrnFontDot_u(const BYTE *str, BYTE *OutDot, int *DotWidth,int *DotHeight)
{
    BYTE byZoom = 0;
	int iRet = 0;
	BYTE cWidthByte;
	int i,j;

	BYTE BufDot[300];
    BYTE TempDot[300];

	if(0 == g_iHaveFontFlag)
	{
		//dbg_printf( "nono ");
		//return 3;
	}
	iRet = GetPrnCharDotMatrix(str, 1, g_iExternFontHeight, BufDot, byZoom);//16,24
//	LOG("GetPrnCharDotMatrix iRet = %d", iRet);
//	s_UartPrint(COM_DEBUG,"BufDot is:\r\n");
//	for (i = 0 ; i < ((g_uWidth+ 7) / 8)* g_uHeigh; i++)
//	{
//		s_UartPrint(COM_DEBUG,"%02x ",BufDot[i]);
//	}
//	s_UartPrint(COM_DEBUG,"\r\n");
   
    *DotWidth=g_uWidth;
	*DotHeight=g_uHeigh;//_pfl_header->YSize;

	if(g_uWidth==0)
		return -1;

	if (g_uWidth<8)
	{
		cWidthByte=1;
	}
	else
	{
		cWidthByte=g_uWidth/8;
		if ((g_uWidth%8)!=0)
		{
			cWidthByte+=1;
		}
	}

	if (g_iExternFontHeightZoom)
	{
          byZoom |= 0x02;
	}

	if (g_iExternFontWidthZoom)
	{
		byZoom |= 0x01;
	}

	byZoom &= 0x03;
//	byZoom = 3;
	switch(byZoom)
	{
	     case 0:
             memset(OutDot, 0x55, (*DotHeight)*cWidthByte);
			 for (i=0;i<g_uHeigh;i++)
			 {
                  for (j=0;j<cWidthByte;j++)
                  {
                       OutDot[i*cWidthByte+j]=BufDot[i*cWidthByte+j];
                  }
			 }
			 break;
		 case 1:
			 {
				 ushort WidthBitCount1;//,WidthBitCount2;
				 ushort ByteCnt;//1,ByteCnt2;
				 uchar cWidth;
				 uchar k;
				 *DotWidth=g_uWidth*2;
                 
				  cWidth = *DotWidth/8;
				  if((*DotWidth%8)!=0)
					  cWidth += 1;
				  memset(OutDot, 0x00, (*DotHeight)*cWidth);

				 //  s_UartPrint(COM_DEBUG,"*DotHeight=%d,cWidthByte=%d,cWidth=%d",*DotHeight,cWidthByte,cWidth);

				  WidthBitCount1 = 0;
			//	  WidthBitCount2 = 0;
				  ByteCnt = 0;
				//  ByteCnt2 = 0;

				 for (i=0;i<g_uHeigh;i++)
				 {
					   WidthBitCount1 = 0;
                       for (j=0;j<cWidthByte;j++)
                       {
						   for (k=0;k<8;k++)
						   {
                                 if (BufDot[i*cWidthByte+j]&(0x80>>k))
								 {
									 OutDot[ByteCnt] |= 0xC0>>((2*(k>=4) ? 2*(k-4):2*k));
									  if((k==3) || (k==7))
										  ByteCnt++;
								 }
								 else
								 {
									  OutDot[ByteCnt] &= ~(0xC0>>((2*(k>=4) ? 2*(k-4):2*k)));
									  if((k==3) || (k==7))
										  ByteCnt++;
								 }

								 WidthBitCount1++;
								 if(WidthBitCount1>=g_uWidth)
								 {
									 if (WidthBitCount1==g_uWidth)//������ֻ��Ҫ����һ���ֽ�
									 {
										 ByteCnt++;
									 }
									 if ((k==3)||(k==7))
									 {
										 ByteCnt--;
									 }
								 }
						   }
						     
                       }
				 }
			 }
			 break;
		 case 2:
			 {
				 *DotHeight = g_uHeigh * 2;
				 memset(OutDot, 0x55, (*DotHeight)*cWidthByte);

				// s_UartPrint(COM_DEBUG,"*DotHeight=%d,cWidthByte=%d",*DotHeight,cWidthByte);
				 for (i = 0;i < g_uHeigh;i++)   //��
				 {
					 for (j = 0;j < cWidthByte;j++)   //��
					 {
						 OutDot[2*i*cWidthByte+j]=BufDot[i*cWidthByte+j];
						 OutDot[(2*i+1)*cWidthByte+j]=BufDot[i*cWidthByte+j];
					 }
				 }
			 }
			 break;
		 default:
			 {
				 ushort WidthBitCount1;//,WidthBitCount2;
				 ushort ByteCnt;//1,ByteCnt2;
				 uchar cWidth;
				 uchar k;

				 *DotWidth=g_uWidth*2;
				 *DotHeight=g_uHeigh*2;

				 cWidth = *DotWidth/8;
				 if((*DotWidth%8)!=0)
					 cWidth += 1;
				 memset(OutDot, 0x00, (*DotHeight)*cWidth);
				 memset(TempDot, 0x00, sizeof(TempDot));
				 //  s_UartPrint(COM_DEBUG,"*DotHeight=%d,cWidthByte=%d,cWidth=%d",*DotHeight,cWidthByte,cWidth);

				 WidthBitCount1 = 0;
				 //	  WidthBitCount2 = 0;
				 ByteCnt = 0;
				 //  ByteCnt2 = 0;

				 for (i=0;i<g_uHeigh;i++)
				 {
					 WidthBitCount1 = 0;
					 for (j=0;j<cWidthByte;j++)
					 {
						 for (k=0;k<8;k++)
						 {
							 if (BufDot[i*cWidthByte+j]&(0x80>>k))
							 {
								 TempDot[ByteCnt] |= 0xC0>>((2*(k>=4) ? 2*(k-4):2*k));
								 if((k==3) || (k==7))
									 ByteCnt++;
							 }
							 else
							 {
								 TempDot[ByteCnt] &= ~(0xC0>>((2*(k>=4) ? 2*(k-4):2*k)));
								 if((k==3) || (k==7))
									 ByteCnt++;
							 }

							 WidthBitCount1++;
							 if(WidthBitCount1>=g_uWidth)
							 {
								 if (WidthBitCount1==g_uWidth)//������ֻ��Ҫ����һ���ֽ�
								 {
									 ByteCnt++;
								 }
								 if ((k==3)||(k==7))
								 {
									 ByteCnt--;
								 }
								 //s_UartPrint(COM_DEBUG,"ByteCnt is %d\r\n",ByteCnt);
							 }
						 }
					 }
				 } 
				 cWidthByte=cWidth;
				 for (i = 0;i < g_uHeigh;i++)   //��
				 {
					 for (j = 0;j < cWidthByte;j++)   //��
					 {
						 OutDot[2*i*cWidthByte+j]=TempDot[i*cWidthByte+j];
						 OutDot[(2*i+1)*cWidthByte+j]=TempDot[i*cWidthByte+j];
					 }
				 }
			 }
			 break;
	}
   
/*
	if(g_iExternFontWidthZoom)
	{
		 if(g_uWidth==0)
			return -1;

		 if (g_uWidth<8)
		 {
			  cWidthByte=1;
		 }
		 else
		 {
			 cWidthByte=g_uWidth/8;
			 if (g_uWidth%8==0)
			 {
				 cWidthByte+=1;
			 }
		 }
		 if (g_uHeigh == 16)
		 {
               *DotWidth=g_uWidth*2;
			   for (i=0;i<g_uHeigh;i++)
			   {  
				   for (j=0;j<g_uWidth;j++)
				   {
                       //OutDot[i]
				   }
			   }
		 }
	}*/
	return iRet;
}


int PrnString_u(BYTE *str)
{
	int i; 
	int k;

	int strLen;
	int c_LeftIndent, c_LeftIndent_tmp;
	int n;
	BYTE tmpStr[1024];

	k_CodeBytes = 2;

	c_LeftIndent = k_LeftIndent;
	if (g_PrnModeFlag == 1)
	{//�����ӡҪ��?
		if ((g_PrnMode&PRN_MODE_MIDDLE) == PRN_MODE_MIDDLE)
		{
			c_LeftIndent = k_LeftIndent;
//			s_UartPrint("width number is %d\r\n",g_widthNum);
			if (g_iExternFontHeight == 24)
			{
				if (g_iAsciiFontHeightZoom || g_iExternFontHeightZoom)
				{
					n = (384-g_widthNum)/2;  
					n /= (24+g_CharSpace_C);	//g_CharSpace_C
					c_LeftIndent_tmp = 384 - g_widthNum - n*(24+g_CharSpace_C);//-----------------
					c_LeftIndent_tmp /= 2;
				}
				else
				{
					n = (384-g_widthNum)/2;  
					n /= (12+g_CharSpace_C);	//g_CharSpace_C
					c_LeftIndent_tmp = 384 - g_widthNum - n*(12+g_CharSpace_C);//-----------------
					c_LeftIndent_tmp /= 2;
				}
			}
			if (g_iExternFontHeight == 16)
			{
				if (g_iAsciiFontHeightZoom || g_iExternFontHeightZoom)
				{
					n = (384-g_widthNum)/2;  
					n /= (16+g_CharSpace_C);	//g_CharSpace_C
					c_LeftIndent_tmp = 384 - g_widthNum - n*(16+g_CharSpace_C);//-----------------
					c_LeftIndent_tmp /= 2;
				}
				else
				{
					n = (384-g_widthNum)/2;  
					n /= (8+g_CharSpace_C);	//g_CharSpace_C
					c_LeftIndent_tmp = 384 - g_widthNum - n*(8+g_CharSpace_C);//-----------------
					c_LeftIndent_tmp /= 2;
				}
			}
			Lib_PrnSetLeftIndent(c_LeftIndent_tmp);

			memset(tmpStr, 0, sizeof(tmpStr));

			for(k=0;k<n;k++)
			{
			     tmpStr[2*k]=0x00;
				 tmpStr[2*k+1]=0x20;
			}

			memcpy(tmpStr+2*n,str,ustrlen((char *)str));
			memcpy(str, tmpStr, ustrlen((char *)tmpStr));//
		}
		else if ((g_PrnMode&PRN_MODE_RIGHT) == PRN_MODE_RIGHT)
		{
			//dbg_printf( "oooooooooooooooo\r\n");
			c_LeftIndent = k_LeftIndent;
			if (g_iExternFontHeight == 24)
			{
				if (g_iAsciiFontHeightZoom || g_iExternFontHeightZoom)
				{
					n = 384-g_widthNum;  
					n /= (24+g_CharSpace_C);
					//dbg_printf( "n=%d ", n);
					c_LeftIndent_tmp = 384 - g_widthNum - n*(24+g_CharSpace_C)/2;//------------------------
				}
				else
				{
					n = 384-g_widthNum;  
					n /= (12+g_CharSpace_C);
					//dbg_printf( "n=%d ", n);
					c_LeftIndent_tmp = 384 - g_widthNum - n*(12+g_CharSpace_C)/2;//------------------------
				}
			}
			if (g_iExternFontHeight == 16)
			{
				if (g_iAsciiFontHeightZoom || g_iExternFontHeightZoom)
				{
					n = 384-g_widthNum;  
					n /= (16+g_CharSpace_C);
					//dbg_printf( "n=%d ", n);
					c_LeftIndent_tmp = 384 - g_widthNum - n*(16+g_CharSpace_C)/2;//------------------------
				}
				else
				{
					n = 384-g_widthNum;  
					n /= (8+g_CharSpace_C);
					//dbg_printf( "n=%d ", n);
					c_LeftIndent_tmp = 384 - g_widthNum - n*(8+g_CharSpace_C)/2;//------------------------
				}
			}
			Lib_PrnSetLeftIndent(c_LeftIndent_tmp);
			memset(tmpStr, 0, sizeof(tmpStr));

            for(k=0;k<n;k++)
			{
			     tmpStr[2*k]=0x00;
				 tmpStr[2*k+1]=0x20;
			}

			memcpy(tmpStr+2*n,str,ustrlen((char *)str));
            memcpy(str, tmpStr, ustrlen((char *)tmpStr));//+2*n
			//k_LeftIndent = c_LeftIndent;
		}
		else
		{//Ĭ��
			//dbg_printf( "jjjjjjjjjjjjjjjj\r\n");
			c_LeftIndent = k_LeftIndent;
			n = 0;

			memset(tmpStr, 0, sizeof(tmpStr));
			for(k=0;k<n;k++)
			{
			     tmpStr[2*k]=0x00;
				 tmpStr[2*k+1]=0x20;
			}
			//strcpy(tmpStr+2*n, str);
			memcpy(tmpStr+2*n,str,ustrlen((char *)str));
            memcpy(str, tmpStr, ustrlen((char *)tmpStr));//+2*n

			k_LeftIndent = c_LeftIndent;
		}

	}

	i=0;
	g_FirstSpaceFlag = 0;
	while(1)
	{
		if (k_PrnStatus == PRN_BUFFOVERFLOW)
		{
			Lib_PrnSetLeftIndent(c_LeftIndent);
			return k_PrnStatus;
		}
		//if (!str[i])
		if((str[i]==0)&&(str[i+1] == 0))
		{
			if (0 != i)
			{
				g_iPrintStrFlag = 1;
			}

			Lib_PrnSetLeftIndent(c_LeftIndent);
			return 0;
		}
	//	if (str[i]=='\n')  // ����
		if ((str[i]==0x00)&&(str[i+1]==0x0A))  // ����
		{
			(void)s_NewLine();
			//i++;
			i=i+2;
			continue;
		}
	//	if (str[i] == '\f')  //��ҳ
		if ((str[i]==0x00)&&(str[i+1]==0x0C)) //��ҳ
		{
			if (k_CurDotLine != k_LeftIndent)
			{
				(void)s_NewLine();
			}
			k_FontDotLine = k_CurDotLine;
			Lib_PrnStep(PAGE_LINES);
			//i++;
			i=i+2;
			continue;
		}
		else 
		{
			k_FontDotLine = 0;
			if (k_CurDotLine == k_LeftIndent)
			{
				if ((k_CurDotLine+48) > MAX_DOT_LINE)
				{
					Lib_PrnSetLeftIndent(c_LeftIndent);
					k_PrnStatus = PRN_BUFFOVERFLOW;
					return k_PrnStatus;
				}
			}

			//����һ��prn_strǰ���ǿո��ַ��������»��ߺͷ��Թ���
			//if (*(str+i) != ' ')
			if(!((*(str+i) == 0x00) && (*(str+i+1) == 0x20)))
			{//
			//	dbg_printf( "kkkkkkkkkkkkkkk\r\n");
				g_FirstSpaceFlag = 1;
			}

//			else
//			{
				s_ProcChar_u(str+i);
//			}
		}
		i += k_CodeBytes;
	}
}

int PrnString(BYTE *str)
{
	int i; 

	int strLen;
	int c_LeftIndent, c_LeftIndent_tmp;
	int n;
	BYTE tmpStr[1024];
	BYTE byCharLen;

	c_LeftIndent = k_LeftIndent;
	if (g_PrnModeFlag == 1)
	{//�����ӡҪ��?
		if ((g_PrnMode&PRN_MODE_MIDDLE) == PRN_MODE_MIDDLE)
		{

			//dbg_printf( "tttttttttttttttt\r\n");
			c_LeftIndent = k_LeftIndent;
			//dbg_printf( "%d,%d ", g_widthNum,g_Spacewidth);
			n = (384-g_widthNum)/2;  
			n /= (g_Spacewidth+g_CharSpace_C);	//g_CharSpace_C
			//dbg_printf( "n=%d ", n);
			c_LeftIndent_tmp = 384 - g_widthNum - 2*n*(g_Spacewidth+g_CharSpace_C);
			c_LeftIndent_tmp /= 2;
			//dbg_printf( "c_LeftIndent_tmp=%d,%d\r\n", c_LeftIndent_tmp, g_CharSpace_C);
			Lib_PrnSetLeftIndent(c_LeftIndent_tmp);

			memset(tmpStr, 0, sizeof(tmpStr));
			memset(tmpStr, ' ', n);
			strcpy((char *)(tmpStr+n), (char *)str);
			memcpy(str, tmpStr, strlen((char *)tmpStr));
			//k_LeftIndent = c_LeftIndent;
		}
		else if ((g_PrnMode&PRN_MODE_RIGHT) == PRN_MODE_RIGHT)
		{
			//dbg_printf( "oooooooooooooooo\r\n");
			c_LeftIndent = k_LeftIndent;
			//dbg_printf( "%d,%d ", g_widthNum, g_Spacewidth);
			n = 384-g_widthNum;  
			n /= (g_Spacewidth+g_CharSpace_C);
			//dbg_printf( "n=%d ", n);
			c_LeftIndent_tmp = 384 - g_widthNum - n*(g_Spacewidth+g_CharSpace_C);

			Lib_PrnSetLeftIndent(c_LeftIndent_tmp);
			memset(tmpStr, 0, sizeof(tmpStr));
			memset(tmpStr, ' ', n);
			strcpy((char *)(tmpStr+n), (char *)str);
			memcpy(str, tmpStr, strlen((void *)tmpStr));
			//k_LeftIndent = c_LeftIndent;
		}
		else
		{//Ĭ��
			//dbg_printf( "jjjjjjjjjjjjjjjj\r\n");
			c_LeftIndent = k_LeftIndent;
			//dbg_printf( "%d,%d ", g_widthNum,g_Spacewidth);
			n = 0;
			//dbg_printf( "n=%d ", n);

			memset(tmpStr, 0, sizeof(tmpStr));
			memset(tmpStr, ' ', n);
			strcpy((char *)tmpStr+n,(char *)str);
			memcpy(str, tmpStr, strlen((void *)tmpStr));
			k_LeftIndent = c_LeftIndent;
		}

	}



	i=0;
	g_FirstSpaceFlag = 0;
	while(1)
	{
		if (k_PrnStatus == PRN_BUFFOVERFLOW)
		{
			Lib_PrnSetLeftIndent(c_LeftIndent);
			return k_PrnStatus;
		}
		if (!str[i])
		{
			if (0 != i)
			{
				g_iPrintStrFlag = 1;
			}

			Lib_PrnSetLeftIndent(c_LeftIndent);
			return 0;
		}
		if (str[i]=='\n')  // ����
		{
			(void)s_NewLine();
			i++;
			continue;
		}
		if (str[i] == '\f')  //��ҳ
		{
			if (k_CurDotLine != k_LeftIndent)
			{
				(void)s_NewLine();
			}
			k_FontDotLine = k_CurDotLine;
			Lib_PrnStep(PAGE_LINES);
			i++;
			continue;
		}
		else 
		{
			k_FontDotLine = 0;
			if (k_CurDotLine == k_LeftIndent)
			{
				if ((k_CurDotLine+48) > MAX_DOT_LINE)
				{
					Lib_PrnSetLeftIndent(c_LeftIndent);
					k_PrnStatus = PRN_BUFFOVERFLOW;
					return k_PrnStatus;
				}
			}

			//����һ��prn_strǰ���ǿո��ַ��������»��ߺͷ��Թ���
			if (*(str+i) != ' ')
			{//
				//dbg_printf( "kkkkkkkkkkkkkkk\r\n");
				g_FirstSpaceFlag = 1;
			}


			s_ProcChar(str+i);//, &k_CodeBytes
		}
		i += k_CodeBytes;
	}
}

void Unicodetest()
{
	BYTE printdata[400];
	//BYTE pbyCharDotMatrix[20];
    //g_iPrinterFormat = 2;
    //����ת�� \u4e2d\u6587\u8f6c\u6362
	printdata[0]=0X4E;//0X1E;//0x30;//0x00;
	printdata[1]=0x00;//0XE0;//0x41;//0x45;

	printdata[2]=0X4E;//0X1E;//0x30;//0x00;
	printdata[3]=0x8C;//0X00;//0x42;//0x42;

	printdata[4]=0X4C;//0X1E;//0x30;//0x4e;
	printdata[5]=0x09;//0X20;//0x43;//0x10;

	printdata[6]=0X63;//0X1E;//0x30;//0x4e;
	printdata[7]=0x62;//0X30;//0x61;//0x50;

	printdata[8]=0X54;//0X1E;//0x30;//0x4e; hehe
	printdata[9]=0x75;//0X40;//0xc0;//0x30;

	printdata[10]=0X54;//0X1E;//0x30;//0x4e;
	printdata[11]=0x75;//0X50;//0xc1;//0x60;

	printdata[12]=0X5E;//0X1E;//0x4e;	���Y���x�x\u5e36\u8cc7\u6599\u8b1d\u8b1d
	printdata[13]=0x36;//0X60;//0xc0;

	printdata[14]=0X8C;//0X1E;//0x4e;
	printdata[15]=0xC7;//0X70;//0xc1;

	printdata[16]=0X65;//0X1E;//0x4e;
	printdata[17]=0x99;//0X80;//0xf0;

	printdata[18]=0X8B;//0x1E;//0X30;
	printdata[19]=0x1D;//0X00;//0X41;//0x00;

	printdata[20]=0X8B;//0X1E;//0x30;//0x00;
	printdata[21]=0x1D;//0X01;//0x42;//0x42;

	printdata[22]=0X00;//0X1E;//0x30;//0x4e;
	printdata[23]=0x61;//0X20;//0x43;//0x10;

	printdata[24]=0X00;//0X1E;//0x30;//0x4e;
	printdata[25]=0x62;//0X21;//0x61;//0x50;

	printdata[26]=0X00;//0X1E;//0x30;//0x4e;
	printdata[27]=0x41;//0X40;//0xc0;//0x30;

	printdata[28]=0X00;//0X1E;//0x30;//0x4e;
	printdata[29]=0x42;//0X41;//0xc1;//0x60;

	printdata[30]=0X00;//0x1e;//0x4e;
	printdata[31]=0x45;//0x60;//0xc0;

	printdata[32]=0X1E;//0x1E;//0x4e;
	printdata[33]=0xc3;//0xa0;//0xc1;

	printdata[34]=0X1E;//0x1E;//0x4e;
	printdata[35]=0xc4;//0xc0;//0xf0;


	printdata[36]=0X1E;//0X1E;//0x30;//0x00;
	printdata[37]=0x41;//0XE0;//0x41;//0x45;

	printdata[38]=0X1E;//0X1E;//0x30;//0x00;
	printdata[39]=0x42;//0X00;//0x42;//0x42;

	printdata[40]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[41]=0x43;//0X20;//0x43;//0x10;

	printdata[42]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[43]=0x61;//0X30;//0x61;//0x50;

	printdata[44]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[45]=0xc0;//0X40;//0xc0;//0x30;

	printdata[46]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[47]=0xc1;//0X50;//0xc1;//0x60;

	printdata[48]=0X1E;//0X1E;//0x4e;
	printdata[49]=0xc2;//0X60;//0xc0;

	printdata[50]=0X1E;//0X1E;//0x4e;
	printdata[51]=0xc3;//0X70;//0xc1;

	printdata[52]=0X1E;//0X1E;//0x4e;
	printdata[53]=0xc4;//0X80;//0xf0;

	printdata[54]=0X1E;//0x1E;//0X30;
	printdata[55]=0x41;//0X00;//0X41;//0x00;

	printdata[56]=0X1E;//0X1E;//0x30;//0x00;
	printdata[57]=0x42;//0X01;//0x42;//0x42;

	printdata[58]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[59]=0x43;//0X20;//0x43;//0x10;

	printdata[60]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[61]=0x61;//0X21;//0x61;//0x50;

	printdata[62]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[63]=0xc0;//0X40;//0xc0;//0x30;

	printdata[64]=0X1E;//0X1E;//0x30;//0x4e;
	printdata[65]=0xc1;//0X41;//0xc1;//0x60;

	printdata[66]=0X1E;//0x30;//0x1e;//0x4e;
	printdata[67]=0XC2;//0xc2;//0x60;//0xc0;

	printdata[68]=0X1E;//0x1E;//0x4e;
	printdata[69]=0x4e;//0xa0;//0xc1;

	printdata[70]=0X1E;//0x1E;//0x4e;
	printdata[71]=0x45;//0xc0;//0xf0;

	s_PrnUnicodeStr(printdata);
}

int s_PrnUnicodeStr(BYTE *str)
{
	int strLen;
	int c_LeftIndent, c_LeftIndent_tmp;
	int n;
	
	int charWith, totalWith, byZoom = 0;
	int height;
	int iRet = 0; 
	int i, j;
	int k;
	BYTE tmpStr[1024];
	BYTE lineNum;//����
	BYTE tmpBuf[300];

     k_CodeBytes = 2;//20120414
     if(g_PrnModeFlag)
	 {
	      memset(tmpStr, 0, sizeof(tmpStr));
		  i = 0;
		  j = 0;
		  totalWith = 0;
		  g_widthNum = 0;
		  g_Spacewidth = 0;
		  
		 
		  g_CharSpace_C = k_CharSpace;
		  
		  while(1)
		  {
		        if((str[i] == 0)&&(str[i+1] == 0))
				{
				    g_widthNum = totalWith-g_CharSpace_C;

					tmpStr[j+k_CodeBytes] = 0;
					tmpStr[j+k_CodeBytes+1] = 0;


					g_PrnModeFlag = 1;
					PrnString_u(tmpStr);
					memset(tmpStr, 0, sizeof(tmpStr));
					return 0;
				}

				if((str[i] == 0) && (str[i+1] == 0x0A))//UNICODE  ���� \n
				{
				     g_widthNum = totalWith-g_CharSpace_C;
					 tmpStr[j+k_CodeBytes] = 0;
					 tmpStr[j+k_CodeBytes+1] = 0;
					 g_PrnModeFlag = 1;
					 PrnString_u(tmpStr);
					 memset(tmpStr, 0, sizeof(tmpStr));
					 g_PrnModeFlag = 0;			

					 tmpStr[0] = 0x00;//'\n';
					 tmpStr[1] = 0x0A;//0;
					 tmpStr[2] = 0x00;
					 tmpStr[3] = 0x00;

					 j = 0;
					 i=i+2;
					 totalWith = 0;
					 continue;
				}
				if ((str[i] == 0x00) && (str[i+1] == 0x0c))  //��ҳ'\f'
				{//�ȴ��?ҳǰ���ַ�
					
					g_widthNum = totalWith;
					tmpStr[j+k_CodeBytes] = 0;
					tmpStr[j+k_CodeBytes+1] = 0;
					g_PrnModeFlag = 1;
					memset(tmpStr, 0, sizeof(tmpStr));
					g_PrnModeFlag = 0;			

					tmpStr[0] = 0x00;//'\f';
					tmpStr[1] = 0x0C;//0;
					tmpStr[2] = 0x00;
					tmpStr[3] = 0x00;
					PrnString_u(tmpStr);

					j = 0;
					i=i+2;
					totalWith = 0;
					continue;
				}
				else
				{
					s_GetPrnFontDot_u(str+i,tmpBuf,&charWith,&height);

					totalWith = totalWith + charWith; //��ǰ������
					if (totalWith >  384)
					{//����
						g_PrnModeFlag = 0;
						//dbg_printf( "3 ");
						lineNum++;
						totalWith = 0;
						tmpStr[j+k_CodeBytes] = 0;
						tmpStr[j+k_CodeBytes+1] = 0;
						PrnString_u(tmpStr);
						j = 0;
						memset(tmpStr, 0, sizeof(tmpStr));
						continue;
					}
					else
					{
						totalWith = totalWith + g_CharSpace_C; //��ǰ������
					//	if (k_CodeBytes==2)
						{
							tmpStr[j] = str[i];
							tmpStr[j+1] = str[i+1];
							g_Spacewidth = charWith/2;
						}
						j += k_CodeBytes;
					}
				}
				i += k_CodeBytes;
		  }
		  g_PrnModeFlag = 1;
		  return 0;
	 }
	 c_LeftIndent = k_LeftIndent;
	 if (g_PrnModeFlag == 1)
	 {//�����ӡҪ��?
	 	//	logs();
	        if ((g_PrnMode&PRN_MODE_MIDDLE) == PRN_MODE_MIDDLE)
			{
				c_LeftIndent = k_LeftIndent;
				n = (384-g_widthNum)/2;  
				n /= (g_Spacewidth+g_CharSpace_C);	//g_CharSpace_C
				c_LeftIndent_tmp = 384 - g_widthNum - 2*n*(g_Spacewidth+g_CharSpace_C);
				c_LeftIndent_tmp /= 2;
				Lib_PrnSetLeftIndent(c_LeftIndent_tmp);

				memset(tmpStr, 0, sizeof(tmpStr));

				for(k=0;k<n;k++)
				{
					tmpStr[2*k]=0x00;
				    tmpStr[2*k+1]=0x20;
				}

				strcpy((char *)(tmpStr+2*n), (char *)str);
				memcpy(str, tmpStr, strlen_u(tmpStr,sizeof(tmpStr)));
			}
			else if ((g_PrnMode&PRN_MODE_RIGHT) == PRN_MODE_RIGHT)
			{
				c_LeftIndent = k_LeftIndent;
				n = 384-g_widthNum;  
				n /= (g_Spacewidth+g_CharSpace_C);

				c_LeftIndent_tmp = 384 - g_widthNum - n*(g_Spacewidth+g_CharSpace_C);

				Lib_PrnSetLeftIndent(c_LeftIndent_tmp);
				memset(tmpStr, 0, sizeof(tmpStr));

				for(k=0;k<n;k++)
				{
					tmpStr[2*k]=0x00;
				    tmpStr[2*k+1]=0x20;
				}

				strcpy((char *)(tmpStr+2*n), (char *)str);
				memcpy(str, tmpStr, strlen_u(tmpStr,sizeof(tmpStr)));
			}
			else
			{//Ĭ��
				c_LeftIndent = k_LeftIndent;
				n = 0;

				memset(tmpStr, 0, sizeof(tmpStr));
				for(k=0;k<n;k++)
				{
					tmpStr[2*k]=0x00;
				    tmpStr[2*k+1]=0x20;
				}

				strcpy((char *)(tmpStr+2*n), (char *)str);
				memcpy(str, tmpStr, strlen_u(tmpStr,sizeof(tmpStr)));
				k_LeftIndent = c_LeftIndent;
			}
	 }

	 i=0;
	g_FirstSpaceFlag = 0;

	while(1)
	{
		
		if (k_PrnStatus == PRN_BUFFOVERFLOW)
		{
			Lib_PrnSetLeftIndent(c_LeftIndent);
			return k_PrnStatus;
		}
		if ((str[i]==0)&&(str[i+1])==0)
		{
			if (0 != i)
			{
				g_iPrintStrFlag = 1;
			}
			Lib_PrnSetLeftIndent(c_LeftIndent);
			return 0;
		}
		if ((str[i]==0x00)&&(str[i+1]==0x0A))  // ����\n
		{
			//LOG("change line\r\n");
			(void)s_NewLine();
			i=i+2;
			continue;
		}
		if ((str[i] == 0x00)&&(str[i+1]==0x0C))  //��ҳ\f
		{
			if (k_CurDotLine != k_LeftIndent)
			{
				(void)s_NewLine();
			}
			k_FontDotLine = k_CurDotLine;
			Lib_PrnStep(PAGE_LINES);
			i=i+2;
			continue;
		}
		else 
		{
			k_FontDotLine = 0;
	//		LOG("1111111111   k_CurDotLine = %d, k_LeftIndent = %d", k_CurDotLine, k_LeftIndent);
			if (k_CurDotLine == k_LeftIndent)
			{
				if ((k_CurDotLine+48) > MAX_DOT_LINE)
				{
					Lib_PrnSetLeftIndent(c_LeftIndent);
					k_PrnStatus = PRN_BUFFOVERFLOW;
					return k_PrnStatus;
				}
			}
			//����һ��prn_strǰ���ǿո��ַ��������»��ߺͷ��Թ���
			if(!((*(str+i) == 0x00)&&(*(str+i+1) == 0x20)))
			{
				g_FirstSpaceFlag = 1;
			}
			k_CodeBytes = 2;
			//	LOG("str+i = %02x", *(str+i));
			s_ProcChar_u(str+i);
			/*
			if (*(str+i)==0x00 && ((*(str+i+1)>=0x00)&&(*(str+i+1)<=0x7F)))
			{
//				s_UartPrint(COM_DEBUG,"i is %d\r\n",i);
				g_iUnicodePrinterFlag = 0;
			//	k_CodeBytes = 1;
				s_ProcChar(str+i+1);//, &k_CodeBytes
				
				i++;
				g_iUnicodePrinterFlag = 1;
			}
			else
			{
				k_CodeBytes = 2;
			//	LOG("str+i = %02x", *(str+i));
				s_ProcChar_u(str+i);
			}
			*/
		}
		i += k_CodeBytes;
	}
	
}


int s_PrnStr(/*const*/ BYTE *str)
{
	int i; 

	int strLen;
	int c_LeftIndent, c_LeftIndent_tmp;
	int n;
	BYTE tmpStr[1024];

	c_LeftIndent = k_LeftIndent;

	i=0;
	g_FirstSpaceFlag = 0;
	while(1)
	{
		if (k_PrnStatus == PRN_BUFFOVERFLOW)
		{
            Lib_PrnSetLeftIndent(c_LeftIndent);
            return k_PrnStatus;
		}
		if (!str[i])
		{
            if (0 != i)
            {
                g_iPrintStrFlag = 1;
            }
			(void)s_NewLine();
            return 0;
		}
		if (str[i]=='\n')  // ����
		{
            (void)s_NewLine();
            i++;
            continue;
		}
		if (str[i] == '\f')  //��ҳ
		{
                    if (k_CurDotLine != k_LeftIndent)
                    {
                        (void)s_NewLine();
                    }
                    k_FontDotLine = k_CurDotLine;
                    Lib_PrnStep(PAGE_LINES);
                    i++;
                    continue;
		}
		else 
		{
            k_FontDotLine = 0;
            if (k_CurDotLine == k_LeftIndent)
            {
                if ((k_CurDotLine+48) > MAX_DOT_LINE)
                {
                    Lib_PrnSetLeftIndent(c_LeftIndent);
                    k_PrnStatus = PRN_BUFFOVERFLOW;
                    return k_PrnStatus;
                }
            }
			//����һ��prn_strǰ���ǿո��ַ��������»��ߺͷ��Թ���
			if (*(str+i) != ' ')
			{//
				//s_UartPrint(COM_DEBUG, "kkkkkkkkkkkkkkk\r\n");
				g_FirstSpaceFlag = 1;
			}				
            s_ProcChar(str+i);//,&k_CodeBytes
		}
		i += k_CodeBytes;
	}
}



int SearchUnicode_Tab1(unsigned short *Unicode,unsigned short *addr)
{
	unsigned short low;
	unsigned short high;
	unsigned short mid;
	unsigned short uni;
	uni = *Unicode;

	low = 0;
	high = MAX_SIZE_UNICODETOGB2312 - 2;
	mid = 0;

	//  s_UartPrint(COM_DEBUG,"*Unicode = %x,%d",*Unicode,*Unicode);
	while (low<=high)
	{
		mid=(high+low)/2;
		if (mid%2)
		{
			mid = mid-1;
		}

		if (Unicode_GB2312_Tab1[mid] == uni)
		{
			*addr = mid;

			return 0;
		}
		if(Unicode_GB2312_Tab1[mid] < uni)
		{
			low = mid + 2;			  
		}
		else
		{
			high = mid - 2;
		}		
	}

	return -1;
}


int utf8_to_unicode(char *in, char *out,int insize)
{ 
	int outputsize=0;
	int inptr=0;
	int outptr=0;
	int counter=insize;
	if(insize>=3&&in[0]==0xef&&in[1]==0xbb&&in[2]==0xbf)
	{  
		inptr++;  
		inptr++;  
		inptr++;  
		counter--;  
		counter--;  
		counter--; 
	} 
	while(counter>0) 
	{  
		if(in[inptr]>0x00&&in[inptr]<=0x7f)  
		{   
			//printf("one bytes\r\n");   
			out[outptr]=0x00;   
			outptr++;   
			out[outptr]=in[inptr];  
		}  
		else if((in[inptr]&0xe0)==0xc0)
		{ 
			//printf("two bytes\r\n");
			char high=in[inptr];
			inptr++;
			counter--;
			char low=in[inptr];
			if((low&0xc0)!=0x80)
			{
				printf("two bytes low location %d, data %02x\r\n",inptr,in[inptr]);
				return -1;
			}   
			out[outptr]=(high>>2)&0x07;  
			outptr++;  
			out[outptr]=(high<<6)+(low&0x3f); 
		}
		else if((in[inptr]&0xf0)==0xe0) 
		{   
			//printf("three bytes\r\n"); 
			char high=in[inptr];   
			inptr++;  
			counter--;  
			char middle=in[inptr]; 
			inptr++;  
			counter--;  
			char low=in[inptr]; 
			if(((middle&0xc0)!=0x80)||((low&0xc0)!=0x80)) 
			{
				printf("three bytes low or middle location %d,low data %02x middle %02x\r\n",inptr,in[inptr],middle);
				return -1; 
			}  
			out[outptr]=(high<<4)+((middle>>2)&0x0f);
			outptr++;   
			out[outptr]=(middle<<6)+(low&0x7f);  
		} 
		else 
		{  
			printf("three bytes high location %d, data %02x counter %d insize %d \r\n",inptr,in[inptr], counter, insize);
			return -1;  
		}  
		inptr++;  
		outptr++;  
		counter--; 
		outputsize+=2; 
	} 
	return outputsize;
}


int UnicodeToGB2312(unsigned short *Unicode,unsigned short *GB2312)
{
	unsigned short pAddr;
	int ret;

    ret=SearchUnicode_Tab1(Unicode,&pAddr);
	if (ret==0)
	{
		*GB2312=Unicode_GB2312_Tab1[pAddr+1];
		return 0;
	}

	*GB2312 = 0;
	return -1;
}


//uchar *u:[in] unicode unicodeëΪ2ؖ½ڣ¬ˤɫʱ¸ֽࠗ۔ۇ°µΗֽ۔ں�?uchar *gb:[out]  gb2312
int Str_UnicodeToGB2312(uchar *u,uchar *gb,unsigned short len)
{
    //  unsigned short len;
	  unsigned short i,j=0;
	  unsigned short temp;
	  unsigned char a[2];
	  int ret = 0;

	  uchar l,h;
//	  LOGD_HEX(u,len);

	  for (i=0;i<len;)
	  {
		  h = u[i++];
		  l = u[i++];
		   if ((l==0) && (h==0))
		   {
			   break;
		   }
		   if ((l<0xA4) && (h<=0))
		   {
			   gb[j++]=l;
			   continue;
		   }
		   temp = h*0x100+l;
		   ret = UnicodeToGB2312(&temp,(unsigned short *)&a[0]);
		   
		   if(ret == 0)
		   {
              gb[j++]=a[1];
			  gb[j++]=a[0];
		   }
		   else
		   {
              gb[j++]=0;
			  return ret;
		   }
	  }
	  gb[j++]=0;
	  return ret;
}


void PrnReverseSet (char isReverse) 
{
	if(isReverse)
		g_PrnMode |= 0x1;
	else
		g_PrnMode &= ~0x1;
}


void PrnUnderlineSet (char isUnderline) 
{
	if(isUnderline)
		g_PrnMode |= PRN_MODE_UNDERLINE_1;
	else
		g_PrnMode &= ~PRN_MODE_UNDERLINE_1;
}
void PrnBoldSet (char isBold) 
{
	if(isBold)
		g_PrnMode |= PRN_MODE_BOLD;
	else
		g_PrnMode &= ~PRN_MODE_BOLD;
}
void PrnItalicSet (char isItalic) 
{
	if(isItalic)
		g_PrnMode |= PRN_MODE_ITALIC;
	else
		g_PrnMode &= ~PRN_MODE_ITALIC;
}

int Prn_SetAlignment(BYTE Alignment)
{
   switch (Alignment)
   {
       case 0x00:
		   g_PrnMode &= ~PRN_MODE_MIDDLE;
		   g_PrnMode &= ~PRN_MODE_RIGHT;
		   g_PrnMode |= PRN_MODE_LEFT;
		   g_PrnModeFlag = 1;
		   break;
       case 0x01:
		   g_PrnMode &= ~PRN_MODE_LEFT;
		   g_PrnMode &= ~PRN_MODE_RIGHT;
		   g_PrnMode |= PRN_MODE_MIDDLE;
		   g_PrnModeFlag = 1;
		   break;
	   case 0x02:
		   g_PrnMode &= ~PRN_MODE_LEFT;
		   g_PrnMode &= ~PRN_MODE_MIDDLE;
		   g_PrnMode |= PRN_MODE_RIGHT;
		   g_PrnModeFlag = 1;
		   break;
	   default:
		   g_PrnMode &= ~PRN_MODE_MIDDLE;
		   g_PrnMode &= ~PRN_MODE_RIGHT;
		   g_PrnMode |= PRN_MODE_LEFT;
		   g_PrnModeFlag = 0;
		   break;
   }

   return 0;
}

int Prn_GetAlignment(BYTE *Alignment)
{
       if ((g_PrnMode & PRN_MODE_MIDDLE) == PRN_MODE_MIDDLE)
	   {
		   if(g_PrnModeFlag)
		      *Alignment = PRN_MODE_MIDDLE;
		   else
			   *Alignment = PRN_MODE_LEFT;
	   }
	   else if ((g_PrnMode & PRN_MODE_RIGHT) == PRN_MODE_RIGHT)
	   {
		   if(g_PrnModeFlag)
			   *Alignment = PRN_MODE_RIGHT;
		   else
			   *Alignment = PRN_MODE_LEFT;
	   }
	   else
	   {
            *Alignment = PRN_MODE_LEFT;
	   }

	   return 0;
}

int DLL_PrnSetMode(unsigned char mode)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	if(mode&0x01)
		PrnUnderlineSet(1);
	else
		PrnUnderlineSet(0);
	
	if(mode&0x02)
		PrnBoldSet(1);
	else 
		PrnBoldSet(0);
	if(mode&0x04)
		PrnItalicSet(1);
	else 
		PrnItalicSet(0);
	if(mode&0x08)
		Prn_SetAlignment(0);
	else if(mode&0x10)
		Prn_SetAlignment(1);
	else if(mode&0x20)
		Prn_SetAlignment(2);
	if(mode&0x40)
		PrnReverseSet(1);
	else 
		PrnReverseSet(0);
	special_mode = mode;
	return 0;
}
int Dll_PrnGetMode(unsigned char *mode)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	*mode = special_mode;
	return 0;
}


#define SENDPACK_LEN (4*1024)//160     //160 8K

struct packet_struct  {
	int packet_line_num;
	char *packet_base;
	int packet_len;
};

static struct packet_struct g_packet_struct[(MAX_DOT_LINE+48)*48/SENDPACK_LEN+1]; 

static int fill_up_packet_for_k21(const unsigned char *dot_buf, int max_len) 
{
		int cur_len = 0;
		int begin_addr = 0;
		int len_index = 0;
		int packet_base_addr = 0;
		int max_packet_num = 0;
		g_packet_struct[0].packet_line_num = 0;
		while(max_len > len_index) {
			if((cur_len / SENDPACK_LEN)>max_packet_num) {//beyond SENDPACK_LEN a litter
				g_packet_struct[max_packet_num].packet_base = &dot_buf[begin_addr];
				g_packet_struct[max_packet_num].packet_len = len_index -begin_addr ;
				max_packet_num++;
				begin_addr = len_index;
				
				//printf("----max_packet_num:0x%x, begin_addr:0x%x len :0x%x\r\n", max_packet_num, begin_addr, len_index);
				g_packet_struct[max_packet_num].packet_line_num = 0;

			} else {
				//printf("len_index:0x%x len :0x%x line_num = %x\r\n", len_index, dot_buf[len_index],g_packet_struct[max_packet_num].packet_line_num);
				cur_len += dot_buf[len_index];
				len_index += dot_buf[len_index] + 1;
				g_packet_struct[max_packet_num].packet_line_num++;
				
			}
		}

		g_packet_struct[max_packet_num].packet_base = &dot_buf[begin_addr];
		g_packet_struct[max_packet_num].packet_len = len_index -begin_addr ;
		max_packet_num++;
		begin_addr = len_index;
		//LOGD("1111max_packet_num:0x%x, begin_addr:0x%x len :0x%x\r\n", max_packet_num, begin_addr, len_index);
		
		return max_packet_num;
}


unsigned char DotBufToK21[550*1024];

#define MAX_PRINT_LINE_NUM (550*1024/48)



unsigned long dotbuf_index = 0;
unsigned long g_Total_PrnDataLen = 0;
#define MAX_SEND_ONETIME 	1024


int Prn_FlowControl_Handle()
{
	int iret = 0;
	ushort wlen = 0x0;
	uchar finished_flag = 0;
	unsigned long send_len = 0;
	int packet_num = 0;
	int retry_count = 10;

	//LOGD("%s %d %d %d",__FUNCTION__,__LINE__,dotbuf_index,g_Total_PrnDataLen);
	while (1) {

		if (dotbuf_index + MAX_SEND_ONETIME > g_Total_PrnDataLen) {
			send_len = g_Total_PrnDataLen - dotbuf_index;
			finished_flag = 1;
			//LOGD("%s %d %d %d",__FUNCTION__,__LINE__,dotbuf_index,g_Total_PrnDataLen);
			//LOGD("g_Total_PrnDataLen :%d dotbuf_index:%d", g_Total_PrnDataLen, dotbuf_index);
		}
		else
			send_len = MAX_SEND_ONETIME;
		
		//LOGD("dotbuf_index :%d send_len:%d", dotbuf_index, send_len);
		
		retry_count = 1;
		while (retry_count--) {
			iret = Prn_PushFifo(0,&DotBufToK21[dotbuf_index],send_len);
			
			if (iret == 0) {
				break;
			}
			else
				return iret;
	
		}
		dotbuf_index += send_len;
		packet_num++;
		if (finished_flag)
			break;

	}

	return 0;
}



int DLL_PrnInit()
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
    g_iPrintStrFlag = 0;
    k_CurDotLine = 0;
    k_FontDotLine = 0;
    k_AllDotLine = 0;
    k_LeftIndent = 0;
    g_iAsciiFontHeight = 24;
    g_iExternFontHeight = 24;
    g_iAsciiFontWidthZoom = 0;
    g_iAsciiFontHeightZoom = 0;
    g_iExternFontWidthZoom = 0;
    g_iExternFontHeightZoom = 0;
    k_CharSpace = 0;
    k_LineSpace = 0;
	g_PrnMode = 0;
	g_PrnModeFlag = 0;
	g_widthNum = 0;
	g_Spacewidth = 0;
	g_FirstSpaceFlag = 0;
	g_CharSpace_C = 0;

    k_CurDotColumn = 0;
    k_CurHeight = 16;
	special_mode = 0;
	//LOGD("%s %d",__FUNCTION__,__LINE__);
	//if(k_DotBuf==NULL) k_DotBuf = malloc_Array2D(MAX_DOT_LINE,48);
    memset(&k_DotBuf[0][0], 0, MAX_DOT_LINE*48);  // ???????????
    k_PrnStatus = 0;
    if (0 == g_iHaveFontFlag)
    {
    	InitFontLib();
    }
    return 0;
}

int DLL_PrnSetSpace(BYTE x, BYTE y)
{
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	k_CharSpace = x;
    	k_LineSpace = y;
	return 0;

}

int DLL_PrnSetFont(BYTE asciiFontHeight, BYTE extendFontHeight, BYTE zoom)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
    if ((16!=asciiFontHeight) && (24!=asciiFontHeight))
    {
        return PRN_SETFONTERR;
    }
    if ((16!=extendFontHeight) && (24!=extendFontHeight))
    {
        return PRN_SETFONTERR;
    }
    if (0 != (zoom&0xcc))
    {
        return PRN_SETFONTERR;
    }
    g_iAsciiFontWidthZoom = 0;
    g_iAsciiFontHeightZoom = 0;
    g_iExternFontWidthZoom = 0;
    g_iExternFontHeightZoom = 0;
    if (0 != (zoom&0x01))
    {
        g_iAsciiFontWidthZoom = 1;
    }
    if (0 != (zoom&0x02))
    {
        g_iAsciiFontHeightZoom = 1;
    }
    if (0 != (zoom&0x10))
    {
        g_iExternFontWidthZoom = 1;
    }
    if (0 != (zoom&0x20))
    {
        g_iExternFontHeightZoom = 1;
    }
    g_iAsciiFontHeight = asciiFontHeight;
    g_iExternFontHeight = extendFontHeight;

	return 0;
}

int DLL_PrnGetFont(BYTE *asciiFontHeight, BYTE *extendFontHeight, BYTE *zoom)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
    *asciiFontHeight = g_iAsciiFontHeight;
    *extendFontHeight = g_iExternFontHeight;
    *zoom = 0;
    if (1 == g_iAsciiFontWidthZoom)
    {
        *zoom |= 0x01;
    }
    if (1 == g_iAsciiFontHeightZoom)
    {
        *zoom |= 0x02;
    }
    if (1 == g_iExternFontWidthZoom)
    {
        *zoom |= 0x10;
    }
    if (1 == g_iExternFontHeightZoom)
    {
        *zoom |= 0x20;
    }
    return 0;
}

int DLL_PrnStep(int pixel)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
    if ((!pixel) || ((k_CurDotLine+pixel)<0))
    {
        return;
    }
    if (k_PrnStatus == PRN_BUFFOVERFLOW)
    {
        return;
    }
    if (k_CurDotColumn != k_LeftIndent)
    {
        if (s_NewLine())
        {
            return;
        }
    }
    if ((k_CurDotLine+pixel) > MAX_DOT_LINE)
    {
        k_PrnStatus = PRN_BUFFOVERFLOW;
        pixel = MAX_DOT_LINE - k_CurDotLine;
    }
    k_CurDotLine += pixel;
}

int DLL_PrnStr(const char *str)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	int ret=0;
	int i = 0;

	uchar unibuf[6000];
	uchar tempbuf[6000];

	memset(unibuf,0,sizeof(unibuf));
	memset(tempbuf,0,sizeof(tempbuf));

	if(g_iUnicodeFontFlag==0)
	{

		Str_UnicodeToGB2312(str,unibuf,ustrlen((char *)str));
		return s_PrnStr(unibuf);

		//return s_PrnStr(str);

	}
	else
	{
		//printf("utf8_to_unicode,strlen(str) is %d\r\n",strlen(str));
		//for(i = 0 ; i < strlen(str); i++) printf("%02x ",str[i]);
		utf8_to_unicode(str,tempbuf,strlen(str));
		//printf("utf8_to_unicode\r\n");
		//for(i = 0 ; i < strlen(str); i++) printf("%02x ",tempbuf[i]);
		return s_PrnUnicodeStr(tempbuf);
	}


	return ret;
}

int DLL_PrnLogo(BYTE *logo)
{
    WORD wWidthBit, wHeight, wWidthByte;
    BYTE byZip, *pbyPrnDot, *pbyLogoDot;
    DWORD dwTemp, i, j;
	int shift;
	BYTE *Font, *tmPtr, ch, ch1;

    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;

    wWidthBit = logo[0]*256+logo[1];
    wHeight = logo[2]*256+logo[3];

    if ((wWidthBit > 384) || (wHeight>500))
    {
        return PRN_DATAERR;
    }

    wWidthByte = (wWidthBit+7)/8;
    byZip = logo[4];

	#if 1
    if (0 != byZip)
    { 
        if (0 != Decompress(&logo[7],gDecompressBuf,logo[5]*256+logo[6],&dwTemp))
        {
            return PRN_FAULT;
        }
    }
	#endif
    if (k_LeftIndent != k_CurDotColumn)
    {
        (void)s_NewLine();
    }

	pbyPrnDot = k_DotBuf[k_CurDotLine]+k_CurDotColumn/8;

    if (0 == byZip)
    {
        pbyLogoDot = logo+5;
    }
    else
    {
        pbyLogoDot = gDecompressBuf;
    }
	Font = pbyLogoDot;
	shift = k_CurDotColumn%8;

	for(i=0; i<wHeight; i++)
	{
		if ((i+k_CurDotLine) >= MAX_DOT_LINE)
		{
			break;
		}
		tmPtr = pbyPrnDot;
		for (j=0; j<wWidthByte; j++)
		{
			ch = *Font++;
			ch1 = ch>>shift;
			*tmPtr++ |= ch1;
			ch <<= (8-shift);
			if(ch)
				*tmPtr = ch;
		}
		pbyPrnDot += 48;
		
	}

	//????0??k_CurDotColumn???????
	for (i=0; i<wHeight+1; i++)
	{
		for (j=0; j<k_CurDotColumn/8; j++)
		{
			k_DotBuf[k_CurDotLine+i][j] = 0;
		}

		if (shift)
		{//
			ch = 0xff >> shift;
			k_DotBuf[k_CurDotLine+i][k_CurDotColumn/8] &= ch;
		}
	}
    k_CurDotLine += i;
    g_iPrintStrFlag = 1;

    return PRN_OK;
}


int DLL_PrnStart()
{
    register int  j, k;
    register BYTE *tPtr, ch;
    int fd;
    int i, count;
    int ret;
	BYTE val = 0x1;
	int packet_len = 0;
	int DotBufIndex = 0;
	unsigned char LineLen;
	int max_packet_num = 0;
	int cur_len = 0;
	int err = 0;
	int cur_prn_line_num = 0;
	int remain_line_num = 0;
	int start_prn_line = 0;
	int has_prn_finished = 0;
	dotbuf_index = 0;
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;

    if(k_CurDotColumn!=k_LeftIndent)
            (void)s_NewLine();
    if(!k_FontDotLine)
            k_FontDotLine=k_CurDotLine;
    k_AllDotLine=k_CurDotLine;
    printf("k_AllDotLine = %d", k_AllDotLine);
	printf("k_CurDotLine = %d", k_CurDotLine);


	
	for(i = 0; i<k_AllDotLine;i++)
	{
		for(LineLen=48;LineLen>0;LineLen--)
		{
			if(k_DotBuf[i][LineLen-1]==0) continue;
			else break;
		}

		DotBufToK21[DotBufIndex++] = LineLen;
		memcpy(&DotBufToK21[DotBufIndex],k_DotBuf[i],LineLen);
		DotBufIndex += LineLen;	
	}
	printf("DotBufIndex:%d",DotBufIndex);
	
	ret = Prn_SetLine(k_AllDotLine);
		
	g_Total_PrnDataLen = DotBufIndex;
	if (DotBufIndex >= 5120) {
		Prn_PushFifo(0, &DotBufToK21[0], 5120);
		dotbuf_index = 	5120;
	} else {
		Prn_PushFifo(0,&DotBufToK21[0], DotBufIndex);
		dotbuf_index = 	DotBufIndex;
	}
	ret = 0;//Prn_SetGray(k_CurGray);
	if (ret != 0 ) {//maybe k21 loss this packet,reissue one time
		printf("reissue the packet that starts k21 print.\r\n");
	}
	Prn_Start();
	


	if (DotBufIndex >= 5120) 
	{
		//�ߴ�ӡ�߷�������
		ret = Prn_FlowControl_Handle();
	}
	if(ret==0)
	{
		ret = wait_prn_finish();
		if(ret) ret+=10;
	}
	
    k_CurDotLine = 0;
    k_FontDotLine = 0;
    k_AllDotLine = 0;
    k_CurDotColumn = 0;
    g_iPrintStrFlag = 0;
    memset(&k_DotBuf[0][0], 0, MAX_DOT_LINE*48); 

	return ret;
}
int DLL_PrnGetLine()
{
	register int  j, k;
    register BYTE *tPtr, ch;
    int fd;
    int i, count;
    int ret;
	BYTE val = 0x1;
	int packet_len = 0;
	int DotBufIndex = 0;
	unsigned char LineLen;
	int max_packet_num = 0;
	int cur_len = 0;
	int err = 0;
	int cur_prn_line_num = 0;
	int remain_line_num = 0;
	int start_prn_line = 0;
	int has_prn_finished = 0;
	dotbuf_index = 0;
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;

    if(k_CurDotColumn!=k_LeftIndent)
            (void)s_NewLine();
    if(!k_FontDotLine)
            k_FontDotLine=k_CurDotLine;
    k_AllDotLine=k_CurDotLine;
    //LOGE("k_AllDotLine = %d", k_AllDotLine);
	//LOGE("k_CurDotLine = %d", k_CurDotLine);


	
	for(i = 0; i<k_AllDotLine;i++)
	{
		for(LineLen=48;LineLen>0;LineLen--)
		{
			if(k_DotBuf[i][LineLen-1]==0) continue;
			else break;
		}

		DotBufToK21[DotBufIndex++] = LineLen;
		memcpy(&DotBufToK21[DotBufIndex],k_DotBuf[i],LineLen);
		DotBufIndex += LineLen;	
	}
	//LOGD("DotBufIndex:%d",DotBufIndex);
	
	g_Total_PrnDataLen = DotBufIndex;
	
	return k_AllDotLine;
	
}
int DLL_PrnGetDotLen()
{
	return g_Total_PrnDataLen;
}
int DLL_PrnGetTranBuff(char *buff)
{
	memcpy(buff,DotBufToK21,g_Total_PrnDataLen);
	return g_Total_PrnDataLen;
}

int DLL_PrnSetLeftIndent(int x)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
    if (x > 336)
    {
        x = 336;
    }
    /*else if (x < 0)
    {
        return;
    }*/
    if (k_LeftIndent != k_CurDotColumn)
    {
        (void)s_NewLine();
    }
    k_LeftIndent = x;
    k_CurDotColumn = x;
}

int DLL_PrnSetGray(BYTE nLevel)
{
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	
	if (nLevel > 7)
		nLevel = 7;
 	k_CurGray = nLevel;	
	return SUCCESS;
}


int DLL_PrnCheckStatus()
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	
	//call printer library
	
	return iRet;	
}

int DLL_PrnSetInvert(BYTE fOn)
{
	
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	
	//call printer library
	
	return iRet;	
}

int DLL_PrnGetTemperature(int *tempValue)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;

	wLen = 0;
	wCmd = MAKEWORD(PRINTER_MODULE,PRINTER_GETTEMP);
	
	Crc16OpenCom();
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
    	*tempValue = iRet;
    	iRet = 0;
	
	Crc16CloseCom();
	return iRet;
}

int DLL_PrnSetTemperature(int tempValue)
{
	int iRet=-1;
	unsigned short wLen=0,wCmd=0;

	memset(g_abySendBuff,0,sizeof(g_abySendBuff));
	memset(g_abyRecvBuff,0,sizeof(g_abyRecvBuff));
	
    	if(authCheck(CurAppNo, MODULE_TYPE_PRINTER))
    		return App_Permissions_Denial;
	Crc16OpenCom();

	g_abySendBuff[0] = tempValue/256;
	g_abySendBuff[1] = tempValue%256;
	
	wLen = 2;
	wCmd = MAKEWORD(PRINTER_MODULE,PRINTER_SETTEMP);
	iRet = Crc16SendPacket(g_abySendBuff,wLen,wCmd);
	if (0 != iRet)
	{
		Crc16CloseCom();
		return Packet_Send_Err;
	}

	iRet = Crc16RecvPacket(g_abyRecvBuff, &wLen, &wCmd, RECV_TIMEOUT);
	if (0!=iRet)
	{
		Crc16CloseCom();
		return Packet_Recv_Err;
	}
	iRet = 0-MAKEWORD(g_abyRecvBuff[5],g_abyRecvBuff[6]);
	
	Crc16CloseCom();
	return iRet;
}
