# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/DiscEraser
    LIBS += ../../../build/lib/libSiliconLib.so \
        ../../../build/lib/libsidi.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/DiscEraser
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/DiscEraser
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll
}
QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

TEMPLATE = lib
SOURCES += body.cpp \
    disceraser.cpp \
    mainpage.cpp
HEADERS += disceraser.h \
    mainpage.h
RESOURCES += disceraser.qrc

FORMS += options.ui

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
