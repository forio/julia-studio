# ------------------------------------------------
# Sample Fervor application
# ------------------------------------------------

QT += core gui

TARGET = Sample
VERSION = 1.0
TEMPLATE = app

mac:ICON = sample.icns
win32:RC_FILE = sample.rc


# Fervor autoupdater
# (set TARGET and VERSION of your app before including Fervor.pri)
!include("../Fervor.pri") {
	error("Unable to include Fervor autoupdater.")
}


# The sample application
SOURCES += main.cpp \
		mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui
