#include "console.h"
#include "juliaeditor.h"

#include <QDebug>

using namespace JuliaPlugin;

// ----------------------------------------------------------------------------
Console::Console( QWidget* parent ) :
  TextEditor::BaseTextEditorWidget(parent), begin_command_pos(0), busy(true), history_index(0), remaining_bytes(0)
{}

// ----------------------------------------------------------------------------
TextEditor::BaseTextEditor *Console::createEditor()
{
  return new JuliaEditor( this );
}

// ----------------------------------------------------------------------------
void Console::BeginCommand()
{
  setUndoRedoEnabled(false);  // skip the prompt text

  moveCursor( QTextCursor::End );
  insertPlainText( prompt );
  begin_command_pos = textCursor().position();
  busy = false;

  setUndoRedoEnabled(true);

  emit( Ready( this ) );
}

// ----------------------------------------------------------------------------
void Console::TryCommand(const QString &command)
{
  if ( !busy )
  {
    SetCurrCommand( command );
    Handle_KeyReturn();
  }
}

// ----------------------------------------------------------------------------
void Console::DisplayResult(const QString &result)
{
    QString output( result );

#if defined(Q_OS_WIN)
    output.remove( QRegExp("\\x1B\\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]") );

    if ( remaining_bytes > 0 )
    {
        if ( output.size() < remaining_bytes )
        {
            remaining_bytes -= output.size();
            return;
        }
        else
            output.remove( 0, remaining_bytes );
    }
#endif
  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  cursor.insertText( output );
}

// ----------------------------------------------------------------------------
void Console::Reset( bool preserve_history )
{
  if ( !preserve_history )
  {
    history_index = 0;
    command_history.clear();
  }

  clear();

  emit( Reseting( preserve_history ) );
}

// ----------------------------------------------------------------------------
void Console::SetPrompt( const QString& new_prompt )
{
  prompt = new_prompt;
}

// ----------------------------------------------------------------------------
void Console::keyPressEvent( QKeyEvent* e )
{
  // handle all key sequences -----
  if ( e->modifiers() & Qt::ControlModifier )
  {
    if ( e->matches( QKeySequence::Cut) )
    {
      if ( InCommandArea() )
        cut();
      else
        copy();
    }
    else if ( e->matches( QKeySequence::Copy ) )
      copy();
    else if ( e->matches( QKeySequence::Paste ) )
    {
      if ( InCommandArea() )
        paste();
    }
    else if ( e->matches(QKeySequence::MoveToStartOfLine) ||
              e->matches(QKeySequence::MoveToStartOfBlock) ||
              e->matches(QKeySequence::MoveToStartOfDocument) )
    {
      QTextCursor cursor = textCursor();
      cursor.setPosition( begin_command_pos );
      setTextCursor( cursor );
    }
    return;
  }
  else if ( e->modifiers() & Qt::MetaModifier )
  {
    // on windows / linux this should never get triggered, but on mac we can
    // use this to detect when someone pressed CTRL + A (which should work)
    if ( e->key() == Qt::Key_A )
    {
      QTextCursor cursor = textCursor();
      cursor.setPosition( begin_command_pos );
      setTextCursor( cursor );
      return;
    }
  }
  // -----

  // handle all console keys -----
  switch ( e->key() )
  {
  case Qt::Key_Return:
  case Qt::Key_Enter:
    if ( Handle_KeyReturn() )
      return;
    break;

  case Qt::Key_Backspace:
    if ( Handle_KeyBackspace() )
      return;
    break;

  case Qt::Key_Up:
    if ( Handle_KeyUp() )
      return;
    break;

  case Qt::Key_Down:
    if ( Handle_KeyDown() )
      return;
    break;

  case Qt::Key_Left:
    if ( Handle_KeyLeft() )
      return;
    break;

  default:
    if ( !InCommandArea() )
      moveCursor( QTextCursor::End );
    break;
  }
  // -----

  // if we got here, just treat it like a normal key press
  TextEditor::BaseTextEditorWidget::keyPressEvent( e );
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyReturn()
{
  command_history.push_back( GetCurrCommand() );
  history_index = command_history.size();

  moveCursor( QTextCursor::End );
  insertPlainText( "\n" );

  // windows hack -----
#if defined(Q_OS_WIN)
  QString str = command_history.back();
  remaining_bytes = str.size();  // for the newline
  if ( str.contains( ' ' ) )
      remaining_bytes += 2;
#endif
  // -----

  busy = true;
  emit( NewCommand( command_history.back() ) );

  return true;
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyBackspace()
{
  if ( !InCommandArea() || textCursor().position() == begin_command_pos )
    return true;

  return false;
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyUp()
{
  if ( !command_history.empty() )
  {
    if ( history_index > 0 )
      --history_index;

    SetCurrCommand( command_history.at( history_index ) );
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyDown()
{
  int history_size = command_history.size();
  if ( history_size )
  {
    if ( history_index < history_size )
      ++history_index;

    if ( history_index == history_size )
      SetCurrCommand( "" );
    else
      SetCurrCommand( command_history.at( history_index ) );
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyLeft()
{
  if ( !InCommandArea() || textCursor().position() == begin_command_pos )
    return true;

  return false;
}

// ----------------------------------------------------------------------------
QString Console::GetCurrCommand()
{
  QTextCursor command_cursor = textCursor();
  command_cursor.setPosition( begin_command_pos );
  command_cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor );
  QString command = command_cursor.selectedText();
  return command;
}

// ----------------------------------------------------------------------------
void Console::SetCurrCommand( const QString& command )
{
  QTextCursor cursor = textCursor();
  cursor.setPosition( begin_command_pos );
  cursor.movePosition( QTextCursor::End, QTextCursor::KeepAnchor );
  cursor.insertText( command );
  setTextCursor( cursor );
}

// ----------------------------------------------------------------------------
bool Console::InCommandArea()
{
  QTextCursor begin_command = textCursor();
  begin_command.setPosition( begin_command_pos );

  // selectionStart will be position if no text is selected
  return begin_command.position() <= textCursor().selectionStart();
}
