#ifndef PINENTRY_H
#define PINENTRY_H

#include <QDialog>
#include <QString>

namespace Ui {
class PinEntry;
}

class PinEntry : public QDialog
{
    Q_OBJECT

public:
    explicit PinEntry(QWidget *parent = 0);
    ~PinEntry();
    void loadStyleSheet(const QString &sheetName);
    void setTextShow(int nBox, int count);
    QWidget *mPtrContainer;
    void setContainerPointer(QWidget* ptrContainer);
private:
    Ui::PinEntry *ui;
private slots:
    void handleRefreshPrompt(int itemid,QString msg);
    void handleItemCtrl(int mode,int count);
    void handlePinEntry(int mode);

};

#endif // PINENTRY_H
