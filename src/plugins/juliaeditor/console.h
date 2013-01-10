#ifndef CONSOLE_H
#define CONSOLE_H

#include <texteditor/basetexteditor.h>

#include <QColor>
#include <QDebug>
#include <QAbstractItemModel>
#include <projectexplorer/ievaluator.h>

namespace JuliaPlugin {

class HistoryModel : public QAbstractListModel
{
  Q_OBJECT

public:
  HistoryModel( QObject* parent = 0 ) : QAbstractListModel(parent)  {}

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  virtual bool insertRows(const QStringList& data, int row, const QModelIndex &parent = QModelIndex());
  virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

  virtual void clear();

private:
  QStringList command_history;
};


class Console : public TextEditor::BaseTextEditorWidget
{
  Q_OBJECT

public:
  explicit Console( QWidget* parent = 0 );

  TextEditor::BaseTextEditor* createEditor();

signals:
  void NewCommand( const ProjectExplorer::EvaluatorMessage& msg );
  void Reseting( bool preserve_history = true );
  void Ready( Console* console );
  void SetCommandFromHistory( const QModelIndex& index );

public slots:
  void BeginCommand();
  void DisplayMsg( const ProjectExplorer::EvaluatorMessage* msg );
  void DisplayMsg( const QString& msg );
  void Reset( bool preserve_history = true );
  void SetBusy(const QString& message = QString());

  void WindowsHack( const QString& command );

  void SetPrompt( const QString& new_prompt );

  bool IsBusy()  { return busy; }

  QString GetCurrCommand();
  void SetCurrCommand( const QString& command );
  void SetCurrCommand( const QModelIndex& index );

  HistoryModel* GetHistoryModel()  { return &command_history; }

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
  bool InCommandArea();
  // -----

  QString prompt;

  QColor curr_command_color;
  QColor old_command_color;
  QColor output_color;

  int begin_command_pos;
  bool busy;

  HistoryModel command_history;
  QModelIndex history_index;

  // windows hack -----
  int remaining_bytes;
  // -----
  
};

}

#endif // CONSOLE_H
