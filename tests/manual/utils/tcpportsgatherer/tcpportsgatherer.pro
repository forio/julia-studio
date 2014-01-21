TEMPLATE = app
TARGET = tcpportsgatherer

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console
CONFIG -= app_bundle

include(../../../../julia-studio.pri)
include(../../../../src/rpath.pri)
include($$IDE_SOURCE_TREE/src/libs/utils/utils.pri)


INCLUDEPATH += ../../../../src/libs
UTILSDIR = ../../../../src/libs/utils

DEFINES += QTCREATOR_UTILS_STATIC_LIB

HEADERS += \
    $${UTILSDIR}/portlist.h \
    $${UTILSDIR}/tcpportsgatherer.h
SOURCES += \
    $${UTILSDIR}/portlist.cpp \
    $${UTILSDIR}/tcpportsgatherer.cpp

win32:LIBS += -liphlpapi -lWs2_32
SOURCES += main.cpp
