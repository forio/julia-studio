TEMPLATE = app
TARGET = julia-studio.sh
OBJECTS_DIR =

PRE_TARGETDEPS = $$PWD/julia-studio.sh

QMAKE_LINK = cp $$PWD/julia-studio.sh $@ && : IGNORE REST OF LINE:
QMAKE_STRIP =

QMAKE_CLEAN = julia-studio.sh

target.path  = $$QTC_PREFIX/bin
INSTALLS    += target

OTHER_FILES = $$PWD/julia-studio.sh
