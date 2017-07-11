#include "pinentry.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PinEntry *form = new PinEntry; //项目主窗口

    //  form->loadStyleSheet("Coffee");
        form->loadStyleSheet("pagefold");

    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsProxyWidget *w = scene->addWidget(form);
    w->setRotation(270);
    QGraphicsView *view = new QGraphicsView(scene);

    view->showMaximized();
    //view->resize(320, 240);
    //view->show();

     return a.exec();
}


