/********************************************************************************
** Form generated from reading UI file 'pinentry.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PINENTRY_H
#define UI_PINENTRY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_PinEntry
{
public:
    QTextEdit *PwdShow;
    QTextEdit *PwdConfirm;
    QLabel *labelPwd;
    QLabel *labelPwdConfirm;
    QLabel *labelPrompt;

    void setupUi(QDialog *PinEntry)
    {
        if (PinEntry->objectName().isEmpty())
            PinEntry->setObjectName(QStringLiteral("PinEntry"));
        PinEntry->resize(227, 314);
        PwdShow = new QTextEdit(PinEntry);
        PwdShow->setObjectName(QStringLiteral("PwdShow"));
        PwdShow->setEnabled(false);
        PwdShow->setGeometry(QRect(0, 120, 221, 31));
        PwdConfirm = new QTextEdit(PinEntry);
        PwdConfirm->setObjectName(QStringLiteral("PwdConfirm"));
        PwdConfirm->setEnabled(false);
        PwdConfirm->setGeometry(QRect(0, 250, 221, 31));
        labelPwd = new QLabel(PinEntry);
        labelPwd->setObjectName(QStringLiteral("labelPwd"));
        labelPwd->setGeometry(QRect(10, 80, 161, 17));
        labelPwdConfirm = new QLabel(PinEntry);
        labelPwdConfirm->setObjectName(QStringLiteral("labelPwdConfirm"));
        labelPwdConfirm->setGeometry(QRect(10, 220, 161, 17));
        labelPrompt = new QLabel(PinEntry);
        labelPrompt->setObjectName(QStringLiteral("labelPrompt"));
        labelPrompt->setGeometry(QRect(10, 10, 211, 41));
        labelPrompt->setWordWrap(true);

        retranslateUi(PinEntry);

        QMetaObject::connectSlotsByName(PinEntry);
    } // setupUi

    void retranslateUi(QDialog *PinEntry)
    {
        PinEntry->setWindowTitle(QApplication::translate("PinEntry", "PinEntry", 0));
        labelPwd->setText(QString());
        labelPwdConfirm->setText(QString());
        labelPrompt->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PinEntry: public Ui_PinEntry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PINENTRY_H
