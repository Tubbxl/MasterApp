#include "paintwidget.h"
#include <QPainter>
#include <QPen>
#include <QTimer>
PaintWidget::PaintWidget(QWidget *parent) :
    QWidget(parent)
{
    isDrawing = false;
    image = QImage(1000,1000,QImage::Format_RGB32);
    image.fill(qRgb(255,255,255));
    memset(mflag,0x00,sizeof(mflag));
    mtimer = new QTimer(this);
    connect(mtimer,SIGNAL(timeout()),this,SLOT(check()));
    mtimer->start(500);
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        endPoint = event->pos();
        isDrawing = true;
    }
    else if(event->button()==Qt::RightButton)
    {
        setWindowFlags(Qt::Window);
        showNormal();
    }
    if(event->pos().x()<15&&event->pos().y()<15)
        mflag[0]=1;
    if(event->pos().x()>230&&event->pos().y()>310)
        mflag[1]=1;
    if(event->pos().x()<15&&event->pos().y()>310)
        mflag[2]=1;
    if(event->pos().x()>230&&event->pos().y()<15)
        mflag[3]=1;
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        endPoint = event->pos();        
        if(event->pos().x()<20&&event->pos().y()<20)
            mflag[0]=1;
        if(event->pos().x()>225&&event->pos().y()>300)
            mflag[1]=1;
        if(event->pos().x()<20&&event->pos().y()>300)
            mflag[2]=1;
        if(event->pos().x()>225&&event->pos().y()<20)
              mflag[3]=1;
          qDebug("%d,%d ,%d%d%d%d",event->pos().x(),event->pos().y(),mflag[0],mflag[1],mflag[2],mflag[3]);
        paint(image);
    }

}
void PaintWidget::mouseReleaseEvent(QMouseEvent *)
{
     isDrawing = false;
     paint(image);

}

void PaintWidget::check()
{
    //qDebug("timerEvent()");
    if(((mflag[0]==1)&&(mflag[1]==1)&&(mflag[2]==1)&&(mflag[3]==1)))
    {
        mtimer->stop();
        emit PaintBack();
    }
}

void PaintWidget::paint(QImage &theImage)
{
    QPainter p(&theImage);
    QPen apen;
    apen.setWidth(2);
    p.setPen(apen);
    p.drawLine(lastPoint,endPoint);
    lastPoint = endPoint;
    update();
}


PaintWidget ::~PaintWidget()
{

}
