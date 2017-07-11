#ifndef SDCARD_H
#define SDCARD_H

#include <QWidget>
#include <QString>

static const QString SD_CARD_PATH  = "/media/media/mmcblk0p1/tese.dat";

namespace Ui {
class SDcard;
}

class SDcard : public QWidget
{
    Q_OBJECT

public:
    explicit SDcard(const QString *type,QWidget *parent = 0);
    ~SDcard();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void saveDate();
    void getDateToCompare();

private slots:
    void on_pass_clicked();

    void on_fail_clicked();
signals:
    void return_test_result(int ret);

private:
    Ui::SDcard *ui;    
    QWidget *mPtrContainer;
    char mFilePath[100];
    char mDirPath[100];
    int mSaveFlag;
    QString TYPE;
};

#endif // SDCARD_H
