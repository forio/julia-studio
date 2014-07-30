#ifndef OUTPUTCONSOLE_H
#define OUTPUTCONSOLE_H

#include <texteditor/basetexteditor.h>
#include <projectexplorer/ievaluator.h>

namespace JuliaPlugin {

class OutputConsole : public TextEditor::BaseTextEditorWidget
{
  Q_OBJECT

public:
  explicit OutputConsole( QWidget* parent = 0 );

  TextEditor::BaseTextEditor* createEditor();

public slots:
  void DisplayMsg( const ProjectExplorer::EvaluatorMessage* msg );
  void DisplayMsg( const QString& msg );
  void SetBusy(const QString& message = QString());
};

}

#endif // OUTPUTCONSOLE_H
