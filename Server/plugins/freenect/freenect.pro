TEMPLATE = lib
CONFIG += plugin link_pkgconfig

PKGCONFIG += libfreenect opencv

TARGET = freenect_plugin


INCLUDEPATH += ../../interfaces ../../logic

SOURCES +=  kinect.cpp \
	    dataprocessor.cpp

HEADERS +=  kinect.h \
	    dataprocessor.h
