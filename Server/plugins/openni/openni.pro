TEMPLATE = lib
CONFIG += plugin

TARGET = openni_plugin


INCLUDEPATH += /usr/include/ni ../../interfaces ../../logic
LIBS += -lOpenNI

SOURCES += openniobject.cpp

HEADERS += openniobject.h


