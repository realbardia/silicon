# -------------------------------------------------
# Project created by QtCreator 2009-12-27T19:30:01
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/apps/Mounter
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so \
        ../../../build-linux/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/apps/Mounter
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 {
    TARGET = ../../../../build-windows/lib/silicon/apps/Mounter
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll \
        ../../../build-windows/bin/sdatabase.dll
}
QT += widgets
TEMPLATE = lib
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi \
    ../../Libs/SDataBase
DEFINES += MOUNTER_LIBRARY
SOURCES += mounter.cpp \
    hpath.cpp \
    managergui.cpp \
    perconf.cpp \
    automount.cpp \
    configure.cpp \
    itempreview.cpp
HEADERS += hpath.h \
    managergui.h \
    perconf.h \
    automount.h \
    configure.h \
    itempreview.h
FORMS += \ 
    configure.ui
RESOURCES += mounter.qrc
