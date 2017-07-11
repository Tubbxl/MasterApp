#include "promptupdateosdlg.h"
#include "ui_promptupdateosdlg.h"

#include "update32550osthread.h"

#include "sdcardupdatedlg.h"

#include "mappdefines.h"

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

PromptUpdateOsDlg::PromptUpdateOsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PromptUpdateOsDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void PromptUpdateOsDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void PromptUpdateOsDlg::initViews()
{

     loadStyleSheet("uistyle1");
}

void PromptUpdateOsDlg::initData()
{

}

 void PromptUpdateOsDlg::setContainerPointer(QWidget* ptrContainer)
 {
    mPtrContainer = ptrContainer;
 }

PromptUpdateOsDlg::~PromptUpdateOsDlg()
{
    delete ui;
}

void PromptUpdateOsDlg::on_btnConfirm_clicked()
{
     mPtrContainer->close();

     int fileType = -1;
     fileType = FILE_TYPE_32550_OS;
     SDCardUpdateDlg *sdcardUpdateDlg = new SDCardUpdateDlg(fileType);

     QGraphicsScene *scene = new QGraphicsScene;
     QGraphicsProxyWidget *proxy = scene->addWidget(sdcardUpdateDlg);
     proxy->setRotation(270);
     QGraphicsView *view = new QGraphicsView(scene);
     view->resize(320, 240);
     view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

      sdcardUpdateDlg->setContainerPointer(view);

    view->show();
}
