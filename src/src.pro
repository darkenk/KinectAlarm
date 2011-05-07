TEMPLATE = subdirs

SUBDIRS += \
    Server \
    Server/plugins/openni \
    Server/plugins/freenect

unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop service iconxpm icon26 icon48 icon64

  target.path =$$BINDIR

#  desktop.path = $$DATADIR/applications/hildon
#  desktop.files += $${TARGET}.desktop

#  service.path = $$DATADIR/dbus-1/services
#  service.files += $${TARGET}.service

#  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
#  icon64.files += ../data/64x64/$${TARGET}.png
}

include(../common.pri)
