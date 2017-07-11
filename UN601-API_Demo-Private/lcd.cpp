#include "lcd.h"
#include "ui_lcd.h"
#include <QFile>
#include <QDebug>


lcd::lcd(int type,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lcd)
{
    ui->setupUi(this);
    mLcdTsetType = type;
    if(type==0)
    {
        mitem = 0;
        ui->label_2->setStyleSheet("background-color: rgbrgb(255, 255, 255);");
        ui->frame->close();
    }
    if(type==1)
    {
          area = new PaintWidget(ui->frame);
          area->resize(234,312);
          ui->label->setText("Touch Test");
          connect(area,SIGNAL(PaintBack()),this,SLOT(failOrPass()));
          ui->label_2->close();
    }
    hideAllWidget();
}

lcd::~lcd()
{
    delete ui;
}

void lcd::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);

    file.close();
}

void lcd::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void lcd::hideAllWidget()
{
    ui->fail->hide();
    ui->label->hide();
    ui->pass->hide();
}

void lcd::showAllWidget()
{
    ui->fail->show();
    ui->label->show();
    ui->pass->show();
}

void lcd::on_fail_clicked()
{
    mitem = 0;
    mPtrContainer->close();
    emit return_test_result(1);
}

void lcd::on_pass_clicked()
{
    mitem = 0;
    mPtrContainer->close();
    emit return_test_result(0);
}

void lcd::failOrPass()
{
    loadStyleSheet("uistyle1");
    ui->fail->show();
    ui->pass->show();
    ui->frame->close();
    ui->label_2->close();
    ui->label->show();
}



void lcd::mousePressEvent(QMouseEvent *)
{
    if(mLcdTsetType == 0)
    {
        qDebug("%d",mitem);
        if(mitem == 0)
        {
            ui->label_2->setStyleSheet("background-color: rgbrgb(0, 0, 0);");
            mitem++;
            return;
        }
        else if(mitem == 1)
        {
            ui->label_2->setStyleSheet("background-color: rgbrgb(255, 0, 0);");
            mitem++;
             return;
        }
        else if(mitem ==2)
        {
            ui->label_2->setStyleSheet("background-color: rgbrgb(0, 255, 0);");
            mitem++;
             return;
        }
        else if(mitem ==3)
        {
              ui->label_2->setStyleSheet("background-color: rgbrgb(0, 0, 255);");
            mitem++;
             return;
        }
        else if(mitem ==4)
        {
            ui->label_2->setStyleSheet("background-color: rgbrgb(254, 254, 254);");
            showAllWidget();
            ui->label_2->close();
            loadStyleSheet("uistyle1");
            mitem++;
        }
    }
}





