TEMPLATE = lib
TARGET = Welcome
QT += network declarative

include(../../qtcreatorplugin.pri)
include(welcome_dependencies.pri)

HEADERS += welcomeplugin.h \
    welcome_global.h

SOURCES += welcomeplugin.cpp

DEFINES += WELCOME_LIBRARY

IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=$$DESTDIR

QML_IMPORT_PATH = $$IDE_SOURCE_TREE/lib/qtcreator/
