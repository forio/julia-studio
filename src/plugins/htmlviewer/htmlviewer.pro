TARGET = HtmlViewer
TEMPLATE = lib

DEFINES += HTMLVIEWER_LIBRARY

# HtmlViewer files

SOURCES += htmlviewerplugin.cpp

HEADERS += htmlviewerplugin.h\
        htmlviewer_global.h\
        htmlviewerconstants.h

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=../../../

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=$$DESTDIR

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac

PROVIDER = Forio

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/coreplugin/coreplugin.pri)

LIBS += -L$$IDE_PLUGIN_PATH/QtProject

RESOURCES += \
    htmlviewer.qrc

HEADERS += \
    htmlviewerfactory.h

SOURCES += \
    htmlviewerfactory.cpp

HEADERS += \
    htmlviewer.h

SOURCES += \
    htmlviewer.cpp

