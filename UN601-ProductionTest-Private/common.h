#ifndef COMMON_H
#define COMMON_H

#include <QThread>
#include <QTimer>
#include <QObject>

namespace Ui {
class common;
}

class common : public QThread
{
    Q_OBJECT

public:
    explicit common(const QString *type,QObject *parent = 0);
    ~common();



    int getCurrDateTime(char* pszDate,  char* pszTime);
public slots:

     void InitView();
     void getDDRSize();
     void ledtest();
     void beeptest();
     void backlight();
     void refreshDisplayDateTime();


signals:
      void sendMessage(QString msg);
private:
    QString TYPE;
    QTimer *mtimer;
    bool mIsRun;
};

#endif // COMMON_H
