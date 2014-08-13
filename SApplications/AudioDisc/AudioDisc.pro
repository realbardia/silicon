# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/apps/audiodisc
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so \
        ../../../build-linux/lib/libsdatabase.so \
        -ltag

    INCLUDEPATH += /usr/include/taglib
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/apps/audiodisc
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib \
        -ltag

    INCLUDEPATH += /usr/include/taglib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/apps/audiodisc
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll \
        ../../../build-windows/bin/sdatabase.dll \
        ../../../build-windows/bin/libtag.dll

    INCLUDEPATH += ../../../include/taglib
}
QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi \
    ../../Libs/SDataBase

TEMPLATE = lib
SOURCES += body.cpp \ 
    audiodisc.cpp \
    audioselector.cpp \
    audioitem.cpp \
    optionsselect.cpp

RESOURCES += audiodisc.qrc

FORMS += extrawidget.ui \
    options.ui

HEADERS += \
    audiodisc.h \
    audioselector.h \
    audioitem.h \
    optionsselect.h
