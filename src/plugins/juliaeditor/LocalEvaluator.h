#ifndef LOCALRUN_HPP
#define LOCALRUN_HPP

#include "../projectexplorer/ievaluator.h"

#include <QProcess>

namespace JuliaPlugin {

class LocalEvaluator : public ProjectExplorer::IEvaluator
{
  Q_OBJECT

public:
  explicit LocalEvaluator( QObject* parent = NULL );
  virtual ~LocalEvaluator();

public slots:
  virtual void eval( const QFileInfo* file_info );
  virtual void eval( const QString& code );

  virtual void reset();

  virtual bool canRun(const QString&);
  virtual bool isRunning();


  void setWorkingDir( const QString& working_directory );

signals:
  void ready();
  void finished( LocalEvaluator* );

  void output( const QString& output );

private slots:
  void onProcessOutput();
  void onProcessError( QProcess::ProcessError error_ );

  void exit( int exit_code );

private:
  void createProcess();
  void startJulia( QStringList args = QStringList() );

  QString process_string;
  QProcess* process;
  QString working_dir;
};

}

#endif // RUN_HPP
