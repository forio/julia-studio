# USE .subdir AND .depends !
# OTHERWISE PLUGINS WILL BUILD IN WRONG ORDER (DIRECTORIES ARE COMPILED IN PARALLEL)

TEMPLATE  = subdirs

SUBDIRS   = plugin_coreplugin \
            plugin_welcome \
            plugin_find \
            plugin_texteditor \
            plugin_cppeditor \
            plugin_bineditor \
            plugin_imageviewer \
            plugin_bookmarks \
            plugin_projectexplorer \
            plugin_vcsbase \
            plugin_perforce \
            plugin_subversion \
            plugin_git \
            plugin_cvs \
            plugin_cpptools \
            plugin_qtsupport \
            plugin_locator \
            plugin_help \
            plugin_cpaster \
            plugin_autotoolsprojectmanager \
            plugin_fakevim \
            plugin_genericprojectmanager \
            plugin_qmljseditor \
            plugin_glsleditor \
            plugin_mercurial \
            plugin_bazaar \
            plugin_tasklist \
            plugin_qmljstools \
            plugin_macros \
            plugin_todo \
            plugin_juliaeditor \
            plugin_htmlviewer

isEmpty(IDE_PACKAGE_MODE) {
    SUBDIRS += plugin_updateinfo

} else:!isEmpty(UPDATEINFO_ENABLE) {
    SUBDIRS += plugin_updateinfo
}
!macx:SUBDIRS += plugin_clearcase

include(../../qtcreator.pri)

plugin_coreplugin.subdir = coreplugin

plugin_updateinfo.subdir = updateinfo
plugin_updateinfo.depends = plugin_coreplugin

plugin_welcome.subdir = welcome
plugin_welcome.depends = plugin_coreplugin
plugin_welcome.depends += plugin_projectexplorer

plugin_find.subdir = find
plugin_find.depends += plugin_coreplugin

plugin_texteditor.subdir = texteditor
plugin_texteditor.depends = plugin_find
plugin_texteditor.depends += plugin_locator
plugin_texteditor.depends += plugin_coreplugin

plugin_cppeditor.subdir = cppeditor
plugin_cppeditor.depends = plugin_texteditor
plugin_cppeditor.depends += plugin_coreplugin
plugin_cppeditor.depends += plugin_cpptools

plugin_bineditor.subdir = bineditor
plugin_bineditor.depends = plugin_texteditor
plugin_bineditor.depends += plugin_coreplugin

plugin_imageviewer.subdir = imageviewer
plugin_imageviewer.depends = plugin_coreplugin

plugin_vcsbase.subdir = vcsbase
plugin_vcsbase.depends = plugin_find
plugin_vcsbase.depends += plugin_texteditor
plugin_vcsbase.depends += plugin_coreplugin
plugin_vcsbase.depends += plugin_projectexplorer
plugin_vcsbase.depends += plugin_cpptools

plugin_perforce.subdir = perforce
plugin_perforce.depends = plugin_vcsbase
plugin_perforce.depends += plugin_projectexplorer
plugin_perforce.depends += plugin_coreplugin

plugin_git.subdir = git
plugin_git.depends = plugin_vcsbase
plugin_git.depends += plugin_projectexplorer
plugin_git.depends += plugin_coreplugin

plugin_cvs.subdir = cvs
plugin_cvs.depends = plugin_vcsbase
plugin_cvs.depends += plugin_projectexplorer
plugin_cvs.depends += plugin_coreplugin

plugin_subversion.subdir = subversion
plugin_subversion.depends = plugin_vcsbase
plugin_subversion.depends += plugin_projectexplorer
plugin_subversion.depends += plugin_coreplugin

plugin_projectexplorer.subdir = projectexplorer
plugin_projectexplorer.depends = plugin_locator
plugin_projectexplorer.depends += plugin_find
plugin_projectexplorer.depends += plugin_coreplugin
plugin_projectexplorer.depends += plugin_texteditor

plugin_qtsupport.subdir = qtsupport
plugin_qtsupport.depends = plugin_projectexplorer

plugin_locator.subdir = locator
plugin_locator.depends = plugin_coreplugin

plugin_cpptools.subdir = cpptools
plugin_cpptools.depends = plugin_projectexplorer
plugin_cpptools.depends += plugin_coreplugin
plugin_cpptools.depends += plugin_texteditor
plugin_cpptools.depends += plugin_find

plugin_bookmarks.subdir = bookmarks
plugin_bookmarks.depends = plugin_projectexplorer
plugin_bookmarks.depends += plugin_coreplugin
plugin_bookmarks.depends += plugin_texteditor

