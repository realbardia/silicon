# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/plugins/SystemTray
    LIBS += ../../../build/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/plugins/SystemTray
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/plugins/SystemTray
    LIBS += ../../../build/bin/libsiliconlib.dll
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

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/plugins/

INSTALLS += target
