TARGET = JuliaEditor
TEMPLATE = lib

QT += network

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=../../../../qt-creator

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)

#CONFIG( debug ) {
  isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=../../../../qtcreator-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Debug
#}
#else {
#  isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=../../../../qtcreator-build-Desktop-Release
#}

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
include($$QTCREATOR_SOURCES/src/plugins/projectexplorer/projectexplorer.pri)
include(juliaeditor_dependencies.pri)

DEFINES += JULIAEDITOR_LIBRARY

OTHER_FILES += juliaeditor.pluginspec\
            juliaeditor.mimetypes.xml

RESOURCES += juliaeditor.qrc

LIBS += -L$$IDE_PLUGIN_PATH/Nokia

OTHER_FILES += \
    julia.xml
FORMS += \
    juliasettingspage.ui

SOURCES += juliaeditorplugin.cpp\
        juliaeditor.cpp\
        console.cpp\
        localevaluator.cpp\
        juliaconsolepane.cpp\
        juliasettingspage.cpp

HEADERS += juliaeditorplugin.h\
        juliaeditor_global.h\
        juliaeditor_constants.h\
        juliaeditor.h\
        console.h\
        localevaluator.h\
        juliaconsolepane.h\
        juliasettingspage.h\
        singleton.h\
        creationpolicies.h\
        lifetimepolicies.h

HEADERS += \
    juliaruncontrolfactory.h

SOURCES += \
    juliaruncontrolfactory.cpp

HEADERS += \
    juliarunconfiguration.h

SOURCES += \
    juliarunconfiguration.cpp

HEADERS += \
    juliarunconfigurationfactory.h

SOURCES += \
    juliarunconfigurationfactory.cpp

HEADERS += \
    juliadummyproject.h

SOURCES += \
    juliadummyproject.cpp

HEADERS += \
    juliaprojectmanager.h

SOURCES += \
    juliaprojectmanager.cpp

HEADERS += \
    juliafilewizard.h

SOURCES += \
    juliafilewizard.cpp

HEADERS += \
    commandhistoryview.h

SOURCES += \
    commandhistoryview.cpp
