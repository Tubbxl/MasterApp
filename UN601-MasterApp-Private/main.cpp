#include "maindialog.h"
#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <QMessageBox>

extern "C" {
 #include "include/dll_api.h"
}


int main(int argc,  char *argv[])
{
    QApplication a(argc, argv);

    DLL_ApiInit();

     MainDialog *mainDialog = new MainDialog;

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *proxy = scene->addWidget(mainDialog);
    proxy->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);
    view->resize(320, 240);

    mainDialog->setParentPointer(view);

    view->show();

    return a.exec();
}
