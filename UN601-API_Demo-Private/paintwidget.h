#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    PaintWidget(QWidget *parent = 0);
    ~PaintWidget();
public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
public slots:
    void check();

    void paint(QImage &theImage);
signals:
    void PaintBack();
private:

    QTimer *mtimer;
    QImage image;
    QPoint lastPoint;
    QPoint endPoint;
    char mflag[4];
    bool isDrawing;
};

#endif // PAINTWIDGET_H
