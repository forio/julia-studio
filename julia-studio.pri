!isEmpty(QTCREATOR_PRI_INCLUDED):error("julia-studio.pri already included")
QTCREATOR_PRI_INCLUDED = 1

QTCREATOR_VERSION = 2.6.81
JULIA_STUDIO_VERSION = 0.4.4
JULIA_STUDIO_VERSION_STR = "$$JULIA_STUDIO_VERSION"

isEqual(QT_MAJOR_VERSION, 5) {

defineReplace(cleanPath) {
    return($$clean_path($$1))
}

defineReplace(targetPath) {
    return($$shell_path($$1))
}

} else { # qt5

defineReplace(cleanPath) {
    win32:1 ~= s|\\\\|/|g
    contains(1, ^/.*):pfx = /
    else:pfx =
    segs = $$split(1, /)
    out =
    for(seg, segs) {
        equals(seg, ..):out = $$member(out, 0, -2)
        else:!equals(seg, .):out += $$seg
    }
    return($$join(out, /, $$pfx))
}

defineReplace(targetPath) {
    return($$replace(1, /, $$QMAKE_DIR_SEP))
}

} # qt5

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

isEqual(QT_MAJOR_VERSION, 5) {

# For use in custom compilers which just copy files
defineReplace(stripSrcDir) {
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

} else { # qt5

# For use in custom compilers which just copy files
win32:i_flag = i
defineReplace(stripSrcDir) {
    win32 {
        !contains(1, ^.:.*):1 = $$OUT_PWD/$$1
    } else {
        !contains(1, ^/.*):1 = $$OUT_PWD/$$1
    }
    out = $$cleanPath($$1)
    out ~= s|^$$re_escape($$_PRO_FILE_PWD_/)||$$i_flag
    return($$out)
}

} # qt5

isEmpty(TEST):CONFIG(debug, debug|release) {
    !debug_and_release|build_pass {
        TEST = 1
    }
}

isEmpty(IDE_LIBRARY_BASENAME) {
    IDE_LIBRARY_BASENAME = lib
}

equals(TEST, 1) {
    QT +=testlib
    DEFINES += WITH_TESTS
}

IDE_SOURCE_TREE = $$PWD
isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$cleanPath($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
IDE_APP_PATH = $$IDE_BUILD_TREE/bin
macx {
    IDE_APP_TARGET   = "Julia Studio"
    IDE_LIBRARY_PATH = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/PlugIns
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH
    IDE_LIBEXEC_PATH = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/Resources
    IDE_DATA_PATH    = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/Resources
    IDE_DOC_PATH     = $$IDE_DATA_PATH/doc
    IDE_BIN_PATH     = $$IDE_APP_PATH/$${IDE_APP_TARGET}.app/Contents/MacOS
    copydata = 1
    isEmpty(TIGER_COMPAT_MODE):TIGER_COMPAT_MODE=$$(QTC_TIGER_COMPAT)
    isEmpty(TIGER_COMPAT_MODE) {
        QMAKE_CXXFLAGS *= -mmacosx-version-min=10.5
        QMAKE_LFLAGS *= -mmacosx-version-min=10.5
    }
} else {
    contains(TEMPLATE, vc.*):vcproj = 1
    IDE_APP_TARGET   = JuliaStudio
    IDE_LIBRARY_PATH = $$IDE_BUILD_TREE/$$IDE_LIBRARY_BASENAME/julia-studio
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH/plugins
    IDE_LIBEXEC_PATH = $$IDE_APP_PATH # FIXME
    IDE_DATA_PATH    = $$IDE_BUILD_TREE/share/julia-studio
    IDE_DOC_PATH     = $$IDE_BUILD_TREE/share/doc/qtcreator
    IDE_BIN_PATH     = $$IDE_APP_PATH
    !isEqual(IDE_SOURCE_TREE, $$IDE_BUILD_TREE):copydata = 1
}

INCLUDEPATH += \
    $$IDE_BUILD_TREE/src \ # for <app/app_version.h>
    $$IDE_SOURCE_TREE/src/libs \
    $$IDE_SOURCE_TREE/tools \
    $$IDE_SOURCE_TREE/src/plugins

CONFIG += depend_includepath

LIBS += -L$$IDE_LIBRARY_PATH

!isEmpty(vcproj) {
    DEFINES += IDE_LIBRARY_BASENAME=\"$$IDE_LIBRARY_BASENAME\"
} else {
    DEFINES += IDE_LIBRARY_BASENAME=\\\"$$IDE_LIBRARY_BASENAME\\\"
}

#DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += QT_NO_CAST_TO_ASCII
!macx:DEFINES += QT_USE_FAST_OPERATOR_PLUS QT_USE_FAST_CONCATENATION

unix {
    CONFIG(debug, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
    CONFIG(release, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

    CONFIG(debug, debug|release):MOC_DIR = $${OUT_PWD}/.moc/debug-shared
    CONFIG(release, debug|release):MOC_DIR = $${OUT_PWD}/.moc/release-shared

    RCC_DIR = $${OUT_PWD}/.rcc
    UI_DIR = $${OUT_PWD}/.uic
}

win32-msvc* {
    #Don't warn about sprintf, fopen etc being 'unsafe'
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

qt:greaterThan(QT_MAJOR_VERSION, 4) {
    contains(QT, core): QT += concurrent
    contains(QT, gui): QT += widgets
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900
}

OTHER_FILES += $$replace(_PRO_FILE_, \\.pro$, .qbs)
