TARGET = JuliaEditor
TEMPLATE = lib

QT += network

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=/Users/westley/Code/qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=/Users/westley/Code/qtcreator-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\Nokia\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/Nokia/qtcreator" or "~/.local/share/Nokia/qtcreator" on Linux
##    "~/Library/Application Support/Nokia/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = Forio

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/coreplugin/coreplugin.pri)
include($$QTCREATOR_SOURCES/src/plugins/texteditor/texteditor.pri)
include(juliaeditor_dependencies.pri)

DEFINES += JULIAEDITOR_LIBRARY

SOURCES += juliaeditorplugin.cpp\
        juliaconsolemanager.cpp\
        juliaeditor.cpp\
        console.cpp\
        run.cpp\
        juliaconsolepane.cpp

HEADERS += juliaeditorplugin.h\
        juliaeditor_global.h\
        juliaeditor_constants.h\
        juliaeditor.h\
        juliaconsolemanager.h\
        console.h\
        run.h\
        juliaconsolepane.h

OTHER_FILES += juliaeditor.pluginspec\
            juliaeditor.mimetypes.xml

RESOURCES += juliaeditor.qrc

LIBS += -L$$IDE_PLUGIN_PATH/Nokia

OTHER_FILES += \
    julia.xml




