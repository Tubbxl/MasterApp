#ifndef PING_H
#define PING_H

#include <QObject>
#include <QThread>
class ping : public QThread
{
    Q_OBJECT
public:
    explicit ping(QObject *parent = 0);
    ping(int in_flag, QObject *parent = 0);
protected:
    void run();
signals:
   void pingresult(int ret);
   void wifipingresult(int ret);
   void gprspingresult(int ret);
signals:

public slots:
private:
   int flag;
};

#endif // PING_H
