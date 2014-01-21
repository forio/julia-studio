TARGET = qml2puppet

TEMPLATE = app

include(../../../../julia-studio.pri)

DESTDIR = $$IDE_BIN_PATH
include(../../../rpath.pri)

include(../../../../share/julia-studio/qml/qmlpuppet/qml2puppet/qml2puppet.pri)
