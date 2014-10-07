#-------------------------------------------------
#
# Project created by QtCreator 2010-08-12T19:58:09
#
#-------------------------------------------------

unix:!macx {
    TARGET = ../../../build/lib/SiliconLib
    LIBS += ../../../build/lib/libsdatabase.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/siliconlib
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libsdatabase.dylib
}
win32 {
    TARGET = ../../../../build/bin/libsiliconlib
    LIBS += ../../../build/bin/sdatabase.dll

    SOURCES += QtWin/qtwin.cpp
    HEADERS += QtWin/qtwin.h
}

unix {
    VERSION = 0.8.00
}

QT += widgets
TEMPLATE = lib
INCLUDEPATH += ../SDataBase

DEFINES += LIBSILICON_LIBRARY

HEADERS += libsilicon_global.h \
    svariableconverter.h \
    sanimation.h \
    sumount.h \
    stargz.h \
    stabitem.h \
    stabbar.h \
    sstylesheet.h \
    ssliderwidget.h \
    sscrollwidget.h \
    srootprocess.h \
    srollmenu.h \
    sprogresslistitem.h \
    spicturewidget.h \
    soptionbox.h \
    smount.h \
    smenu.h \
    sitemlist.h \
    sinfowidget.h \
    silitab.h \
    silistacked.h \
    sfoldergraph.h \
    scontextbutton.h \
    scolor.h \
    scatwidget.h \
    sargumentlist.h \
    sapplication.h \
    saboutdata.h \
    sfilelistwidget.h \
    smastericons.h \
    sauthorinfo.h \
    QProgressIndicator/QProgressIndicator.h \
    simage.h \
    smenupanel.h \
    sinnerprogressbar.h \
    sfilterlist.h \
    sfilemanager.h \
    silicon.h \
    QtSystemInfo/QtSystemInfo.h \
    spushbutton.h \
    sstring.h \
    sstringlist.h \
    scolorbutton.h \
    sthumbnailbar.h \
    sdrawer.h \
    sactiontabbar.h \
    splugin.h \
    sinnermessage.h \
    sdesktopfunctions.h \
    spage.h \
    libsilicon_prev.h \
    sdialogmethod.h \
    sdialog.h \
    sgraphicstack.h \
    sdialogtools.h \
    sgraphictimer.h \
    scombobox.h \
    spicturewidget_prev.h \
    swidgetlist_prev.h \
    swidgetlist.h \
    sautohidewidget.h \
    swidgetmanager.h \
    swidgetpreview.h \
    sdialogpointer.h

SOURCES += svariableconverter.cpp \
    sanimation.cpp \
    sumount.cpp \
    stargz.cpp \
    stabitem.cpp \
    stabbar.cpp \
    sstylesheet.cpp \
    ssliderwidget.cpp \
    sscrollwidget.cpp \
    srootprocess.cpp \
    srollmenu.cpp \
    sprogresslistitem.cpp \
    spicturewidget.cpp \
    soptionbox.cpp \
    smount.cpp \
    smenu.cpp \
    sitemlist.cpp \
    sinfowidget.cpp \
    silitab.cpp \
    silistacked.cpp \
    sfoldergraph.cpp \
    scontextbutton.cpp \
    scolor.cpp \
    scatwidget.cpp \
    sargumentlist.cpp \
    sapplication.cpp \
    saboutdata.cpp \
    sfilelistwidget.cpp \
    smastericons.cpp \
    sauthorinfo.cpp \
    QProgressIndicator/QProgressIndicator.cpp \
    simage.cpp \
    smenupanel.cpp \
    sinnerprogressbar.cpp \
    sfilterlist.cpp \
    sfilemanager.cpp \
    silicon.cpp \
    QtSystemInfo/QtSystemInfo.cpp \
    spushbutton.cpp \
    sstring.cpp \
    sstringlist.cpp \
    scolorbutton.cpp \
    sthumbnailbar.cpp \
    sdrawer.cpp \
    sactiontabbar.cpp \
    splugin.cpp \
    sinnermessage.cpp \
    sdesktopfunctions.cpp \
    spage.cpp \
    sdialogmethod.cpp \
    sdialog.cpp \
    sgraphicstack.cpp \
    sdialogtools.cpp \
    sgraphictimer.cpp \
    scombobox.cpp \
    swidgetlist.cpp \
    sautohidewidget.cpp \
    swidgetmanager.cpp \
    swidgetpreview.cpp \
    sdialogpointer.cpp

isEmpty(PREFIX) {
    PREFIX = /usr
}

contains(QMAKE_HOST.arch, x86_64):{
    LIBS_PATH = lib/x86_64-linux-gnu/
} else {
    LIBS_PATH = lib/i386-linux-gnu/
}

target = $$TARGET
target.path = $$PREFIX/$$LIBS_PATH
headers.files = $$HEADERS
headers.path = $$PREFIX/include/silicon/siliconlib

INSTALLS += target headers
