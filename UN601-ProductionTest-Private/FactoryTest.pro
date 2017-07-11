#-------------------------------------------------
#
# Project created by QtCreator 2017-04-25T09:13:57
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = factorytest
TEMPLATE = app

SOURCES += main.cpp\
    dialog.cpp \
    settingmenudlg.cpp \
    versioninfodlg.cpp \
    lcd.cpp \
    paintwidget.cpp \
    common.cpp \
    keypad.cpp \
    ethernetsetting.cpp \
    ethconnect.cpp \
    ping.cpp \
    passwdinput.cpp \
    wifiscan.cpp \
    wifisetting.cpp \
    gprssetting.cpp \
    gprsconnect.cpp \
    wificonnectth.cpp \
    sdcard.cpp \
    serial.cpp \
    serialthread.cpp \
    mainmenuwidget.cpp \
    icccard.cpp \
    scanner.cpp \
    scanread.cpp

HEADERS  += \
    dialog.h \
    dll_api.h \
    settingmenudlg.h \
    settingmenu.h \
    versioninfodlg.h \
    lcd.h \
    paintwidget.h \
    common.h \
    keypad.h \
    ethernetsetting.h \
    ethconnect.h \
    ping.h \
    passwdinput.h \
    wifiscan.h \
    wifisetting.h \
    gprssetting.h \
    gprsconnect.h \
    wificonnectth.h \
    sdcard.h \
    serial.h \
    serialthread.h \
    mainmenuwidget.h \
    icccard.h \
    scanner.h \
    scanread.h

FORMS    += \
    dialog.ui \
    settingmenudlg.ui \
    versioninfodlg.ui \
    lcd.ui \
    common.ui \
    keypad.ui \
    ethernetsetting.ui \
    passwdinput.ui \
    wifisetting.ui \
    gprssetting.ui \
    sdcard.ui \
    serial.ui \
    mainmenuwidget.ui \
    scanner.ui

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
