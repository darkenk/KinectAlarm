#-------------------------------------------------
#
# Project created by QtCreator 2011-03-26T21:58:50
#
#-------------------------------------------------

QT       += core gui opengl thread network xml

TARGET = Server
TEMPLATE = app


INCLUDEPATH += /usr/include/ni ./logic ./ui ./interfaces
#LIBS += -lOpenNI

SOURCES += main.cpp\
	ui/mainwindow.cpp \
#	logic/openniobject.cpp \
    logic/kinectsubject.cpp \
    ui/kinectimagewidget.cpp \
    ui/alarmtrayicon.cpp \
    ui/settingsdialog.cpp \
    logic/harddrivestorage.cpp \
    logic/kinectpluginloader.cpp \
    logic/picasastorage.cpp \
    logic/storage.cpp

HEADERS  += ui/mainwindow.h \
#    logic/openniobject.h \
    interfaces/ikinectobserver.h \
    logic/kinectsubject.h \
    interfaces/ikinectobservable.h \
    interfaces/ikinect.h \
    ui/kinectimagewidget.h \
    ui/alarmtrayicon.h \
    ui/settingsdialog.h \
    logic/harddrivestorage.h \
    logic/kinectpluginloader.h \
    logic/picasastorage.h \
    logic/storage.h \
    interfaces/ikinectstorage.h \
    kinectglobal.h

FORMS    += ui/mainwindow.ui \
    ui/settingsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    config.xml \
    home1.png \
    home_addblocked.png
