# -------------------------------------------------
# Project created by QtCreator 2009-12-09T22:02:26
# -------------------------------------------------
unix:!macx {
    TARGET = ../../../build/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build/lib/libSiliconLib.so
}
macx {
    TARGET = ../../../build/bin/silicon.app/Contents/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build/bin/silicon.app/Contents/lib/libSiliconLib.dylib
}
win32{
    TARGET = ../../../../build/lib/silicon/plugins/SingleInnerDialogQML
    LIBS += ../../../build/bin/libsiliconlib.dll
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

isEmpty(PREFIX) {
    PREFIX = /usr
}

target = $$TARGET
target.path = $$PREFIX/lib/silicon/plugins/

INSTALLS += target
