QT += core gui webkit network

qt:greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
}

isEmpty(FV_APP_NAME) {
	warning("Fervor: falling back to application name '$$TARGET'")
        DEFINES += FV_APP_NAME=\\\"$$TARGET\\\"
} else {
	message("Fervor: building for application name '$$FV_APP_NAME'")
        DEFINES += FV_APP_NAME=\\\"$$FV_APP_NAME\\\"
}

isEmpty(FV_APP_VERSION) {
	warning("Fervor: falling back to application version '$$VERSION'")
        DEFINES += FV_APP_VERSION=\\\"$$VERSION\\\"
} else {
	message("Fervor: building for application version '$$FV_APP_VERSION'")
        DEFINES += FV_APP_VERSION=\\\"$$FV_APP_VERSION\\\"
}

# Unit tests
#DEFINES += FV_DEBUG=1
#DEPENDPATH += "$$PWD/tests/"
#INCLUDEPATH += "$$PWD/tests/"
#CONFIG += qtestlib
#SOURCES += ../../fervor/tests/fvversioncomparatortest.cpp
#HEADERS += ../../fervor/tests/fvversioncomparatortest.h

DEPENDPATH += "$$PWD"
INCLUDEPATH += "$$PWD"

SOURCES += ../../fervor/fvupdatewindow.cpp \
	../../fervor/fvupdater.cpp \
	../../fervor/fvversioncomparator.cpp \
	../../fervor/fvplatform.cpp \
	../../fervor/fvignoredversions.cpp \
	../../fervor/fvavailableupdate.cpp \
	../../fervor/fvupdateconfirmdialog.cpp

HEADERS += ../../fervor/fvupdatewindow.h \
	../../fervor/fvupdater.h \
	../../fervor/fvversioncomparator.h \
	../../fervor/fvplatform.h \
	../../fervor/fvignoredversions.h \
	../../fervor/fvavailableupdate.h \
	../../fervor/fvupdateconfirmdialog.h

FORMS += ../../fervor/fvupdatewindow.ui \
	../../fervor/fvupdateconfirmdialog.ui

TRANSLATIONS += ../../fervor/fervor_lt.ts
CODECFORTR = UTF-8
