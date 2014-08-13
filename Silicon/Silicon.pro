# -------------------------------------------------
# Project created by QtCreator 2009-11-17T15:40:25
# -------------------------------------------------
unix:!macx {
    TARGET = ../../build-linux/bin/silicon
    LIBS += ../../build-linux/lib/libSiliconLib.so \
        ../../build-linux/lib/libsidi.so \
        ../../build-linux/lib/libsdatabase.so
}
macx {
    TARGET = ../../build-osx/bin/silicon
    LIBS += ../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 { 
    TARGET = ../../../build-windows/bin/silicon
    LIBS += ../../build-windows/bin/libsiliconlib.dll \
        ../../build-windows/bin/sidi.dll \
        ../../build-windows/bin/sdatabase.dll
}

include(qtsingleapplication/qtsingleapplication.pri)
#include(locale/locale.pri)

QT += widgets
TEMPLATE = app
unix: CONFIG += dbus

INCLUDEPATH += ../Libs/SiliconLib \
    ../Libs/SDataBase \
    ../Libs/SiDi

SOURCES += main.cpp \
    siliconui.cpp \
    sdefaults.cpp \
    sthemeengine.cpp \
    perconf.cpp \
    configure.cpp \
    sapplicationengine.cpp \
    siliconsole.cpp \
    depends.cpp \
    authorwidget.cpp \
    authorlistwidget.cpp \
    appinfowidget.cpp \
    about.cpp \
    fileloaderengine.cpp \
    sapplicationsconfigure.cpp \
    menubarmanager.cpp \
    spluginengine.cpp \
    pluginmanager.cpp \
    applicationmanager.cpp \
    sprogresslist.cpp \
    discmanager.cpp \
    dialogmanager.cpp \
    statusbarmanager.cpp \
    toolbarmanager.cpp \
    appmainwindow.cpp

HEADERS += siliconui.h \
    sdefaults.h \
    sthemeengine.h \
    perconf.h \
    configure.h \
    sapplicationengine.h \
    siliconsole.h \
    depends.h \
    authorwidget.h \
    authorlistwidget.h \
    appinfowidget.h \
    about.h \
    fileloaderengine.h \
    sapplicationsconfigure.h \
    menubarmanager.h \
    spluginengine.h \
    pluginmanager.h \
    applicationmanager.h \
    sprogresslist.h \
    discmanager.h \
    dialogmanager.h \
    statusbarmanager.h \
    toolbarmanager.h \
    appmainwindow.h

FORMS += appearanceUI.ui \
    depends.ui \
    authorwidget.ui \
    appinfowidget.ui \
    fileloaderengine.ui \
    sapplicationsconfigure.ui \
    loadedplugins.ui \
    applicationmanager.ui

RESOURCES += resource.qrc
