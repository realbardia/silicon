# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/Library
    LIBS += ../../../build/lib/libsilicon.so \
        ../../../build/lib/libsidi.so \
        ../../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/Library
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libsilicon.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 {
    TARGET = ../../../../build/lib/silicon/apps/Library
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll \
        ../../../build/bin/sdatabase.dll
}
QT += widgets
TEMPLATE = lib
INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi \
    ../../Libs/SDataBase

SOURCES += body.cpp \
    formatlist.cpp \
    charlist.cpp \
    librarydb.cpp \
    tagslist.cpp \
    libraryui.cpp \
    itemlist.cpp \
    itempreview.cpp

HEADERS += \
    formatlist.h \
    charlist.h \
    librarydb.h \
    tagslist.h \
    libraryui.h \
    itemlist.h \
    itempreview.h

RESOURCES += \
    library.qrc

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
