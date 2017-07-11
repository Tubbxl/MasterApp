#include "adminslogin.h"

#include "mappdefines.h"

#include <stdio.h>

extern "C" {
 #include "include/dll_api.h"
#include "include/SSLComm.h"
}

AdminsLogin::AdminsLogin(unsigned char funcType,  QString strFuncType, unsigned char pwdType, QObject *parent) :
    QThread(parent)
{
    mAdminsFuncType = funcType;
    mStrFuncType = strFuncType;
    mPwdType = pwdType;
}


AdminsLogin::~AdminsLogin()
{

}

void AdminsLogin::refreshPromptAndExit(QString strPrompt)
{
     refreshPromptSignal(strPrompt);
    sleep(2);
     closePromptSignal();
}

int AdminsLogin::adminsPwdLogin()
{
    int iRet = -1;
    qDebug("AdminsPwdLogin---DLL_PciPwdInput PWD_TYPE_ADMIN_A" );
    iRet = DLL_PciPwdInput(PWD_TYPE_ADMIN_A, FUNC_TYPE_VERIFY_PWD);

   qDebug("adminsLogin---DLL_PciPwdInput PWD_TYPE_ADMIN_A iRet: [%d]", iRet);
    if(PWD_OK != iRet)
    {
        return iRet;
    }

   qDebug("AdminsPwdLogin---DLL_PciPwdInput PWD_TYPE_ADMIN_B" );

    iRet = DLL_PciPwdInput(PWD_TYPE_ADMIN_B, FUNC_TYPE_VERIFY_PWD);
  qDebug("adminsLogin---DLL_PciPwdInput PWD_TYPE_ADMIN_B iRet: [%d]", iRet);

    return iRet;
}

int AdminsLogin::adminChangePwd(unsigned char pwdType)
{
    int iRet = -1;

    qDebug("AdminChangePwd---DLL_PciPwdInput mPwdType: [%d]", pwdType);

    iRet = DLL_PciPwdInput(pwdType, FUNC_TYPE_VERIFY_PWD);
    if((PWD_OK != iRet) &&
      (PWD_VerifyDefaultOK != iRet))
    {
        return iRet;
    }

    iRet = DLL_PciPwdInput(pwdType, FUNC_TYPE_CHANGE_PWD);


    return iRet;
}

int AdminsLogin::verifyAdminsPwd()
{
    int schipStat = 0;

      schipStat = DLL_PciCheckSecurityChipStatus();

    int iRet = -1;

    if (SCHIP_CHANGED_ADMINA_PWD != (schipStat & SCHIP_CHANGED_ADMINA_PWD)) {

       qDebug("ChangePwd PWD_TYPE_ADMIN_A");
        iRet = adminChangePwd(PWD_TYPE_ADMIN_A);
       qDebug("adminChangePwd A iRet: [%d]", iRet);
        if (0 != iRet) {

            return iRet;
        }

    }

    if (SCHIP_CHANGED_ADMINB_PWD != (schipStat & SCHIP_CHANGED_ADMINB_PWD)) {

        qDebug("ChangePwd PWD_TYPE_ADMIN_B");
        iRet = adminChangePwd(PWD_TYPE_ADMIN_B);
        qDebug("adminChangePwd B iRet: [%d]", iRet);
        if (0 != iRet) {

            return iRet;
        }
    }

    iRet = adminsPwdLogin();
    qDebug("adminsLogin iRet:[%d] ",  iRet);
    if (0 != iRet) {

        return iRet;
    }

    return 0;
}

void AdminsLogin::getAdminPwdErrPrompt(int iResult)
{
    QString strPromt = "";
    switch(iResult)
    {
        case KB_InputTimeOut_Err:
            strPromt = "Admin pwd input timeout!";
        case KB_UserCancel_Err:
            strPromt = "Admin pwd input cancel!";
        case PWD_Allzero_Err:
            strPromt ="Admin pwd can not be all zeros!";
        case PWD_DefaultSame_Err:
            strPromt ="Admin new pwd can not be the same as the default pwd!";
        case PWD_Simple_Err:
            strPromt = "Admin pwd can not be too simple!";
        case PWD_OldSame_Err:
            strPromt ="Admin new pwd can not be the same as the old pwd!";
        default:
            strPromt ="Admin pwd error!";
    }


    refreshPromptAndExit(strPromt);

}


void AdminsLogin::run()
{
        int iRet = -1;

        if(ADMINS_VERIFY_PWD == mAdminsFuncType)
        {
           iRet = verifyAdminsPwd();
           if(0 == iRet)
           {
                invokeFuncTypeSignal(mStrFuncType);
           }
        }
        else if(ADMINS_CHANGE_PWD == mAdminsFuncType)
        {
          iRet = adminChangePwd(mPwdType);
          if(0 == iRet)
          {
                refreshPromptAndExit("Change pwd success!");
          }
        }

        if( 0 != iRet)
        {
            getAdminPwdErrPrompt(iRet);
        }
}

