#include "outputconsole.h"
#include "juliaeditor.h"
#include "juliamsg.h"

using namespace JuliaPlugin;

// ----------------------------------------------------------------------------
OutputConsole::OutputConsole( QWidget* parent ) :
  TextEditor::BaseTextEditorWidget(parent)
{
  setReadOnly( true );
}

// ----------------------------------------------------------------------------
void OutputConsole::DisplayMsg(const ProjectExplorer::EvaluatorMessage* msg)
{
  if ( msg->typnam != OUTPUT_EVAL_name &&
       msg->typnam != OUTPUT_EVAL_SILENT_name &&
       msg->typnam != OUTPUT_ERROR_name )
  {
    return;
  }

  QString output( msg->params[0] );
  if ( msg->typnam != OUTPUT_EVAL_SILENT_name )
    output += "\n";
  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  cursor.insertText( output );
  moveCursor( QTextCursor::End );
}

// ----------------------------------------------------------------------------
void OutputConsole::DisplayMsg(const QString &msg)
{
  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  cursor.insertText( msg );
  moveCursor( QTextCursor::End );
}

TextEditor::BaseTextEditor *OutputConsole::createEditor()
{
  return new JuliaEditor( this );
}

// ----------------------------------------------------------------------------
void OutputConsole::SetBusy( const QString &message )
{
  moveCursor( QTextCursor::End );

  insertPlainText( "\n" );

  if (message.size())
    insertPlainText(message);

  insertPlainText( "\n" );
}
