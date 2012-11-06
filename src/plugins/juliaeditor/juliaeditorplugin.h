#ifndef JULIAEDITORPLUGIN_H
#define JULIAEDITORPLUGIN_H

#include "juliaeditor_global.h"
#include "juliaeditor.h"
#include "juliasettingspage.h"

#include <extensionsystem/iplugin.h>
#include <coreplugin/editormanager/ieditorfactory.h>

#include <QSharedPointer>

namespace TextEditor {
  class TextEditorActionHandler;
  class ITextEditor;
}

namespace JuliaPlugin {

class LocalEvaluator;
class JuliaConsolePane;

namespace Internal {
    
class JuliaEditorPlugin : public ExtensionSystem::IPlugin
{
  Q_OBJECT

public:
  JuliaEditorPlugin();
  ~JuliaEditorPlugin();

  bool initialize(const QStringList &arguments, QString *errorString);
  void extensionsInitialized();
  ShutdownFlag aboutToShutdown();

private slots:
  void initEditor( TextEditor::BaseTextEditorWidget* editor );
  void triggerAction();

private:
  TextEditor::TextEditorActionHandler* action_handler;

  LocalEvaluator* evaluator;
  JuliaConsolePane* console_pane;
};


// *******

class JuliaEditorFactory : public Core::IEditorFactory
{
  Q_OBJECT

public:
  JuliaEditorFactory( JuliaEditorPlugin* plugin_ );

public slots:
  QStringList mimeTypes() const  { return mime_types; }

  Core::IEditor* createEditor( QWidget* parent );

  Core::Id id() const;
  QString displayName() const;

signals:
  void newEditor( JuliaEditorWidget* editor );

private:
  JuliaEditorPlugin* plugin;
  QStringList mime_types;

};
    
} // namespace Internal
} // namespace JuliaEditor

#endif // JULIAEDITOR_H

