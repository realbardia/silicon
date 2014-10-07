# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/DiscImaging
    LIBS += ../../../build/lib/libSiliconLib.so \
        ../../../build/lib/libsidi.so \
        ../../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/DiscImaging
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/DiscImaging
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
    discimaging.cpp \
    mainpage.cpp
HEADERS += \ 
    discimaging.h \
    mainpage.h
RESOURCES += discimaging.qrc

FORMS += \
    extrawidget.ui \
    options.ui

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
