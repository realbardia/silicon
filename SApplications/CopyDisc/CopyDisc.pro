# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/CopyDisc
    LIBS += ../../../build/lib/libSiliconLib.so \
        ../../../build/lib/libsidi.so \
        ../../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/CopyDisc
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/CopyDisc
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll \
        ../../../build/bin/sdatabase.dll
}
QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi \
    ../../Libs/SDataBase

TEMPLATE = lib
SOURCES += body.cpp \
    copydisc.cpp \
    mainpage.cpp
HEADERS += \ 
    copydisc.h \
    mainpage.h
RESOURCES += copydisc.qrc

FORMS += \
    extrawidget.ui \
    options.ui

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
