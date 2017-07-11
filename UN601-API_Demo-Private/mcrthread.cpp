#include "mcrthread.h"
extern "C" {
#include "dll_api.h"
}
MCRthread::MCRthread()
{
    stopflag = false;
}

void MCRthread::run()
{
    int iRet = -1;
    unsigned char track1Len[1];
    unsigned char track2Len[1];
    unsigned char track3Len[1];
    unsigned char track1[256];
    unsigned char track2[256];
    unsigned char track3[256];
    unsigned char verson[32]={0};
    QString str;
     static int totalCount = 0;
     static int successCount = 0;

     emit  sendMessage("Read Mcr Verson");
     iRet = DLL_McrReadVersion(verson);
     if(iRet == 0)
     {
         str.clear();
         str.sprintf("Mcr Verson:%s \n",verson);
        emit  sendMessage(str);
     }

    iRet = DLL_McrOpen();
    str.clear();
    str.sprintf("MCR open failed, return value is: %d\n",iRet);
    if(iRet!=0) { emit sendMessage(str); return;}
    else {emit sendMessage("MCR open success!\n");}



    iRet = DLL_McrReset();
    str.clear();
    str.sprintf("MCR reset failed, return value is: %d\n",iRet);
    if(iRet!=0) {emit sendMessage(str); DLL_McrClose(); return;}
    else
    {emit sendMessage("MCR reset success!\n");}

   emit  sendMessage("Please swipe card...\n");
    while(1)
    {
        iRet = DLL_McrCheck();
        if(iRet == 0)
        {
            break;
        }
        if(stopflag)
        {
            DLL_McrClose();
            return;
        }
    }
    totalCount++;
    str.clear();
    str.sprintf("swipe count:%d\n",totalCount);
    emit sendMessage(str);
   emit  sendMessage("Detect card swiped\n");
    memset(track1,0,sizeof(track1));
    memset(track2,0,sizeof(track2));
    memset(track3,0,sizeof(track3));
    iRet = DLL_McrRead(track1Len,track1,track2Len,track2,track3Len,track3);
    if(iRet > 0)
    {
        successCount++;
        str.clear();
        str.sprintf("success count:%d\n",successCount);
        emit sendMessage(str);

        str.clear();
        QString string;
        string.clear();
        if(iRet <= 7){
            if((iRet & 0x01) == 0x01)
            {
                str.clear();
                //string = getStringFromUnsignedChar(track1,track1Len[0]);

                str.sprintf("track1:%s",track1);
                str.append(string);
                str+="\n";
               emit  sendMessage(str);
            }
            if((iRet & 0x02) == 0x02)
             {
                str.clear();
                str.sprintf("track2:%s",track2);
                emit sendMessage(str);
              }
            if((iRet & 0x04) == 0x04)
            {
                str.clear();
                //string = getStringFromUnsignedChar(track3,track3Len[0]);
                str.sprintf("track3:%s",track3);
                str.append(string);
                str+="\n";
                emit sendMessage(str);
            }
        }else{
            string = "McrRead data error\n";
            str += string;
            emit sendMessage(str);
        }

        DLL_McrClose();
        emit sendMessage("\nMCR close, Test Success!!!\n");
    }
    else
    {
        str.clear();
        str.sprintf("success count:%d\n",successCount);
        emit sendMessage(str);
        emit sendMessage("Read card failed!!!\n");
        DLL_McrClose();
    }
}

void MCRthread::setstop()
{
    stopflag =  true;
}

