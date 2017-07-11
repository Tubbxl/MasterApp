#ifndef PICCCHECK_H
#define PICCCHECK_H

#include <QThread>


class Picccheck : public QThread
{
    Q_OBJECT
public:
    explicit Picccheck();
    void run();
    void stopPicc();

    bool stop;

signals:
    void sendMessage(QString str);

public slots:

};

#endif // PICCCHECK_H
