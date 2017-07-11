#include "scanner.h"
#include "ui_scanner.h"


#include <QDebug>
#include <QFile>

extern "C"{
#include "dll_api.h"
}

#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0

Scanner::Scanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scanner)
{
    ui->setupUi(this);
    ui->label->close();
    ui->pre->setMaxLength(4);
    ui->suf->setMaxLength(2);
    ui->pre->close();
    ui->suf->close();
    memset(mDate,0x00,sizeof(mDate));
    mMode = 0;
    ui->set->close();
    ui->set_2->close();
    mReadThread = new scanread;

   // connect(this,SIGNAL(readend()),this,SLOT(datedeal()));
     loadStyleSheet("uistyle1");
}

void Scanner::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/qss/" + sheetName.toLower() + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    qApp->setStyleSheet(styleSheet);

    file.close();
}

void Scanner::setContainerPointer(QWidget *ptrContainer)
{
        mPtrContainer = ptrContainer;
}

Scanner::~Scanner()
{
    delete ui;
}

void Scanner::on_set_clicked()
{
    QString str;
    QString suffix;
    str = ui->pre->text();
    QByteArray pre = str.toLatin1();//str.toLatin1();
    mPrefix  = pre.data();
    if(str.isEmpty())
        mPrefix = NULL;

    suffix = ui->suf->text();
    QByteArray suf = suffix.toLatin1();
    mSuffix = suf.data();
    if(suffix.isEmpty())
        mSuffix = NULL;
    //qDebug("mode = %02x mPrefix=[%s] mSuffix=[%s]",mMode,mPrefix,mSuffix);
    DLL_ScanModeSet((unsigned char)mMode,(unsigned char*)mPrefix,(unsigned char*)mSuffix);
}

void Scanner::on_set_2_clicked()
{
    ui->label->clear();
    ui->resultt->clear();
    DLL_ScanTrigger();
    /*connect(mReadThread,&scanread::sendreaddate( mDatalen, mDate),[ =]{
         printf(" in thread readdata = %s  %d\n",mDate,mDatalen);
        qDebug("0000000000000000000");
    });*/
   QObject::connect(mReadThread,SIGNAL(sendreaddate(int *,unsigned char*)),this,SLOT(getdata(int *, unsigned char*)));
    mReadThread->start();

}

void Scanner::on_mode_clicked()  //mode
{
    if(ui->mode->isChecked())
    {
        qDebug("keywedge  1");
        setbit(mMode,0);
        return;
    }
    qDebug("keywedge  0");
       clrbit(mMode,0);
}

void Scanner::on_mode_2_clicked() //have enter
{
    if(ui->mode_2->isChecked())
    {
        qDebug("enter  1");
        setbit(mMode,2);
        return;
    }
         clrbit(mMode,2);
         qDebug("enter  0");
}

void Scanner::on_prefix_clicked()  //prefix
{
    if(ui->prefix->isChecked())
    {
        qDebug("prefix  1");
        setbit(mMode,1);
        ui->pre->show();
        return;
    }
        clrbit(mMode,1);
        ui->pre->close();
        ui->pre->clear();
         qDebug("prefix  0");
}

void Scanner::on_suffix_clicked() //suffix
{
    if(ui->suffix->isChecked())
    {
        qDebug("suffix  1");
       setbit(mMode,1);
        ui->suf->show();
        return;
    }
        clrbit(mMode,1);
        ui->suf->close();
        ui->suf->clear();
        qDebug("suffix  0");
}

void Scanner::on_pushButton_clicked()
{
    ui->pushButton->close();
    ui->set->show();
    ui->set_2->show();
    DLL_ScanOpen();
}

void Scanner::datedeal()
{
    QString str;
    qDebug("deal data");
  //  if(mDatalen)
    {
        ui->resultt->setText(QString(QLatin1String(mDate)));
        str.sprintf("Len:%s",strlen(mDate));
        ui->label->setText(str);
    }

}

void Scanner::getdata(int *len, unsigned char * data)
{
    QString str;

    int Len = 0;
    Len = *len;

    qDebug("get data %d   %s",Len,data);
    strcpy((char *)mDate,(char *)data);
    if(Len<strlen(mDate))
        Len = strlen(mDate);
    if(Len>0)
    {
       str.sprintf("Len:%d",Len);
       ui->label->setText(str);
       ui->label->show();
    }
    if(Len>0&&mDate[3]!=0)
    {

        mDatalen = Len;
        if((mMode&0x01)==0)
        {
            ui->resultt->setText(QString(QLatin1String(mDate)));
        }
    }
    //emit readend();
    mReadThread->exit();
    mReadThread->quit();
   mReadThread->wait();
}

void Scanner::on_pushButton_2_clicked()
{
    DLL_ScanClose();
    mPtrContainer->close();
    emit return_test_result("Scan Test Finish!!!");
}
