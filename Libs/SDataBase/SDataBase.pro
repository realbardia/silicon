# -------------------------------------------------
# Project created by QtCreator 2009-11-16T13:40:50
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/sdatabase
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/sdatabase
}
win32 {
    TARGET = ../../../../build/bin/sdatabase
}

unix {
    VERSION = 0.8.00
}

QT += sql widgets
QT += testlib

INCLUDEPATH += qsql_sqlite.cpp
TEMPLATE = lib
DEFINES += SDATABASE_LIBRARY

SOURCES += sdatabase.cpp \
    sbuffer.cpp \
    sdynamicstorage.cpp \
    sstaticstorage.cpp \
    sinireader.cpp \
    sfilestringlist.cpp \
    sfileinfo.cpp \
    smusicinfo.cpp \
    svideoinfo.cpp \
    spictureinfo.cpp \
    sdocinfo.cpp \
    sdiscinfo.cpp \
    ssqlconnect.cpp \
    sdbtable.cpp \
    ssql.cpp \
    stinyfileinfo.cpp
HEADERS += sdatabase.h \
    sbuffer.h \
    sdynamicstorage.h \
    sstaticstorage.h \
    sinireader.h \
    sfilestringlist.h \
    sdatabase_global.h \
    sfileinfo.h \
    smusicinfo.h \
    svideoinfo.h \
    spictureinfo.h \
    sdocinfo.h \
    sdiscinfo.h \
    ssqlconnect.h \
    sdatabase_prev.h \
    sdbtable.h \
    ssql.h \
    stinyfileinfo.h

isEmpty(PREFIX) {
    PREFIX = /usr
}

contains(BUILD_MODE,opt) {
    LIBS_PATH = lib/
} else {
    contains(QMAKE_HOST.arch, x86_64):{
        LIBS_PATH = lib/x86_64-linux-gnu/
    } else {
        LIBS_PATH = lib/i386-linux-gnu/
    }
}

target = $$TARGET
target.path = $$PREFIX/$$LIBS_PATH
headers.files = $$HEADERS
headers.path = $$PREFIX/include/silicon/database

INSTALLS += target headers
