#ifndef KEYPAD_H
#define KEYPAD_H

#define KEY1             0x31
#define KEY2             0x32
#define KEY3             0x33
#define KEY4             0x34
#define KEY5             0x35
#define KEY6             0x36
#define KEY7             0x37
#define KEY8             0x38
#define KEY9             0x39
#define KEY0             0x30
#define KEYWELL        0x23
#define KEYCLEAR         0x08
#define KEYALPHA         0x07
#define KEYENTER         0x0d
#define KEYCANCEL        0x1b
#define KEYBACKSPACE     0x1c
#define KEYF1				 0x1e
#define KEYF2				 0x1f
#define KEYF3				 0x20
#define KEYF4				 0x21

#include <QWidget>
#include <QTimer>

namespace Ui {
class Keypad;
}

class Keypad : public QWidget
{
    Q_OBJECT

public:
    explicit Keypad(QWidget *parent = 0);
    ~Keypad();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
    void keyPressEvent(QKeyEvent *event);
    void hidebnt();
private slots:
    void on_fail_clicked();

    void on_pass_clicked();

   // void btndeal();

signals:
      void return_test_result(int ret);
private:
    Ui::Keypad *ui;
    QWidget *mPtrContainer;
 //   QTimer *mtimer;
    int mKeyCnt;
};

#endif // KEYPAD_H
