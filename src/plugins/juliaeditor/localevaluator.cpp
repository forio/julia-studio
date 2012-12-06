#include "localevaluator.h"
#include "juliasettingspage.h"
#include "singleton.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QStringBuilder>

#if defined(Q_OS_WIN)
#include <Windows.h>
#include <sstream>
#endif
namespace JuliaPlugin {

// ----------------------------------------------------------------------------
LocalEvaluator::LocalEvaluator( QObject* parent )
  : ProjectExplorer::IEvaluator(parent)
{
  createProcess();
  startJulia();
}

// ----------------------------------------------------------------------------
LocalEvaluator::~LocalEvaluator()
{
  // out process is a child, it will be cleaned up automatically

  // well... not on windows.
  kill();
}

// ----------------------------------------------------------------------------
void LocalEvaluator::eval( const QFileInfo *file_info )
{
  if ( process->state() != QProcess::Running )
    return;

  QString command;
#if defined(Q_OS_WIN)
  command = QString("include(\"" + file_info->absoluteFilePath() + "\")\r\n");
  output("\n");
  executing( command + "\n" );  // windows hack!
#else
  command = QString("push(LOAD_PATH, \"" + file_info->absolutePath() + "\");include(\"" + file_info->absoluteFilePath() + "\")\n").toAscii();
  output(file_info->baseName() + "\n");
#endif

  process->write( command.toAscii() );}

// ----------------------------------------------------------------------------
void LocalEvaluator::eval( const QString& code )
{
  if ( process->state() != QProcess::Running )
    return;

  QString command( code );
  command.append( "\n\r" );
  process->write( command.toAscii() );
}

// ----------------------------------------------------------------------------
void LocalEvaluator::reset()
{
  //setWorkingDir( "" );

  disconnect( process, SIGNAL( error(QProcess::ProcessError) ), this, SLOT( onProcessError(QProcess::ProcessError) ) );
  disconnect( process, SIGNAL( readyRead() ), this, SLOT( onProcessOutput() ) );
  disconnect( process, SIGNAL( finished(int) ), this, SLOT( exit(int) ) );

  kill();
  process->deleteLater();

  createProcess();
  startJulia();
}

// ----------------------------------------------------------------------------
bool JuliaPlugin::LocalEvaluator::canRun(const QString&)
{
  return true;
}

// ----------------------------------------------------------------------------
bool LocalEvaluator::isRunning()
{
    return process->state() == QProcess::Running;
}

// ----------------------------------------------------------------------------
void LocalEvaluator::kill()
{
#if defined(Q_OS_WIN)
  std::ostringstream stream;
  stream << process->pid()->dwProcessId;
  std::string pid = stream.str();
  QProcess::execute( "taskkill /pid " + QString::fromUtf8(pid.data(), pid.size()) + " /f /t" );
#else
  process->kill();
#endif
}

// ----------------------------------------------------------------------------
void LocalEvaluator::setWorkingDir(const QString& working_directory)
{
  if ( !working_directory.size() )  // on reset
  {
      curr_working_dir = working_directory;
      return;
  }

  if ( working_directory == curr_working_dir ||
       process->state() != QProcess::Running )
  {
    return;
  }

  QString command;

#if defined(Q_OS_WIN)
  command = QString("cd(\"" + working_directory + "\")\r\n");
  output("\n");
  executing( command + "\n" );  // windows hack!
#else
  command = QString( "cd(\"" + working_directory + "\")\n" );
  output( "working dir: " + working_directory + "\n" );
#endif

  qDebug() << command;

  process->write( command.toAscii() );
  curr_working_dir = working_directory;
}

// ----------------------------------------------------------------------------
void LocalEvaluator::onProcessOutput()
{
  QString output_str = process->readAllStandardOutput();
  emit output( output_str );

  QRegExp expr( "julia>", Qt::CaseSensitive );
  if ( expr.indexIn( output_str ) != -1 )
    emit ready();
}

// ----------------------------------------------------------------------------
void LocalEvaluator::onProcessError(QProcess::ProcessError error_)
{
    switch( error_ )
    {
    case QProcess::FailedToStart:
        output( "Error: Failed to start Julia.\nExpected location: " + process_string );
        break;

    case QProcess::Crashed:
        output( "Error: Julia crashed unexpectedly.\nIt's still a young language, this can happen from time to time.\nPlease reset the console and try again..." );
        break;

    case QProcess::Timedout:
        output( "Error: Timeout waiting for Julia." );
        break;

    case QProcess::WriteError:
        output( "Error: Write to Julia failed. Try again..." );
        break;

    case QProcess::ReadError:
        output( "Error: Read from Julia failed. Try again..." );
        break;

    case QProcess::UnknownError:
    default:
        output( "Unknown Error. We apologize and wish you luck :/" );
        break;
    }
}

// ----------------------------------------------------------------------------
void LocalEvaluator::exit( int exit_code )
{
  emit finished( this );
}

// ----------------------------------------------------------------------------
void LocalEvaluator::createProcess()
{
  process = new QProcess(this);
  process->setProcessChannelMode(QProcess::MergedChannels);

  connect( process, SIGNAL( error(QProcess::ProcessError) ), SLOT( onProcessError(QProcess::ProcessError) ) );
  connect( process, SIGNAL( readyRead() ), SLOT( onProcessOutput() ) );
  connect( process, SIGNAL( finished(int) ), SLOT( exit(int) ) );
}

// ----------------------------------------------------------------------------
void LocalEvaluator::startJulia( QStringList args )
{
  QDir julia_dir(Singleton<JuliaSettings>::GetInstance()->GetPathToBinaries());

  process_string = QDir::toNativeSeparators(julia_dir.absoluteFilePath("bin/julia-release-basic"));

#if defined(Q_OS_WIN)
  process_string = QDir::toNativeSeparators(julia_dir.absoluteFilePath("julia.bat"));
  // set up context for julia (only for windows)
  QDir lib_dir(julia_dir);
  lib_dir.cd("lib");

  QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
  environment.insert("PATH", environment.value("Path") + ";" + QDir::toNativeSeparators( lib_dir.absolutePath() ) );
  process->setProcessEnvironment( environment );
#endif

  process->start( process_string, args, QProcess::ReadWrite );


  if ( curr_working_dir.count() )
  {
    QString command;

#if defined(Q_OS_WIN)
    command = QString("cd(\"" + curr_working_dir + "\")\r\n");
#else
    command = QString( "cd(\"" + curr_working_dir + "\")\n" );
#endif

    process->write( command.toAscii() );
  }

}

}

