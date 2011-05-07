TEMPLATE = lib
CONFIG += plugin

TARGET = openni_plugin
DESTDIR = ../


INCLUDEPATH += /usr/include/ni ../../interfaces ../../logic
LIBS += -lOpenNI

SOURCES += openniobject.cpp

HEADERS += openniobject.h

include(../../../../common.pri)

DESTDIR = $$DESTDIR/plugins

target.path = $$PREFIX/share/kinectalarm/plugins

INSTALLS += target


