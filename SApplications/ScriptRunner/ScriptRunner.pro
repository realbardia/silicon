# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/apps/ScriptRunner
    LIBS += ../../../build/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/apps/ScriptRunner
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/apps/ScriptRunner
    LIBS += ../../../build/bin/libsiliconlib.dll
}
QT += widgets
INCLUDEPATH += ../../Libs/SiliconLib
TEMPLATE = lib
SOURCES += body.cpp \
    scriptrunner.cpp \
    runnerpage.cpp
HEADERS += \ 
    scriptrunner.h \
    runnerpage.h
RESOURCES += scriptrunner.qrc

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/apps/

INSTALLS += target
