# -------------------------------------------------
# Project created by QtCreator 2009-11-17T15:40:25
# -------------------------------------------------
unix:!macx {
    TARGET = ../../build/bin/silicon
    LIBS += ../../build/lib/libsilicon.so \
        ../../build/lib/libsidi.so \
        ../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../build/bin/silicon
    LIBS += ../../build/bin/silicon.app/Contents/lib/libsilicon.dylib \
        ../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 { 
    TARGET = ../../../build/bin/silicon
    LIBS += ../../build/bin/libsiliconlib.dll \
        ../../build/bin/sidi.dll \
        ../../build/bin/sdatabase.dll
}

include(qtsingleapplication/qtsingleapplication.pri)
include(locale/locale.pri)

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

icons_files.source = files/icons/
icons_files.target = ../../build/share/silicon
themes_files.source = files/themes/
themes_files.target = ../../build/share/silicon
DEPLOYMENTFOLDERS += icons_files themes_files
include(qmake/qtcAddDeployment.pri)
qtcAddDeployment()

isEmpty(PREFIX) {
    PREFIX = /usr
    APPDESK_PATH = /usr/
}

contains(BUILD_MODE,opt) {
    APPDESK_PATH = /usr/
} else {
    APPDESK_PATH = $$PREFIX/
}

target = $$TARGET
target.path = $$PREFIX/bin
icons.files = files/icons/
icons.path = $$PREFIX/share/silicon
pixmap.files = files/icons/silicon.png
pixmap.path = $$APPDESK_PATH/share/pixmaps
themes.files = files/themes/
themes.path = $$PREFIX/share/silicon
translations.files = ../../build/share/silicon/languages
translations.path = $$PREFIX/share/silicon
desktopFile.files = files/Silicon.desktop
desktopFile.path = $$APPDESK_PATH/share/applications

INSTALLS = target icons pixmap themes translations desktopFile

contains(BUILD_MODE,opt) {
    runFile.files = run
    runFile.path = $$PREFIX
    INSTALLS += runFile
}
