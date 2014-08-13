# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/plugins/HalDeviceNotifier
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/HalDeviceNotifier
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/plugins/HalDeviceNotifier
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll
}
QT += dbus

INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

TEMPLATE = lib
SOURCES += body.cpp \
    shaldevicenotifier.cpp \
    plugin.cpp
HEADERS += \ 
    shaldevicenotifier.h \
    plugin.h
RESOURCES += haldevicenotifier.qrc
