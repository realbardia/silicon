# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/apps/DiscDetails
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so \
        ../../../build-linux/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/apps/DiscDetails
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32{
    TARGET = ../../../build-linux/lib/silicon/apps/DiscDetails
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll \
        ../../../build-windows/bin/sdatabase.dll
}
QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi \
    ../../Libs/SDataBase

TEMPLATE = lib
SOURCES += body.cpp \
    discdetails.cpp \
    deviceitem.cpp
HEADERS += \ 
    discdetails.h \
    deviceitem.h
RESOURCES += discdetails.qrc

FORMS += \
    device.ui
