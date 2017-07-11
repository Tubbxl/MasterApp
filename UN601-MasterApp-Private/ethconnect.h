#ifndef ETHCONNECT_H
#define ETHCONNECT_H

#include <QObject>
#include <QThread>
class ethconnect : public QThread
{
    Q_OBJECT
public:
    explicit ethconnect(QObject *parent = 0);
protected:
    void run();
signals:
    void ethconnetret(int a);
public slots:
};

#endif // ETHCONNECT_H
