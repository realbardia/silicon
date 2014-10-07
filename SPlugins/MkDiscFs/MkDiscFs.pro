# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/plugins/MkDiscFs
    LIBS += ../../../build/lib/libSiliconLib.so \
        ../../../build/lib/libsidi.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/plugins/MkDiscFs
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        ../../../build/bin/silicon.app/Contents/lib/libsidi.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/plugins/MkDiscFs
    LIBS += ../../../build/bin/libsiliconlib.dll \
        ../../../build/bin/sidi.dll
}

INCLUDEPATH += ../../Libs/SiliconLib \
    ../../Libs/SiDi

TEMPLATE = lib
SOURCES += body.cpp \
    plugin.cpp \
    mkdiscfsengine.cpp \
    mkdiscfs.cpp
HEADERS += \ 
    plugin.h \
    mkdiscfsengine.h \
    mkdiscfs.h
RESOURCES += mkdiscfs.qrc

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/plugins/

INSTALLS += target
