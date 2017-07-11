#include "updatemsgthread.h"

#include <stdio.h>
#include <sys/ipc.h> //包含ftok
#include <sys/msg.h>
#include <sys/types.h>

#define MAX_SIZE 1024//512
static bool isVerify = false;
static int iPinMaxLenCount = 0;

typedef struct
{
    long mtype ;
    char mdata[MAX_SIZE] ;
} msg_t;
unsigned char recvBuf[MAX_SIZE];
unsigned char sendBuf[MAX_SIZE];

UpdateMsgThread::UpdateMsgThread(QObject *parent) :
    QThread(parent)
{
}

UpdateMsgThread::~UpdateMsgThread()
{

}



void UpdateMsgThread::run()
{
    int msgid = -1;
    msgid = msgget((key_t)128L, 0666 | IPC_CREAT);
    printf("msgid is %d\r\n",msgid);
    if (msgid < 0)
    {
        puts("Create msg queue error!\n") ;
        exit(0) ;
    }

    server(msgid);//, msgid) ;
}


void  UpdateMsgThread::server(int readfd)//, int writefd)
{
    msg_t* msgPtr ;
    ssize_t  recvBytes ;
    int type=0,subtype=0;
    int i = 0;
    unsigned char prompt[256];
    unsigned char amount[15],tempAmount[15];
    unsigned char font[4];
    unsigned char background[3];
    int promptLen=0;
    QString strAmount,strPrompt;
    int pinlen=0;
    int pwdlen = 0;
    int count = 0;


    for ( ; ; )
    {
        //从消息队列中读取来自客户的数据
        msgPtr = (msg_t*)malloc(MAX_SIZE + sizeof(long)) ;
        recvBytes = msgrcv(readfd, msgPtr, MAX_SIZE, 3, 0) ; //阻塞读
        printf("pinentry recvBytes is %d\r\n",recvBytes);
        if (recvBytes <= 0)
        {
            puts("pathname missing") ;
            continue ;
        }
        msgPtr->mdata[recvBytes] = '\0' ;

        //分析消息，提取客户端数据
        memset(recvBuf,0,sizeof(recvBuf));
        memcpy(recvBuf,msgPtr->mdata,recvBytes);
        printf("pinentry recv data is:\r\n");
        for(i = 0 ; i < recvBytes; i++) printf("%02x ",recvBuf[i]);
        printf("\r\n");

        type = recvBuf[0];
        subtype = recvBuf[1];
        memset(prompt,0,sizeof(prompt));
        memset(amount,0,sizeof(amount));
        memset(tempAmount,0,sizeof(tempAmount));
        if(type == 1 && subtype == 0)//pin prompt show
        {
            printf("show PIN entry dialog\r\n");
           showPinEntrySignal(1);

            memcpy(amount,recvBuf+2,14);
            promptLen = recvBytes-4-3-14-2;
            memcpy(prompt,recvBuf+2+14,promptLen);
            memcpy(font,recvBuf+2+14+promptLen,4);
            memcpy(background,recvBuf+2+14+promptLen+4,3);

            printf("prompt len is %d\r\n",promptLen);
             strPrompt.sprintf("%s",prompt);
             if(!strPrompt.isEmpty()) refreshPromptSignal(0,strPrompt);
             if(memcmp(amount,tempAmount,14))
             {
                 strAmount.sprintf("%s",amount);
                 refreshPromptSignal(1,strAmount);
             }
             else
             {
                 refreshPromptSignal(1,"");
             }

            itemCtrlSignal(0,0);//0:control item enable or disable

          /*  byte font[] = new byte[4];
            byte background[] = new byte [3];
            System.arraycopy(data.getByteArray("font"), 0, font, 0,4);
            System.arraycopy(data.getByteArray("background"), 0, background, 0,3);

            int r = (int)((font[0] + 256) % 256);
            int g = (int)((font[1] + 256) % 256);
            int b = (int)((font[2] + 256) % 256);
            int size = (int)((font[3] + 256) % 256);

            textPrompt.setTextColor(Color.rgb(r,g,b));
            textPrompt.setTextSize(size);
            textAmount.setTextColor(Color.rgb(r,g,b));
            textAmount.setTextSize(size);

            r=(int)((background[0] + 256) % 256);
            g=(int)((background[1] + 256) % 256);
            b=(int)((background[2] + 256) % 256);
            ((Activity) mContext).getWindow().getDecorView().setBackgroundColor(Color.rgb(r,g,b));*/

        }
        if(type == 1 && subtype == 7)//pin entry show
        {
            pinlen = recvBuf[2];
            printf("pinentry pinlen is %d\r\n",pinlen);

            if(pinlen == 0) itemCtrlSignal(1,pinlen);//1: control edit item show text
            else if(pinlen == 100)//退出PIN显示框
            {
                itemCtrlSignal(1,pinlen);
                iPinMaxLenCount = 0;
                isVerify = false;
                showPinEntrySignal(0);
            }
            else itemCtrlSignal(1,pinlen);
        }
        if((type == 2) && (subtype>=1&&subtype<=4))//pwd prompt show
        {
            showPinEntrySignal(1);
             //textPrompt.setTextColor(Color.rgb(0,0,0));
            //textPrompt.setTextSize(20);
            //textAmount.setVisibility(View.INVISIBLE);
            //((Activity) mContext).getWindow().getDecorView().setBackgroundColor(Color.rgb(255,255,255));
              itemCtrlSignal(0,1);//0:control item enable or disable

            if(subtype == 1) refreshPromptSignal(0,"Plesae Enter Administrator A's Password:");
            if(subtype == 2) refreshPromptSignal(0,"Please Enter Administrator A's New password:");
            if(subtype == 3) refreshPromptSignal(0,"Plesae Enter Administrator B's Password:");
            if(subtype == 4) refreshPromptSignal(0,"Please Enter Administrator B's New password:");

            if(subtype == 2 || subtype == 4)
            {
                isVerify = false;
                refreshPromptSignal(2,"Confirm:");
                itemCtrlSignal(0,2);//0:control item enable or disable

                /*textConfirm.setVisibility(View.VISIBLE);
                editConfirm.setVisibility(View.VISIBLE);*/
            }
            else if(subtype == 1 || subtype == 3)
            {
                isVerify = true;
                itemCtrlSignal(0,3);//0:control item enable or disable

               /* textConfirm.setVisibility(View.INVISIBLE);
                editConfirm.setVisibility(View.INVISIBLE);*/

            }
        }
        if(type == 2 && subtype == 8)//pwd entry show
        {
             pwdlen = recvBuf[2];
             count = recvBuf[3];
             printf( "pwdlen is :%d,count is %d\r\n",pwdlen,count);

            if(pwdlen == 0 && count == 0) {itemCtrlSignal(2,0); iPinMaxLenCount = 0;}//editPin.setText("");
            else if(pwdlen == 0 && count == 1) {itemCtrlSignal(3,0); iPinMaxLenCount = 0;}//editConfirm.setText("");
            else if(pwdlen == 100 && (count == 2 || count == 1))//退出PWD显示框
            {
                //editConfirm.setText("");
                //editPin.setText("");
                itemCtrlSignal(2,0);
                itemCtrlSignal(3,0);

                iPinMaxLenCount = 0;
                isVerify = false;
                showPinEntrySignal(0);
//                moveTaskToBack(true);
            }
            else if (pwdlen == 100 && count == 0)
            {
                /*editConfirm.setFocusable(true);
                editConfirm.setFocusableInTouchMode(true);
                editConfirm.requestFocus();*/
                itemCtrlSignal(0,4);
                iPinMaxLenCount = 0;
            }
            else if(pwdlen >= 1 && pwdlen <= 8)
            {
                if(pwdlen == 8)
                {
                    iPinMaxLenCount ++;
                }
                if(iPinMaxLenCount < 2)
                {
                    if(isVerify)  itemCtrlSignal(4,pwdlen);//setTextShow(0,pwdlen);//editPin.append("*");
                    else
                    {
                        if(count < 1) itemCtrlSignal(4,pwdlen);//setTextShow(0,pwdlen);//editPin.append("*");
                        else itemCtrlSignal(5,pwdlen);//setTextShow(1,pwdlen);//editConfirm.append("*");
                    }
                }
            }
        }

/*

            msgPtr->mtype = 3 ;
         memcpy(msgPtr->mdata,sendBuf,wLen+2);

        if (msgsnd(writefd, msgPtr, wLen+2, IPC_NOWAIT) == -1)
        {
            puts("Send Error!");
            exit(0);
        }*/

    }//for()
}



