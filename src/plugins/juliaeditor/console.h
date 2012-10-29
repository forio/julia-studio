#ifndef CONSOLE_H
#define CONSOLE_H

#include <texteditor/basetexteditor.h>

#include <QColor>
#include <QDebug>

namespace JuliaPlugin {

class Console : public TextEditor::BaseTextEditorWidget
{
  Q_OBJECT

public:
  explicit Console( QWidget* parent = 0 );

  TextEditor::BaseTextEditor* createEditor();

signals:
  void NewCommand( const QString& command );
  void Reseting( bool preserve_history = true );
  void Ready( Console* console );

public slots:
  void BeginCommand();
  void TryCommand( const QString& command );
  void DisplayResult( const QString& result );
  void Reset( bool preserve_history = true );

  void SetPrompt( const QString& new_prompt );

  // colors -----
  QColor GetCurrCommandColor()              { return curr_command_color; }
  void SetCurrCommandColor( QColor color )  {  curr_command_color = color; }

  QColor GetOldCommandColor()               { return old_command_color; }
  void SetOldCommandColor( QColor color )   {  old_command_color = color; }

  QColor GetOutputColor()                   { return output_color; }
  void SetOutputColor( QColor color )       { output_color = color; }
  // -----

protected:
  // user input -----
  void keyPressEvent( QKeyEvent* e );
  bool Handle_KeyReturn();
  bool Handle_KeyBackspace();
  bool Handle_KeyUp();
  bool Handle_KeyDown();
  bool Handle_KeyLeft();
  // -----

  // utility -----
  QString GetCurrCommand();
  void SetCurrCommand( const QString& command );

  bool InCommandArea();
  // -----

  QString prompt;

  QColor curr_command_color;
  QColor old_command_color;
  QColor output_color;

  int begin_command_pos;
  bool busy;

  QStringList command_history;
  int history_index;

  // windows hack -----
  int remaining_bytes;
  // -----
  
};

}

#endif // CONSOLE_H
