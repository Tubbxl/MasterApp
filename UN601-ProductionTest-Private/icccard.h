#ifndef ICCCARD_H
#define ICCCARD_H

#include <QObject>
#include <QThread>

class icccard : public QThread
{
    Q_OBJECT
public:
    explicit icccard(QObject *parent = 0);

    bool mStopflag;
    bool mIsRun;
signals:
    void sendMessage(QString msg);
public slots:
    void iccdeal();
    void piccdeal();
    void scandeal();
    void mcrdeal();
    void stopTheThread();
};

#endif // ICCCARD_H
