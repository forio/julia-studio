TEMPLATE = lib

SOURCES += plugin3.cpp
HEADERS += plugin3.h

OTHER_FILES = $$PWD/plugin.spec

include(../../../../../../julia-studio.pri)
include(../../../../../../src/libs/extensionsystem/extensionsystem.pri)
include(../../../../qttestrpath.pri)

COPYDIR = $$OUT_PWD
COPYFILES = $$OTHER_FILES
include(../../../copy.pri)

TARGET = $$qtLibraryName(plugin3)
DESTDIR = $$OUT_PWD

LIBS += -L$$OUT_PWD/../plugin2
LIBS += -l$$qtLibraryName(plugin2)

macx {
    QMAKE_LFLAGS_SONAME = -Wl,-install_name,$${OUT_PWD}/
} else:unix {
    QMAKE_RPATHDIR += $OUT_PWD/../lib
}
