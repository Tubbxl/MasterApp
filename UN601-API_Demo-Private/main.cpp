#include "settingmenudlg.h"
#include "settingmenu.h"

#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <QMessageBox>

extern "C" {
 #include "dll_api.h"
}
int main(int argc,  char *argv[])
{
    QApplication a(argc, argv);

    DLL_ApiInit();

    int iCountLine = -1;
    SettingMenuDlg *fMenuDlg = NULL;

   iCountLine  =   sizeof(gb_ProductionTest_UI)/sizeof(gb_ProductionTest_UI[0]) ;
   fMenuDlg = new SettingMenuDlg((QString  (*)[18])gb_ProductionTest_UI, iCountLine);

  QGraphicsScene *scene = new QGraphicsScene;
  QGraphicsProxyWidget *proxy = scene->addWidget(fMenuDlg);
  proxy->setRotation(270);
  QGraphicsView *view = new QGraphicsView(scene);
  view->resize(320, 240);
  view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

 fMenuDlg->setContainerPointer(view);
 view->setWindowFlags(Qt::Dialog);

 view->showFullScreen();
  view->show();

    return a.exec();
}
