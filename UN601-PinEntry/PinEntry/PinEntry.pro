#-------------------------------------------------
#
# Project created by QtCreator 2017-05-13T10:11:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PinEntry
TEMPLATE = app


SOURCES += main.cpp\
        pinentry.cpp \
    updatemsgthread.cpp

HEADERS  += pinentry.h \
    updatemsgthread.h

FORMS    += pinentry.ui
