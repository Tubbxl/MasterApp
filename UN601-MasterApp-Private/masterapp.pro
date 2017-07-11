#-------------------------------------------------
#
# Project created by QtCreator 2017-04-26T17:33:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MasterApp
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    settingmenudlg.cpp \
    sdcardupdatedlg.cpp \
    mfile.cpp \
    sysupdatemenudlg.cpp \
    updatebootthread.cpp \
    updatekernelthread.cpp \
    updatesyslibthread.cpp \
    updatemasterappthread.cpp \
    updatesubapps.cpp \
    updatesubappthread.cpp \
    downloadcathread.cpp \
    downloadfmsignkey.cpp \
    sslcommpacket.cpp \
    downloadappsignkey.cpp \
    downloadmkey.cpp \
    dukpt.cpp \
    mkey.cpp \
    appnofile.cpp \
    apppermsfile.cpp \
    update32550osthread.cpp \
    checkstatusthread.cpp \
    promptupdateosdlg.cpp \
    versioninfodlg.cpp \
    displayapplist.cpp \
    setdatetimedlg.cpp \
    delappdlg.cpp \
    adminslogin.cpp \
    adminschpwddlg.cpp \
    ethconnect.cpp \
    gprs.cpp \
    gprsconnect.cpp \
    network.cpp \
    passwdinput.cpp \
    setthread.cpp \
    wifi.cpp \
    wificonnectth.cpp \
    wifiscan.cpp \
    optestthread.cpp \
    wifisetting.cpp \
    ethernetsetting.cpp \
    gprssetting.cpp \
    ping.cpp \
    updateapiserversthread.cpp

HEADERS  += maindialog.h \
    settingmenu.h \
    settingmenudlg.h \
    sdcardupdatedlg.h \
    mfile.h \
    mappdefines.h \
    sysupdatemenudlg.h \
    updatebootthread.h \
    updatekernelthread.h \
    updatesyslibthread.h \
    updatemasterappthread.h \
    updatesubapps.h \
    updatesubappthread.h \
    include/SSLComm.h \
    downloadcathread.h \
    downloadfmsignkey.h \
    sslcommpacket.h \
    downloadappsignkey.h \
    downloadmkey.h \
    dukpt.h \
    mkey.h \
    appnofile.h \
    apppermsfile.h \
    update32550osthread.h \
    user.h \
    checkstatusthread.h \
    promptupdateosdlg.h \
    versioninfodlg.h \
    displayapplist.h \
    setdatetimedlg.h \
    include/dll_api.h \
    delappdlg.h \
    adminslogin.h \
    adminschpwddlg.h \
    ethconnect.h \
    gprs.h \
    gprsconnect.h \
    network.h \
    passwdinput.h \
    setthread.h \
    wifi.h \
    wificonnectth.h \
    wifiscan.h \
    optestthread.h \
    wifisetting.h \
    gprssetting.h \
    ethernetsetting.h \
    ping.h \
    updateapiserversthread.h

FORMS    += maindialog.ui \
    settingmenudlg.ui \
    sdcardupdatedlg.ui \
    sysupdatemenudlg.ui \
    promptupdateosdlg.ui \
    versioninfodlg.ui \
    displayapplist.ui \
    setdatetimedlg.ui \
    delappdlg.ui \
    adminschpwddlg.ui \
    gprs.ui \
    network.ui \
    passwdinput.ui \
    wifi.ui \
    wifisetting.ui \
    ethernetsetting.ui \
    gprssetting.ui

RESOURCES += \
    MasterApp.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lSSLComm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lSSLComm
else:unix: LIBS += -L$$PWD/libs/ -lSSLComm

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lcrypto
else:unix: LIBS += -L$$PWD/libs/ -lcrypto

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lssl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lssl
else:unix: LIBS += -L$$PWD/libs/ -lssl

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lapi
else:unix: LIBS += -L$$PWD/libs/ -lapi

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lnetworkapi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lnetworkapi
else:unix: LIBS += -L$$PWD/libs/ -lnetworkapi

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

CONFIG += C++11