plugin_fakevim.subdir = fakevim
plugin_fakevim.depends = plugin_coreplugin
plugin_fakevim.depends += plugin_texteditor

plugin_qtestlib.subdir = qtestlib
plugin_qtestlib.depends = plugin_projectexplorer
plugin_qtestlib.depends += plugin_coreplugin

plugin_help.subdir = help
plugin_help.depends = plugin_find
plugin_help.depends += plugin_locator
plugin_help.depends += plugin_coreplugin

plugin_regexp.subdir = regexp
plugin_regexp.depends = plugin_coreplugin

plugin_cpaster.subdir = cpaster
plugin_cpaster.depends = plugin_texteditor
plugin_cpaster.depends += plugin_coreplugin

plugin_autotoolsprojectmanager.subdir = autotoolsprojectmanager
plugin_autotoolsprojectmanager.depends = plugin_projectexplorer
plugin_autotoolsprojectmanager.depends += plugin_coreplugin
plugin_autotoolsprojectmanager.depends += plugin_cpptools
plugin_autotoolsprojectmanager.depends += plugin_qtsupport

plugin_genericprojectmanager.subdir = genericprojectmanager
plugin_genericprojectmanager.depends = plugin_texteditor
plugin_genericprojectmanager.depends += plugin_projectexplorer
plugin_genericprojectmanager.depends += plugin_cpptools
plugin_genericprojectmanager.depends += plugin_qtsupport

plugin_qmljseditor.subdir = qmljseditor
plugin_qmljseditor.depends = plugin_texteditor
plugin_qmljseditor.depends += plugin_coreplugin
plugin_qmljseditor.depends += plugin_projectexplorer
plugin_qmljseditor.depends += plugin_qmljstools

plugin_glsleditor.subdir = glsleditor
plugin_glsleditor.depends = plugin_texteditor
plugin_glsleditor.depends += plugin_coreplugin
plugin_glsleditor.depends += plugin_projectexplorer
plugin_glsleditor.depends += plugin_cpptools

plugin_qmldesigner.subdir = qmldesigner
plugin_qmldesigner.depends = plugin_coreplugin
plugin_qmldesigner.depends += plugin_texteditor
plugin_qmldesigner.depends += plugin_qmljseditor
plugin_qmldesigner.depends += plugin_projectexplorer
plugin_qmldesigner.depends += plugin_qt4projectmanager
plugin_qmldesigner.depends += plugin_qmlprojectmanager
plugin_qmldesigner.depends += plugin_cpptools

plugin_mercurial.subdir = mercurial
plugin_mercurial.depends = plugin_vcsbase
plugin_mercurial.depends += plugin_projectexplorer
plugin_mercurial.depends += plugin_coreplugin

plugin_bazaar.subdir = bazaar
plugin_bazaar.depends = plugin_vcsbase
plugin_bazaar.depends += plugin_projectexplorer
plugin_bazaar.depends += plugin_coreplugin

plugin_tasklist.subdir = tasklist
plugin_tasklist.depends = plugin_coreplugin
plugin_tasklist.depends += plugin_projectexplorer

plugin_qmljstools.subdir = qmljstools
plugin_qmljstools.depends = plugin_projectexplorer
plugin_qmljstools.depends += plugin_coreplugin
plugin_qmljstools.depends += plugin_texteditor
plugin_qmljstools.depends += plugin_qtsupport
plugin_qmljstools.depends += plugin_cpptools

plugin_macros.subdir = macros
plugin_macros.depends = plugin_texteditor
plugin_macros.depends += plugin_find
plugin_macros.depends += plugin_locator
plugin_macros.depends += plugin_coreplugin

plugin_todo.subdir = todo
plugin_todo.depends = plugin_coreplugin
plugin_todo.depends += plugin_projectexplorer
plugin_todo.depends += plugin_texteditor
plugin_todo.depends += plugin_cpptools

plugin_clearcase.subdir = clearcase
plugin_clearcase.depends = plugin_vcsbase
plugin_clearcase.depends += plugin_projectexplorer
plugin_clearcase.depends += plugin_coreplugin

plugin_juliaeditor.subdir = juliaeditor
plugin_juliaeditor.depends = plugin_texteditor
plugin_juliaeditor.depends += plugin_coreplugin
plugin_juliaeditor.depends += plugin_projectexplorer

plugin_htmlviewer.subdir = htmlviewer
plugin_htmlviewer.depends += plugin_coreplugin
plugin_htmlviewer.depends += plugin_projectexplorer
