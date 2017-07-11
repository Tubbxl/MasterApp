#include "sysupdatemenudlg.h"
#include "ui_sysupdatemenudlg.h"

#include "updatekernelthread.h"

#include "mappdefines.h"
#include "settingmenu.h"

#include "sdcardupdatedlg.h"

#include <QtGui>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

SysUpdateMenuDlg::SysUpdateMenuDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysUpdateMenuDlg)
{
    ui->setupUi(this);

    initData();

    initViews();
}

void SysUpdateMenuDlg::loadStyleSheet(const QString &sheetName)
{
   QFile file(":/qss/" + sheetName.toLower() + ".qss");
   file.open(QFile::ReadOnly);
   QString styleSheet = QLatin1String(file.readAll());

   qApp->setStyleSheet(styleSheet);

   file.close();
}

void SysUpdateMenuDlg::initViews()
{
     ui->listSysUpdate->setFocusPolicy(Qt::NoFocus);
     ui->listSysUpdate->resize(234, 200);

     ui->labelSysUpdate->setText(gb_Update_System_UI[0]);

       int iCountSysUpdate  =   sizeof(gb_Update_System_UI)/sizeof(gb_Update_System_UI[0]);
       int i = 0;
       for(i = 1; i < iCountSysUpdate; i++)
       {
             ui->listSysUpdate->addItem(gb_Update_System_UI[i]);
       }

     loadStyleSheet("uistyle1");
}

void SysUpdateMenuDlg::initData()
{

    connect(ui->listSysUpdate, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickSysUpdateMenuItem(QListWidgetItem*)));

     QTimer::singleShot( UI_TIME_OUT, this, SLOT(uiTimeout()) );

}

void SysUpdateMenuDlg::uiTimeout()
{
    on_btnBack_clicked();
}

SysUpdateMenuDlg::~SysUpdateMenuDlg()
{
    delete ui;
}

void SysUpdateMenuDlg::setContainerPointer(QWidget* ptrContainer)
{
   mPtrContainer = ptrContainer;
}

void SysUpdateMenuDlg::clickSysUpdateMenuItem(QListWidgetItem* item)
{

    QString strText = item->text();
    int fileType = -1;

    if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_BOOT))
    {
         fileType = FILE_TYPE_BOOT;
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_KERNEL))
    {
         fileType = FILE_TYPE_KERNEL;
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_32550_OS))
    {
         fileType = FILE_TYPE_32550_OS;
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_SYSTEM_LIB))
    {
         fileType = FILE_TYPE_SYSTEM_LIB;
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_MASTER_APP))
    {
         fileType = FILE_TYPE_MASTER_APP;
    }
    else if(0 == QString::compare(strText, FUNC_TYPE_UPDATE_API_SERVER))
    {
         fileType = FILE_TYPE_API_SERVER;
    }


    if(-1 != fileType)
    {
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



  //   qDebug("clickSettingMenuItem---index:[%d]", index);

}

void SysUpdateMenuDlg::on_btnBack_clicked()
{
    mPtrContainer->close();
}
