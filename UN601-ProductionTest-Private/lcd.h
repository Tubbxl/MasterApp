#ifndef LCD_H
#define LCD_H

#include <QWidget>
#include <QMouseEvent>
#include "paintwidget.h"
namespace Ui {
class lcd;
}

class lcd : public QWidget
{
    Q_OBJECT

public:
    explicit lcd(int type,QWidget *parent = 0);
    ~lcd();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);

    void hideAllWidget();
    void showAllWidget();
    void setColor();
     void paint(QImage &theImage);
signals:
      void return_test_result(QString ret);
private slots:
      void on_fail_clicked();

      void on_pass_clicked();

      void failOrPass();
     // void on_lcd_windowIconChanged(const QIcon &icon);
public slots:
    void mousePressEvent(QMouseEvent *event);


private:
    Ui::lcd *ui;

    int mitem;
    int mLcdTsetType;
    QWidget *mPtrContainer;
    PaintWidget *area;


};

#endif // LCD_H
