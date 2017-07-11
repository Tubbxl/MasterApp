#ifndef GPRSSETTING_H
#define GPRSSETTING_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QMessageBox>
#include "gprsconnect.h"
#include "ping.h"
extern "C" {
#include "include/dll_api.h"
}
namespace Ui {
class GprsSet;
}

class Gprssetting : public QWidget
{
    Q_OBJECT

public:
    explicit Gprssetting(QWidget *parent = 0);
    ~Gprssetting();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
signals:
     void back();
private slots:
    void on_connect_clicked();

    void on_pushButton_2_clicked();
    void deal(int ret);
    void gprs_ping_result(int ret);
private:
    Ui::GprsSet *ui;
    gprsconnect *th;
    int flag;

    ping *gprsping;

    QWidget *mPtrContainer;

    QMessageBox *mpPromptMsgBox;
};

#endif // GPRS_H
