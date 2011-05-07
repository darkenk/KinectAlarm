TEMPLATE = lib
CONFIG += plugin link_pkgconfig

PKGCONFIG += libfreenect opencv

TARGET = freenect_plugin
#DESTDIR = ../


INCLUDEPATH += ../../interfaces ../../logic

SOURCES +=  kinect.cpp \
	    dataprocessor.cpp

HEADERS +=  kinect.h \
	    dataprocessor.h

include(../../../../common.pri)

DESTDIR = $$DESTDIR/plugins

target.path = $$PREFIX/share/kinectalarm/plugins

INSTALLS += target
