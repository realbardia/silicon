# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/plugins/SystemTray
    LIBS += ../../../build-linux/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/SystemTray
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/plugins/SystemTray
    LIBS += ../../../build-windows/bin/libsiliconlib.dll
}

QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib

TEMPLATE = lib
SOURCES += body.cpp \
    plugin.cpp \
    systray.cpp
HEADERS += plugin.h \
    systray.h
RESOURCES += systemtray.qrc
