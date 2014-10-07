# -------------------------------------------------
# Project created by QtCreator 2009-12-27T19:30:01
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/Mounter
    LIBS += ../../../build/lib/libSiliconLib.so \
        ../../../build/lib/libsidi.so \
        ../../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/Mounter
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 {
    TARGET = ../../../../build/lib/silicon/apps/Mounter
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll \
        ../../../build/bin/sdatabase.dll
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

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
