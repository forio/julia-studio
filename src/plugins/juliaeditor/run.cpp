#include "run.h"
#include "juliasettingspage.h"
#include "singleton.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

using namespace JuliaPlugin;

#if 0
// ----------------------------------------------------------------------------
LocalTcpRun::LocalTcpRun( const Project* project_, OutputHandle output_, QObject* parent )
  : Run(parent), project(project_), output(output_)
{
  connection = new TcpConnection;
  connect( connection, SIGNAL( Connected() ), SLOT( Eval() ) );
  connect( connection, SIGNAL( ReadyRead() ), SLOT( ReceiveOutput() ) );
  connect( connection, SIGNAL( Error(QAbstractSocket::SocketError) ), SLOT( ConnectionError(QAbstractSocket::SocketError) ) );
}

// ----------------------------------------------------------------------------
LocalTcpRun::~LocalTcpRun()
{
  delete connection;
}

// ----------------------------------------------------------------------------
void LocalTcpRun::Eval()
{
  // for now let's grab the entry file and just execute that -----
  const QFileInfo& entry_file_info = project->GetEntryFile();

  File file;
  file.setFileName( entry_file_info.absoluteFilePath() );
  if ( !file.open( QFile::ReadOnly | QFile::Text ) )
  {
    Kill();
    return;
  }

  QTextStream from_file( &file );
  // -----

  Message msg;
  msg.type = 3;  // eval message
  msg.args.push_back( "juliastudio_instance" );
  msg.args.push_back( project->GetName() );
  msg.args.push_back( from_file.readAll() );

  unsigned total_size = 2 + 4 * msg.args.size();
  for ( int i = 0; i < msg.args.size(); i++ )
      total_size += msg.args[i].size();

  char* msg_buf = new char[ total_size ];
  char* walker = msg_buf;

  // fill out the message
  *(walker++) = msg.type;
  *(walker++) = msg.args.size();

  // iterate through the arguments
  char* ascii_str;
  int size;
  for ( int i = 0; i < msg.args.size(); i++ )
  {
    ascii_str = msg.args[i].toLocal8Bit().data();
    size = msg.args[i].size();

    *( (uint32_t*)(walker) ) = (uint32_t)(size);
    memcpy( ( walker + sizeof(uint32_t) ), ascii_str, size );
    walker += sizeof(uint32_t) + size;
  }

  connection->Send( msg_buf, total_size );
}

// ----------------------------------------------------------------------------
void LocalTcpRun::Kill()
{
}

// ----------------------------------------------------------------------------
void LocalTcpRun::ReceiveOutput()
{
  // Broken on Windows! I need to find a way to include alloca()
#if !defined(Q_OS_WIN)

  QByteArray data_byte_array = connection->GetSocket()->readAll();
  char* data = data_byte_array.data();
  //int size = data_byte_array.size();

  Message msg;
  msg.type = *( (uint8_t*)( &data[0] ) );
  uint8_t num_args = *( (uint8_t*)( &data[1] ) );

  int pos = 2;
  uint32_t arg_size;
  for ( uint8_t i = 0; i < num_args; i++ )
  {
    arg_size = *( (uint32_t*)( &data[pos] ) );
    pos += sizeof( uint32_t );

    // QString does not have a constructor that takes a char* and a size
    // (a foolish omission, in my opinion, as std::string is the obvious
    // interface to target when writing a string...) so we're going to
    // alloca a substring and construct our QString out of that.
    char* arg = (char*)alloca( arg_size + 1 );
    strncpy( arg, data + pos, arg_size );
    arg[arg_size] = '\0';

    msg.args.push_back( QString( arg ) );
    pos += arg_size;
  }

  // this is very temporary
  switch( msg.type )
  {
  default:
    for( int i = 0; i < msg.args.size(); ++i )
        OutputController->Print( msg.args[i], output );
  }

  //TODO: detect when we've actually finished... look at the message type
  if ( msg.args.size() == 2 )
    emit Complete( this );

#endif
}

// ----------------------------------------------------------------------------
void LocalTcpRun::ConnectionError( QAbstractSocket::SocketError error )
{
  qDebug() << "socket error";
}

// HttpRun *******
// ----------------------------------------------------------------------------
HttpRun::HttpRun( const Project* project_, QNetworkAccessManager* network_controller, OutputHandle output_, QObject* parent )
  : Run(parent), project(project_), output(output_)
{
  connection = new HttpConnection( network_controller );
  connect( connection, SIGNAL( Connected(IConnection*) ), SLOT( Eval() ) );
  connection->Connect( "http://127.0.0.1", 3000 );
}

// ----------------------------------------------------------------------------
HttpRun::~HttpRun()
{
  delete connection;
}

// ----------------------------------------------------------------------------
void HttpRun::Eval()
{
  //TODO: none of this should be exposed!
  QUrl url;
  url.setUrl( connection->ip );
  url.setPort( connection->port );

  OutputController->Print( "Sending requests to: " + url.toString(), output );

  connection->network_manager->get( QNetworkRequest( url ) );

  OutputController->Print( "That's all she wrote", output );
}

