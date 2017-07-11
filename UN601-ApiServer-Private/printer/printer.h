#ifndef _PRINTER_H_
#define _PRINTER_H_


#define QUEUE_PRINT_INIT              0xf0
#define QUEUE_PRINT_SET_FONT          0x01
#define QUEUE_PRINT_SET_SPEED         0x02
#define QUEUE_PRINT_SET_GRAY          0x03
#define QUEUE_PRINT_SET_STEP          0x04
#define QUEUE_PRINT_SET_SPACE         0x05
#define QUEUE_PRINT_SET_LEFT_INDENT   0x06
#define QUEUE_PRINT_STRING            0x07
#define QUEUE_PRINT_LOGO              0x08
#define QUEUE_PRINT_START             0xf1

#define TICKET_QUEUE_FILE_NAME    "_QueueTicket_"

#define TICKET_QUEUE_FILE_ATTR    "\xff\x0f"
#define QUEUE_FILE_LOCK_FLAG      "\x1c\x1e\x1d\x1b"



#define PAGE_LINES       200
#define MAX_DOT_LINE     5000

#define CHINESE_LAN	     1
#define ENGLISH_LAN      0


#define PRESTEP_NUM         21	// 打印前预走纸最大步数

#define DEFAULT_PRINTER_GRAY  5            // 默认的打印灰度值

//*******************************************************************************
#define MAX_HEAT_DOT		64
#define MAX_HEAT_BLOCK		(384/MAX_HEAT_DOT)
#define STEP_TIME60         2016//2016  // 2660,1.33MS,4V;  2500,1.25ms,4.5v;  2232,1.12ms,5v;  2118,1.06ms,5.5v;  2016,1.01ms,6.0v;  1838,0.92ms,6.5v
#define STEP_TIME50         2232//2232  // 2660,1.33MS,4V;  2500,1.25ms,4.5v;  2232,1.12ms,5v;  2118,1.06ms,5.5v;  2016,1.01ms,6.0v;  1838,0.92ms,6.5v
#define HEAT_TIME           3427//4147	//3427 // 6480,3.24ms,4v;  5120,2.56MS,4.5V;  4147,2.07ms,5v;  3427,1.71ms,5.5v;  2880,1.44ms,6v
#define HEAT_SPLIT          3				// 定义加热均分为多少次进行
#define TEMPERATURE_FORBID  34//81				// 禁止打印温度
#define PRINT_START_TEMPERATURE_FORBID  27//74  // 禁止打印的温度
#define MAX_MANUAL_COUNTER	6000			// 人工走纸最大步数manual
#define MAX_THICK_LEVEL		7				// 打印字体粗细最大等级

#define HEAT_FACTOR    9

#define TIMER_SPEED_FACTOR1  257
#define TIMER_SPEED_FACTOR2  337



#define TIMER_MOTOR_FACTOR1   97
#define TIMER_MOTOR_FACTOR2  125


#define MAX_PRINTER_SPEED_LEVEL     23      // 打印机最高速度等级
#define DEFAULT_PRINTER_SPEED       17

/******   打印温度与AD采样值关系3.3V,6.8K**************************/
typedef struct
{
    int Temperature;        // 对应的温度
    int ADValue;            // 采样值
}ADConvT;

/************   打印缓冲结构             **************************/
typedef struct
{
    int   CurOp;                          // 1=正在打印该缓冲; 0=no
    int   DataValid;                      // 1=该数据有效,等待打印; 0=no
    int   Temperature;                    // 表示采样温度
    DWORD StepTime;                       // 表示该缓冲打印时步进的时间
    BYTE  SendDot[MAX_HEAT_BLOCK][48];    // 存储即将送入的点[6][48]
    DWORD DotCount[MAX_HEAT_BLOCK];       // black dots number array of per line
    DWORD DotHeatTime[MAX_HEAT_BLOCK];    // 存储加热时间,供加热时递减
    DWORD DotHeatTimeSlit[MAX_HEAT_BLOCK];// 存储分次后每次加热时间=最开始的k_DotHeatTime/HEAT_SPLIT
}HeatUnit;

int s_GetPrnFontDot(const BYTE *str, BYTE *OutDot, int *DotWidth,int *DotHeight, int *DotNibble);
int s_CheckPaper(void);
void s_StopPrn(void);
void s_MotorTimer(void);
void s_HeatTimer(void);
int s_ReadHeadTemp(void);
void s_LatchData(BYTE *buf);
void s_LoadData(int cur);
void s_SetPrnTime(int cur);
void TestDelayMs(int ms);


BYTE s_CharWith(const BYTE *str);
BYTE s_BaiticCharWith(const BYTE *str);

#endif

