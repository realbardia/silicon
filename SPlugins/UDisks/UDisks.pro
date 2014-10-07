# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/plugins/UDisksDeviceNotifier
    LIBS += ../../../build/lib/libsilicon.so \
        ../../../build/lib/libsidi.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/plugins/UDisksDeviceNotifier
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libsilicon.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/plugins/UDisksDeviceNotifier
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll
}
QT += dbus

INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

TEMPLATE = lib
SOURCES += body.cpp \
    sudisks.cpp \
    plugin.cpp \
    speeddetector.cpp
HEADERS += \ 
    sudisks.h \
    plugin.h \
    speeddetector.h
RESOURCES += udisks.qrc

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/plugins/

INSTALLS += target
