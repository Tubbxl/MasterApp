#-------------------------------------------------
#
# Project created by QtCreator 2017-04-25T09:13:57
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = API_Demo
TEMPLATE = app

SOURCES += main.cpp\
    dialog.cpp \
    settingmenudlg.cpp \
    ethernetsetting.cpp \
    ethconnect.cpp \
    passwdinput.cpp \
    wifiscan.cpp \
    wifisetting.cpp \
    gprssetting.cpp \
    gprsconnect.cpp \
    wificonnectth.cpp \
    serial.cpp \
    serialthread.cpp \
    setdatetimedlg.cpp \
    picccheck.cpp \
    mcrthread.cpp \
    scanthread.cpp \
    scanner.cpp \
    scanread.cpp \
    signature.cpp

HEADERS  += \
    dialog.h \
    dll_api.h \
    settingmenudlg.h \
    settingmenu.h \
    ethernetsetting.h \
    ethconnect.h \
    passwdinput.h \
    wifiscan.h \
    wifisetting.h \
    gprssetting.h \
    gprsconnect.h \
    wificonnectth.h \
    serial.h \
    serialthread.h \
    setdatetimedlg.h \
    picccheck.h \
    mcrthread.h \
    scanthread.h \
    scanner.h \
    scanread.h \
    signature.h

FORMS    += \
    dialog.ui \
    settingmenudlg.ui \
    ethernetsetting.ui \
    passwdinput.ui \
    wifisetting.ui \
    gprssetting.ui \
    sdcard.ui \
    serial.ui \
    setdatetimedlg.ui \
    scanner.ui \
    signature.ui

RESOURCES += \
    ProductionTest.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lapi
else:unix: LIBS += -L$$PWD/lib/ -lapi

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lnetworkapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lnetworkapi
else:unix: LIBS += -L$$PWD/lib/ -lnetworkapi

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

CONFIG += C++11
