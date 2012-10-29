#ifndef JULIAEDITORCONSTANTS_H
#define JULIAEDITORCONSTANTS_H

namespace JuliaPlugin {
  namespace Constants {

    char const * const ACTION_ID = "JuliaEditor.Action";
    char const * const MENU_ID = "JuliaEditor.Menu";

    char const * const JULIAEDITOR = "JuliaPlugin.JuliaEditor";
    char const * const JULIAEDITOR_ID = "JuliaPlugin.JuliaEditor";
    char const * const JULIAEDITOR_DISPLAY_NAME = QT_TRANSLATE_NOOP("OpenWith::Editors", "Julia Editor");

    char const * const JULIA_MIMETYPE = "text/julia";
    char const * const JULIA_PROJECT_MIMETYPE = "text/julia-projet";

    char const * const JULIA_SETTINGS_ID = "Julia";
    char const * const JULIA_SETTINGS_NAME = QT_TRANSLATE_NOOP("JuliaPlugin", "Julia");

  } // namespace JuliaEditor
} // namespace Constants

#endif // JULIAEDITORCONSTANTS_H

