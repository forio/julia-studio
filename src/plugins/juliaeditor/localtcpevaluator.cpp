#include "localtcpevaluator.h"
#include "juliasettingspage.h"
#include "singleton.h"
#include "juliamsg.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QStringBuilder>

using namespace JuliaPlugin;

LocalTcpEvaluator::LocalTcpEvaluator(QObject *parent) :
  ProjectExplorer::IEvaluator(parent), socket(NULL), busy(false)
{
  startJuliaProcess();
}

LocalTcpEvaluator::~LocalTcpEvaluator()
{
  kill();
}

void LocalTcpEvaluator::eval( const QFileInfo* file_info )
{
  QString command;
#if defined(Q_OS_WIN)
  command = QString("include(\"" + file_info->absoluteFilePath() + "\")\r\n");
  output("\n");
  executing( command );  // windows hack!
#else
  command = QString("push(LOAD_PATH, \"" + file_info->absolutePath() + "\");include(\"" + file_info->absoluteFilePath() + "\")\n").toAscii();
  //output(file_info->baseName() + "\n");
#endif

  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_EVAL;
  msg.params.push_back(command);

  eval(msg);
}

void LocalTcpEvaluator::eval( const ProjectExplorer::EvaluatorMessage& msg )
{
  if ( busy || !socket || socket->state() != QTcpSocket::ConnectedState )
  {
    work_queue.push_back(msg);
    return;
  }

  QByteArray bytes;
  msg.toBytes(bytes);

  socket->write(bytes);
}

void LocalTcpEvaluator::reset()
{
  disconnect( process, SIGNAL( error(QProcess::ProcessError) ), this, SLOT( onProcessError(QProcess::ProcessError) ) );
  disconnect( process, SIGNAL( readyRead() ), this, SLOT( onProcessOutput() ) );
  disconnect( process, SIGNAL( finished(int) ), this, SLOT( exit(int) ) );
  disconnect( process, SIGNAL( started()), this, SLOT(onProcessStarted()) );

  disconnect(socket, SIGNAL(readyRead()), this, SLOT(onSocketOutput()));
  disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
  disconnect(socket, SIGNAL(connected()), this, SIGNAL(ready()));

  kill();
  process->deleteLater();
  socket->deleteLater();

  startJuliaProcess();
}

bool LocalTcpEvaluator::canRun(const QString &)
{
  return true;
}

bool LocalTcpEvaluator::isRunning()
{
  return (process->state() == QProcess::Running) && (socket->state() == QTcpSocket::ConnectedState);
}

void LocalTcpEvaluator::kill()
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

void LocalTcpEvaluator::setWorkingDir(const QString &working_directory)
{
}

void LocalTcpEvaluator::onProcessOutput()
{
  QByteArray output_bytes = process->readAll();
  qDebug() << output_bytes;

  QRegExp connection_msg("PORT:");
  int index = connection_msg.indexIn(output_bytes);
  if (index != -1)
  {
    QString port_str = output_bytes.data() + index + 5;

    bool ok;
    unsigned port = port_str.toUInt(&ok);

    qDebug() << port;

    connectToJulia(port);
  }
}

void LocalTcpEvaluator::onProcessError(QProcess::ProcessError error_)
{
  //output("PROCESS ERROR!");
  qDebug() << "PROCESS ERROR";
}

void LocalTcpEvaluator::onProcessStarted()
{}

void LocalTcpEvaluator::onSocketOutput()
{
  ProjectExplorer::EvaluatorMessage msg;
  QByteArray bytes = socket->readAll();
  msg.fromBytes(bytes);

  emit output(&msg);
  busy = false;

  continueOrReady();

#if 0
  if (msg.params.size())
  {
    output(msg.params[0] + "\n");
    emit ready();
  }
  else
    output("DID NOT GET ANY MSG PARAMS");
#endif
}

void LocalTcpEvaluator::onSocketError(QAbstractSocket::SocketError error)
{
  //output("SOCKET ERROR");
  qDebug() << "SOCKET ERROR";
}

void LocalTcpEvaluator::continueOrReady()
{
  if ( work_queue.size() )
  {
    ProjectExplorer::EvaluatorMessage msg = work_queue.front();
    work_queue.pop_front();

    eval(msg);
  }
  else
    emit ready();
}

void LocalTcpEvaluator::exit(int exit_code)
{
  emit finished( this );
}

void LocalTcpEvaluator::startJuliaProcess(QStringList args)
{
  process = new QProcess(this);
  process->setProcessChannelMode(QProcess::MergedChannels);

  connect( process, SIGNAL( error(QProcess::ProcessError) ), SLOT( onProcessError(QProcess::ProcessError) ) );
  connect( process, SIGNAL( readyRead() ), SLOT( onProcessOutput() ) );
  connect( process, SIGNAL( finished(int) ), SLOT( exit(int) ) );
  connect( process, SIGNAL( started()), SLOT(onProcessStarted()) );

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

  args.append("/Users/westley/Code/sandbox/main.jl");
  process->start( process_string, args, QProcess::ReadWrite );
}

void LocalTcpEvaluator::connectToJulia(unsigned port)
{
  socket = new QTcpSocket(this);
  connect(socket, SIGNAL(readyRead()), SLOT(onSocketOutput()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(connected()), SLOT(continueOrReady()));

  socket->connectToHost("127.0.0.1", port);
}
