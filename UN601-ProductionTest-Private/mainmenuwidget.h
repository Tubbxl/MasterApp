#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QThread>

namespace Ui {
class mainmenuwidget;
}

class mainmenuwidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainmenuwidget(QWidget *parent = 0);
    mainmenuwidget( QString  (*pStrList)[18], int listSize, QWidget *parent = 0);
    ~mainmenuwidget();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);


    void displayResult(QString ret);
     void handleFuncTestType(QString strTestType);

     void LCDRgb();
     void scannerTest();

  signals:
     void toStopThread();

private slots:
    void on_next_clicked();

    void on_start_2_clicked();

    void on_count_clicked();

    void on_start_clicked();

    void on_pass_clicked();

    void on_fail_clicked();

    void receiveMsg(QString Msg);

private:
    Ui::mainmenuwidget *ui;
    QWidget *mPtrContainer;

    QString (*mpStrMenuUI)[18];
    int miMenuGroup;
    int mIndex;
    int mTsetCount;
    int mFailCount;
    int mPassCount;
    bool mStart;

    bool mThreadCreate[40];

    QThread mItemThreas[40];
    QString mMenuType;
};

#endif // MAINMENUWIDGET_H
