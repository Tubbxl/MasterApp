#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <QWidget>

namespace Ui {
class signature;
}

class signature : public QWidget
{
    Q_OBJECT

public:
    explicit signature(QWidget *parent = 0);
    ~signature();
    void loadStyleSheet(const QString &sheetName);
    void setContainerPointer(QWidget* ptrContainer);
private slots:
    void on_pushButton_clicked();

private:
    Ui::signature *ui;
    QWidget *mPtrContainer;
};

#endif // SIGNATURE_H
