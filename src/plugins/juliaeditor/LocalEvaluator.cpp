#include "LocalEvaluator.h"
#include "juliasettingspage.h"
#include "singleton.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>

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
}


// ----------------------------------------------------------------------------
void LocalEvaluator::eval( const QFileInfo *file_info )
{
  if ( process->state() != QProcess::Running )
    return;

  process->write( ("push(LOAD_PATH, \"" + file_info->absolutePath() + "\"); load(\"" + file_info->fileName() + "\")\n").toAscii() );
}

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
  disconnect( process, SIGNAL( error(QProcess::ProcessError) ), this, SLOT( onProcessError(QProcess::ProcessError) ) );
  disconnect( process, SIGNAL( readyRead() ), this, SLOT( onProcessOutput() ) );
  disconnect( process, SIGNAL( finished(int) ), this, SLOT( exit(int) ) );

  process->kill();
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
void LocalEvaluator::setWorkingDir(const QString &working_directory)
{
  working_dir = working_directory;
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
        output( "Error: Failed to start Julia. Expected location: " + process_string );
        break;

    case QProcess::Crashed:
        output( "Error: Julia crashed unexpectedly. Are you using a stable build of Julia?" );
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

  process_string = julia_dir.absoluteFilePath("bin/julia-release-basic");

#if defined(Q_OS_WIN)
  //args.insert( args.begin(), QString("-q") );
    // set up context for julia only for windows
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    environment.insert("PATH", environment.value("Path") + ";" + QDir::toNativeSeparators( Settings->GetJuliaSourcePath() ) );
    process->setProcessEnvironment( environment );
#endif

  if ( working_dir.length() )
    process->setWorkingDirectory( QDir::toNativeSeparators( working_dir ) );

  qDebug() << process_string;
  process->start( process_string, args, QProcess::ReadWrite );
}

}

