# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/plugins/Win32DeviceNotifier
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/Win32DeviceNotifier
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/plugins/Win32DeviceNotifier
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll
}

INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

#DEFINES += _WIN32_WINDOWS=0x0410

TEMPLATE = lib
SOURCES += body.cpp \
    win32devicenotifier.cpp \
    plugin.cpp \
    cdrecordhw.cpp
HEADERS += \ 
    win32devicenotifier.h \
    plugin.h \
    cdrecordhw.h
RESOURCES += win32devicenotifier.qrc
