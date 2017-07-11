#ifndef SCANREAD_H
#define SCANREAD_H

#include <QObject>
#include <QThread>
class scanread : public QThread
{
    Q_OBJECT
public:
    explicit scanread(QObject *parent = 0);
     char readdata[100];
    void run();
signals:
    void sendreaddate(int *len,unsigned char *);
public slots:
};

#endif // SCANREAD_H
