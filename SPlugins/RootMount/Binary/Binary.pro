# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../../build-linux/bin/silicon_rootmount
    LIBS += ../../../../build-linux/lib/libSiliconLib.so
}

macx {
    TARGET = ../../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/silicon_rootmount
    LIBS += ../../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../../build-windows/lib/silicon/plugins/silicon_rootmount
    LIBS += ../../../../build-windows/bin/libsiliconlib.dll
}

QT += testlib

INCLUDEPATH += ../../../Libs/SiliconLib

TEMPLATE = app
SOURCES += body.cpp \
    mounter.cpp \
    unmounter.cpp \
    manager.cpp

HEADERS += \
    mounter.h \
    unmounter.h \
    manager.h
