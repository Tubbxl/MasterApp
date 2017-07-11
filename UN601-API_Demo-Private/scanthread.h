#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QObject>
#include <QThread>

class ScanThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanThread();
    bool stopflag;
    void run();



signals:
    void sendMessage(QString str);
public slots:
};

#endif // SCANTHREAD_H
