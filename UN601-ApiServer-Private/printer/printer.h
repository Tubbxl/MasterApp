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


#define PRESTEP_NUM         21	// ��ӡǰԤ��ֽ�����

#define DEFAULT_PRINTER_GRAY  5            // Ĭ�ϵĴ�ӡ�Ҷ�ֵ

//*******************************************************************************
#define MAX_HEAT_DOT		64
#define MAX_HEAT_BLOCK		(384/MAX_HEAT_DOT)
#define STEP_TIME60         2016//2016  // 2660,1.33MS,4V;  2500,1.25ms,4.5v;  2232,1.12ms,5v;  2118,1.06ms,5.5v;  2016,1.01ms,6.0v;  1838,0.92ms,6.5v
#define STEP_TIME50         2232//2232  // 2660,1.33MS,4V;  2500,1.25ms,4.5v;  2232,1.12ms,5v;  2118,1.06ms,5.5v;  2016,1.01ms,6.0v;  1838,0.92ms,6.5v
#define HEAT_TIME           3427//4147	//3427 // 6480,3.24ms,4v;  5120,2.56MS,4.5V;  4147,2.07ms,5v;  3427,1.71ms,5.5v;  2880,1.44ms,6v
#define HEAT_SPLIT          3				// ������Ⱦ���Ϊ���ٴν���
#define TEMPERATURE_FORBID  34//81				// ��ֹ��ӡ�¶�
#define PRINT_START_TEMPERATURE_FORBID  27//74  // ��ֹ��ӡ���¶�
#define MAX_MANUAL_COUNTER	6000			// �˹���ֽ�����manual
#define MAX_THICK_LEVEL		7				// ��ӡ�����ϸ���ȼ�

#define HEAT_FACTOR    9

#define TIMER_SPEED_FACTOR1  257
#define TIMER_SPEED_FACTOR2  337



#define TIMER_MOTOR_FACTOR1   97
#define TIMER_MOTOR_FACTOR2  125


#define MAX_PRINTER_SPEED_LEVEL     23      // ��ӡ������ٶȵȼ�
#define DEFAULT_PRINTER_SPEED       17

/******   ��ӡ�¶���AD����ֵ��ϵ3.3V,6.8K**************************/
typedef struct
{
    int Temperature;        // ��Ӧ���¶�
    int ADValue;            // ����ֵ
}ADConvT;

/************   ��ӡ����ṹ             **************************/
typedef struct
{
    int   CurOp;                          // 1=���ڴ�ӡ�û���; 0=no
    int   DataValid;                      // 1=��������Ч,�ȴ���ӡ; 0=no
    int   Temperature;                    // ��ʾ�����¶�
    DWORD StepTime;                       // ��ʾ�û����ӡʱ������ʱ��
    BYTE  SendDot[MAX_HEAT_BLOCK][48];    // �洢��������ĵ�[6][48]
    DWORD DotCount[MAX_HEAT_BLOCK];       // black dots number array of per line
    DWORD DotHeatTime[MAX_HEAT_BLOCK];    // �洢����ʱ��,������ʱ�ݼ�
    DWORD DotHeatTimeSlit[MAX_HEAT_BLOCK];// �洢�ִκ�ÿ�μ���ʱ��=�ʼ��k_DotHeatTime/HEAT_SPLIT
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

