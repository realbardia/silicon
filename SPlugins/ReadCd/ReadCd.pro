# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/plugins/ReadCd
    LIBS += ../../../build-linux/lib/libSiliconLib.so \
        ../../../build-linux/lib/libsidi.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/ReadCd
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/plugins/ReadCd
    LIBS += ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/sidi.dll
}

INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

TEMPLATE = lib
SOURCES += body.cpp \
    plugin.cpp \
    readcdengine.cpp \
    readcd.cpp
HEADERS += \ 
    plugin.h \
    readcdengine.h \
    readcd.h
RESOURCES += readcd.qrc
