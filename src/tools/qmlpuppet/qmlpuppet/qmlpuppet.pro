TARGET = qmlpuppet

TEMPLATE = app

include(../../../../julia-studio.pri)
include(../../../private_headers.pri)
DESTDIR = $$IDE_BIN_PATH
include(../../../rpath.pri)

include(../../../../share/julia-studio/qml/qmlpuppet/qmlpuppet/qmlpuppet.pri)
