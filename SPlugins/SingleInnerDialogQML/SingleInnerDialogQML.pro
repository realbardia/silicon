# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build-linux/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build-linux/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build-osx/bin/silicon.app/Contents/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build-osx/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build-windows/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build-windows/bin/libsiliconlib.dll
}

INCLUDEPATH += ../../Libs/SiliconLib

QT += declarative gui

TEMPLATE = lib
SOURCES += body.cpp \
    plugin.cpp \
    dmanager.cpp \
    dialogwidget.cpp
HEADERS += \ 
    plugin.h \
    dmanager.h \
    dialogwidget.h
RESOURCES += singleinnerdialogqml.qrc

OTHER_FILES += \
    qml/ShadowRectangle.qml \
    qml/main.qml
