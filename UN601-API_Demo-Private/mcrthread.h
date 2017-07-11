#ifndef MCRTHREAD_H
#define MCRTHREAD_H

#include <QObject>
#include <QThread>

class MCRthread : public QThread
{
    Q_OBJECT
public:
    explicit MCRthread();
    void run();
    void setstop();

    bool stopflag;

signals:
    void sendMessage(QString str);

public slots:
};

#endif // MCRTHREAD_H
