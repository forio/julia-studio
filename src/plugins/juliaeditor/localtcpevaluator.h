#ifndef LOCALTCPEVALUATOR_H
#define LOCALTCPEVALUATOR_H

#include "../projectexplorer/ievaluator.h"

#include <QProcess>
#include <QTcpSocket>
#include <QTcpSocket>
#include <QQueue>

namespace JuliaPlugin {

class LocalTcpEvaluator : public ProjectExplorer::IEvaluator
{
  Q_OBJECT

public:
  explicit LocalTcpEvaluator(QObject *parent = 0);
  virtual ~LocalTcpEvaluator();

public slots:
  virtual void startup();

  virtual void eval(const QFileInfo* file);
  virtual void eval( const ProjectExplorer::EvaluatorMessage& msg );

  virtual void reset();

  virtual bool canRun(const QString&);
  virtual bool isRunning();

  virtual void kill();

  void setWorkingDir( const QString& working_directory );
  
signals:
  void ready();
  void finished( LocalTcpEvaluator* );

  void output( const ProjectExplorer::EvaluatorMessage* msg );
  void output( const QString& msg );
  
private slots:
  void onProcessOutput();
  void onProcessError( QProcess::ProcessError error_ );
  void onProcessStarted();

  void onSocketOutput();
  void onSocketError( QAbstractSocket::SocketError error );
  void onSocketConnected();

  void continueOrReady();

  void exit( int exit_code );

  void startJuliaProcess( QStringList args = QStringList() );
  void prepareJulia();
  void connectToJulia( unsigned port );

  void onChangeDirResult(const ProjectExplorer::EvaluatorMessage* msg);
  void onPlot( const ProjectExplorer::EvaluatorMessage* msg );

private:
  QQueue<ProjectExplorer::EvaluatorMessage> work_queue;
  bool busy;

  QString process_string;
  QProcess* process;

  QTcpSocket* socket;
  qint32 curr_msg_size;

  QString curr_working_dir;
  QString plot_view_filename;
  
};

}

#endif // LOCALTCPEVALUATOR_H
