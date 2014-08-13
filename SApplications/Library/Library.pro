# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/apps/Library
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so \
        ../../../build-linux/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/apps/Library
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 {
    TARGET = ../../../../build-windows/lib/silicon/apps/Library
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll \
        ../../../build-windows/bin/sdatabase.dll
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
