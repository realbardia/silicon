# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/sampleApp
    LIBS += ../../../build/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/sampleApp
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/sampleApp
    LIBS += ../../../build/bin/libsiliconlib.dll
}
QT += widgets

INCLUDEPATH += ../../Libs/SiliconLib
TEMPLATE = lib
SOURCES += body.cpp
HEADERS += 
RESOURCES += sample.qrc