// ----------------------------------------------------------------------------
void HttpRun::Kill()
{
}

#endif

// LocalRun *******
// ----------------------------------------------------------------------------
LocalRun::LocalRun( Run::RunMode mode_, QObject* parent )
  : Run(parent)
{
  CreateProcess();
  SetMode( mode_ );
}

// ----------------------------------------------------------------------------
LocalRun::~LocalRun()
{
  // out process is a child, it will be cleaned up automatically
}

// ----------------------------------------------------------------------------
void LocalRun::SetMode( Run::RunMode mode_ )
{
  Run::SetMode( mode_ );

  switch ( mode_ )
  {
  case Run::Mode_KeepAlive:
    StartJulia( QStringList() );
    break;

  default:
    break;
  }
}

// ----------------------------------------------------------------------------
//void LocalRun::Evaluate( const Project *project )
//{
//  Evaluate( &project->GetEntryFile() );
//}

// ----------------------------------------------------------------------------
void LocalRun::Evaluate( const QFileInfo *file_info )
{
  if ( mode == Run::Mode_SingleRun )
  {
    Output( "--- Starting Julia ---" );
    connect( process, SIGNAL( finished(int) ), SLOT( PrintExitMessage(int) ) );

    QStringList args;
    args << file_info->fileName();
    StartJulia( args );
  }
  else if ( mode == Run::Mode_KeepAlive )
    process->write( ("load(\"" + file_info->fileName() + "\")\n").toAscii() );
}

// ----------------------------------------------------------------------------
void LocalRun::Evaluate( const QString& code )
{
  if ( mode == Run::Mode_SingleRun )
  {
    Output( "--- Starting Julia ---" );
    connect( process, SIGNAL( finished(int) ), SLOT( PrintExitMessage(int) ) );

    QStringList args;
    args << code;
    StartJulia( args );
  }
  else if ( mode == Run::Mode_KeepAlive )
  {
    QString command( code );
    command.append( "\n\r" );
    process->write( command.toAscii() );
  }
}

// ----------------------------------------------------------------------------
void LocalRun::Kill()
{
  process->kill();
}

// ----------------------------------------------------------------------------
void LocalRun::Reset()
{
  disconnect( process, SIGNAL( error(QProcess::ProcessError) ), this, SLOT( OnProcessError(QProcess::ProcessError) ) );
  disconnect( process, SIGNAL( readyRead() ), this, SLOT( OnProcessOutput() ) );
  disconnect( process, SIGNAL( finished(int) ), this, SLOT( Exit(int) ) );

  Kill();
  process->deleteLater();

  CreateProcess();
  SetMode( mode );
}

// ----------------------------------------------------------------------------
void LocalRun::SetWorkingDir(const QString &working_directory)
{
  working_dir = working_directory;
}

// ----------------------------------------------------------------------------
void LocalRun::OnProcessOutput()
{
  QString output = process->readAllStandardOutput();
  emit Output( output );

  QRegExp expr( "julia>", Qt::CaseSensitive );
  if ( expr.indexIn( output ) != -1 )
    emit Ready();
}

// ----------------------------------------------------------------------------
void LocalRun::OnProcessError(QProcess::ProcessError error_)
{
    switch( error_ )
    {
    case QProcess::FailedToStart:
        Output( "Error: Failed to start Julia. Expected location: " + process_string );
        break;

    case QProcess::Crashed:
        Output( "Error: Julia crashed unexpectedly. Are you using a stable build of Julia?" );
        break;

    case QProcess::Timedout:
        Output( "Error: Timeout waiting for Julia." );
        break;

    case QProcess::WriteError:
        Output( "Error: Write to Julia failed. Try again..." );
        break;

    case QProcess::ReadError:
        Output( "Error: Read from Julia failed. Try again..." );
        break;

    case QProcess::UnknownError:
    default:
        Output( "Unknown Error. We apologize and wish you luck :/" );
        break;
    }
}

// ----------------------------------------------------------------------------
void LocalRun::Exit( int exit_code )
{
  emit Finished( this );
}

// ----------------------------------------------------------------------------
void LocalRun::PrintExitMessage( int exit_code )
{
  Output( "--- Julia process exited with code " + QString::number( exit_code ) + " ---" );
}

// ----------------------------------------------------------------------------
void LocalRun::CreateProcess()
{
  process = new QProcess(this);
  process->setProcessChannelMode(QProcess::MergedChannels);

  connect( process, SIGNAL( error(QProcess::ProcessError) ), SLOT( OnProcessError(QProcess::ProcessError) ) );
  connect( process, SIGNAL( readyRead() ), SLOT( OnProcessOutput() ) );
  connect( process, SIGNAL( finished(int) ), SLOT( Exit(int) ) );
}

// ----------------------------------------------------------------------------
void LocalRun::StartJulia( QStringList args )
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

