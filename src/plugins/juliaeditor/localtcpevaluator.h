#ifndef LOCALTCPEVALUATOR_H
#define LOCALTCPEVALUATOR_H

#include "../projectexplorer/ievaluator.h"

#include <QProcess>
#include <QTcpSocket>
#include <QTcpSocket>

namespace JuliaPlugin {

class LocalTcpEvaluator : public ProjectExplorer::IEvaluator
{
  Q_OBJECT

public:
  explicit LocalTcpEvaluator(QObject *parent = 0);
  virtual ~LocalTcpEvaluator();

public slots:
  virtual void eval( const QFileInfo* file_info );
  virtual void eval( const QString& code );

  virtual void reset();

  virtual bool canRun(const QString&);
  virtual bool isRunning();

  virtual void kill();

  void setWorkingDir( const QString& working_directory );
  
signals:
  void ready();
  void finished( LocalTcpEvaluator* );

  void output( const QString& output );
  
private slots:
  void onProcessOutput();
  void onProcessError( QProcess::ProcessError error_ );
  void onProcessStarted();

  void onSocketOutput();
  void onSocketError( QAbstractSocket::SocketError error );

  void exit( int exit_code );

  void startJuliaProcess( QStringList args = QStringList() );
  void connectToJulia( unsigned port );

private:
  QString process_string;
  QProcess* process;

  QTcpSocket* socket;

  QString curr_working_dir;
  
};

}

#endif // LOCALTCPEVALUATOR_H
