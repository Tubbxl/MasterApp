#ifndef GPRSCONNECT_H
#define GPRSCONNECT_H

#include <QObject>
#include <QThread>
class gprsconnect : public QThread
{
    Q_OBJECT
public:
    explicit gprsconnect(QObject *parent = 0);
    gprsconnect(unsigned char *input_apn, unsigned char *input_name,  unsigned char *input_pswd,  unsigned char *input_tel);
    ~gprsconnect();
protected:
    void run();
signals:
   void gprscntret(int ret);
public slots:
private:
     unsigned char apn[20]={0};
     unsigned char name[20]={0};
      unsigned char pswd[20]={0};
     unsigned char tel[20]={0};
};

#endif // GPRSCONNECT_H
