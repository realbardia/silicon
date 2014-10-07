# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../../build/bin/silicon_rootmount
    LIBS += ../../../../build/lib/libSiliconLib.so
}

macx {
    TARGET = ../../../../build/bin/silicon.app/Contents/lib/silicon/plugins/silicon_rootmount
    LIBS += ../../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../../build/lib/silicon/plugins/silicon_rootmount
    LIBS += ../../../../build/bin/libsiliconlib.dll
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

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/bin

INSTALLS += target
