# -------------------------------------------------
# Project created by QtCreator 2009-11-16T13:40:50
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/sdatabase
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/sdatabase
}
win32 {
    TARGET = ../../../../build-windows/bin/sdatabase
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
