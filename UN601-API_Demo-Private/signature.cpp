#include "signature.h"
#include "ui_signature.h"
#include <QThread>
#include <QFile>
#include <QPainter>

extern "C" {
#include "dll_api.h"
}
signature::signature(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signature)
{
    ui->setupUi(this);
    DLL_HandSignInit();
    QThread::msleep(500);
    unsigned char buff[8*1024] = {0};
    DLL_HandSignGetData(buff);
    QImage mage(buff,234,234,QImage::Format_MonoLSB);// = QImage::fromData(buff,7550);
    QPainter painter(ui->frame);
    painter.drawImage(0,0,mage);
}

signature::~signature()
{
    delete ui;
}

void signature::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void signature::setContainerPointer(QWidget *ptrContainer)
{
    mPtrContainer = ptrContainer;
}

void signature::on_pushButton_clicked()
{
    mPtrContainer->close();
}
