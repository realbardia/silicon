# -------------------------------------------------
# Project created by QtCreator 2010-02-09T15:18:01
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/sidi
    INCLUDEPATH += /usr/include/taglib
    LIBS += ../../../build-linux/lib/libsdatabase.so \
        ../../../build-linux/lib/libSiliconLib.so \
        -ltag
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/sidi
    INCLUDEPATH += /usr/include/taglib
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libsdatabase.dylib \
        ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib \
        -ltag
}
win32 {
    TARGET = ../../../../build-windows/bin/sidi
    INCLUDEPATH += ../../../include/taglib
    LIBS += ../../../build-windows/bin/sdatabase.dll \
        ../../../build-windows/bin/libsiliconlib.dll \
        ../../../build-windows/bin/libtag.dll

    INCLUDEPATH += ../../../include/taglib
}

unix {
    VERSION = 0.8.00
}

QT += testlib widgets

TEMPLATE = lib
INCLUDEPATH += ../SDataBase \
    ../SiliconLib

DEFINES += SIDI_LIBRARY
unix:DEFINES += IS_LINUX
win32:DEFINES += IS_WINDOWS
SOURCES += sdeviceitem.cpp \
    sisoinfo.cpp \
    sdevicefeatures.cpp \
    sdevicelist.cpp \
    sdiscfeatures.cpp \
    sabstractdevicenotifier.cpp \
    sscandisc.cpp \
    sidi_prev.cpp \
    sabstractdiscburner.cpp \
    siditools.cpp \
    sabstractprocessengine.cpp \
    sabstractprocess.cpp \
    sprocessevent.cpp \
    sabstractdiscreader.cpp \
    sbuscontroller.cpp \
    stime.cpp \
    sabstractdataburner.cpp \
    sabstractimagecreator.cpp \
    sdiscdetector.cpp \
    sabstractdisceraser.cpp \
    sabstractconverter.cpp \
    sconverter.cpp \
    sabstractconverterengine.cpp \
    sabstractimagemounter.cpp \
    sabstractaudioburner.cpp
HEADERS += SiDi_global.h \
    sdeviceitem.h \
    sisoinfo.h \
    sdevicefeatures.h \
    sdevicelist.h \
    sdiscfeatures.h \
    sabstractdevicenotifier.h \
    sscandisc.h \
    sidi_prev.h \
    sabstractdiscburner.h \
    siditools.h \
    sabstractprocessengine.h \
    sabstractprocess.h \
    sprocessevent.h \
    sabstractdiscreader.h \
    sbuscontroller.h \
    stime.h \
    sabstractdataburner.h \
    sabstractimagecreator.h \
    sdiscdetector.h \
    sabstractdisceraser.h \
    sabstractconverter.h \
    sconverter.h \
    sabstractconverterengine.h \
    sabstractimagemounter.h \
    sabstractaudioburner.h
