# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/audiodisc
    LIBS += ../../../build/lib/libsilicon.so \
        ../../../build/lib/libsidi.so \
        ../../../build/lib/libsdatabase.so \
        -ltag

    INCLUDEPATH += /usr/include/taglib
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/audiodisc
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libsilicon.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib \
        -ltag

    INCLUDEPATH += /usr/include/taglib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/audiodisc
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll \
        ../../../build/bin/sdatabase.dll \
        ../../../build/bin/libtag.dll

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

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
