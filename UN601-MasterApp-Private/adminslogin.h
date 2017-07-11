#ifndef ADMINSLOGIN_H
#define ADMINSLOGIN_H

#include <QThread>

#include "mappdefines.h"

class AdminsLogin : public QThread
{
    Q_OBJECT
public:
    explicit AdminsLogin(unsigned char funcType, QString strFuncType, unsigned char pwdType=PWD_TYPE_ADMIN_A, QObject *parent = 0);
    ~AdminsLogin();
    void refreshPromptAndExit(QString strPrompt);

    int adminsPwdLogin();
    int adminChangePwd(unsigned char pwdType);
    int verifyAdminsPwd();

    void getAdminPwdErrPrompt(int iResult);

protected:
    void run();

signals:
    void refreshPromptSignal(QString strPrompt);
     void closePromptSignal();
    void invokeFuncTypeSignal(QString strFuncType);

private:

     unsigned char mAdminsFuncType;
     QString mStrFuncType;
     unsigned char mPwdType;

};

#endif // ADMINSLOGIN_H
