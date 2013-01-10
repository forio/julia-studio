#include "console.h"
#include "juliaeditor.h"
#include "juliamsg.h"

#include <QDebug>

using namespace JuliaPlugin;


int HistoryModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : command_history.count();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
  if ( index.row() >= command_history.count() || index.column() != 0 )
      return QVariant();

  if ( role == Qt::DisplayRole )
      return command_history.at( index.row() );

  return QVariant();
}

bool HistoryModel::insertRows(const QStringList& data, int row, const QModelIndex &parent)
{
  beginInsertRows( parent, row, row + data.size() );
  for ( int i = 0; i < data.size(); ++i )
    command_history.insert(row + i, data[i]);
  endInsertRows();

  return true;
}

bool HistoryModel::removeRows(int row, int count, const QModelIndex &parent)
{
  QStringList::iterator begin = command_history.begin() + row;
  QStringList::iterator end = begin + count;

  beginRemoveRows( parent, row, row + count);
  command_history.erase(begin, end);
  endRemoveRows();

  return true;
}

void HistoryModel::clear()
{
  beginRemoveRows( QModelIndex(), 0, rowCount() );
  command_history.clear();
  endRemoveRows();
}



// ----------------------------------------------------------------------------
Console::Console( QWidget* parent ) :
  TextEditor::BaseTextEditorWidget(parent), begin_command_pos(0), busy(true), remaining_bytes(0)
{
  SetPrompt("badass julia> ");
}

// ----------------------------------------------------------------------------
TextEditor::BaseTextEditor *Console::createEditor()
{
  return new JuliaEditor( this );
}

// ----------------------------------------------------------------------------
void Console::BeginCommand()
{
  if (!busy)
    return;

  setUndoRedoEnabled(false);  // skip the prompt text

  moveCursor( QTextCursor::End );
  insertPlainText( prompt );
  begin_command_pos = textCursor().position();
  busy = false;

  setUndoRedoEnabled(true);

  emit( Ready( this ) );
}

// ----------------------------------------------------------------------------
void Console::DisplayMsg(const ProjectExplorer::EvaluatorMessage* msg)
{
  if ( msg->type != JM_OUTPUT_EVAL &&
       msg->type != JM_OUTPUT_ERROR )
  {
    return;
  }

  QString output( msg->params[0] + "\n\n" );

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
void Console::DisplayMsg(const QString &msg)
{
  bool expecting = busy;
  SetBusy();

  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  cursor.insertText( msg );

  if (!expecting)
    BeginCommand();
}

// ----------------------------------------------------------------------------
void Console::Reset( bool preserve_history )
{
  if ( !preserve_history )
  {
    history_index = QModelIndex();
    command_history.clear();
  }

  clear();
  busy = true;

#if defined(Q_OS_WIN)
  remaining_bytes = 0;
#endif

  emit( Reseting( preserve_history ) );
}

// ----------------------------------------------------------------------------
void Console::SetBusy(const QString &message)
{
  if (busy)
    return;

  busy = true;

  moveCursor( QTextCursor::End );

  if (message.size())
    insertPlainText(message);

  insertPlainText( "\n" );
}

// ----------------------------------------------------------------------------
void Console::WindowsHack(const QString &command)
{
    remaining_bytes = command.size();  // for the newline
    if ( command.contains( ' ' ) )
        remaining_bytes += 2;
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
  const QString command = GetCurrCommand();
  command_history.insertRows( QStringList(command), command_history.rowCount() );
  history_index = QModelIndex();

  SetBusy();

  // windows hack -----
#if defined(Q_OS_WIN)
  WindowsHack(command);
#endif
  // -----

  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_EVAL;
  msg.params.push_back(command);

  emit NewCommand(msg);

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
  if ( command_history.rowCount() )
  {
    if ( !history_index.isValid() )
      history_index = command_history.index( command_history.rowCount() - 1 );
    else if ( history_index.row() > 0 )
      history_index = history_index.sibling( history_index.row() - 1, 0 );

    SetCurrCommand(history_index);
  }

  return true;
}

// ----------------------------------------------------------------------------
bool Console::Handle_KeyDown()
{
  int history_size = command_history.rowCount();
  if ( history_size )
  {
    if ( history_index.row() < history_size )
      history_index = history_index.sibling( history_index.row() + 1, 0 );

    SetCurrCommand(history_index);
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

#if defined(Q_OS_WIN)
  command.replace( "\u+2029", "\n" );
#endif
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

void Console::SetCurrCommand(const QModelIndex &index)
{
  history_index = index;
  SetCurrCommand( command_history.data(index).toString() );
  SetCommandFromHistory( index );
  setFocus();
}

// ----------------------------------------------------------------------------
bool Console::InCommandArea()
{
  QTextCursor begin_command = textCursor();
  begin_command.setPosition( begin_command_pos );

  // selectionStart will be position if no text is selected
  return begin_command.position() <= textCursor().selectionStart();
}
