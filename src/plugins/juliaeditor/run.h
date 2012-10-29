#ifndef LOCALRUN_HPP
#define LOCALRUN_HPP

#include <QObject>
#include <QProcess>

QT_BEGIN_NAMESPACE
  class QFileInfo;
QT_END_NAMESPACE

namespace JuliaPlugin {

class Run : public QObject
{
  Q_OBJECT

public:
  enum RunMode
  {
    Mode_Invalid,
    Mode_KeepAlive,
    Mode_SingleRun
  };

  explicit Run( QObject* parent = NULL ) : QObject(parent)
  {;}

  virtual ~Run()
  {;}

public slots:
  virtual void SetMode( RunMode mode_ )  { mode = mode_; }
  virtual RunMode GetMode()              { return mode; }

  //virtual void Evaluate( const Project* project ) = 0;
  virtual void Evaluate( const QFileInfo* file_info ) = 0;
  virtual void Evaluate( const QString& code ) = 0;

  virtual void Kill() = 0;

  virtual void Reset() = 0;

signals:
  void Ready();
  void Finished( Run* );

  void Output( const QString& output );

protected:
  RunMode mode;
};

// *******

#if 0  // These have not been updated to work with the newest run interface
// Yes, this should probably be moved away from the base class.
// Later.
class LocalTcpRun : public Run
{
  Q_OBJECT

public:
  explicit LocalTcpRun( const Project* project_, OutputHandle output_, QObject* parent = NULL );
  virtual ~LocalTcpRun();

  virtual void Eval();
  virtual void Kill();

private slots:
  void ReceiveOutput();
  void ConnectionError( QAbstractSocket::SocketError error );

private:
  struct Message
  {
    uint8_t type;
    QVector< QString > args;
  };

  const Project* project;  // the project this job will run
  OutputHandle output;   // the output window for this project
  TcpConnection* connection;

  // Do not call these -----
  LocalTcpRun( const LocalTcpRun& );
  LocalTcpRun& operator =( const LocalTcpRun& );
  // -----
};

// *******

class HttpRun : public Run
{
  Q_OBJECT

public:
  explicit HttpRun( const Project* project_,
                    QNetworkAccessManager* network_controller,
                    OutputHandle output_,
                    QObject* parent = NULL );

  virtual ~HttpRun();

  virtual void Eval();
  virtual void Kill();

private slots:

private:
  const Project* project;
  OutputHandle output;
  HttpConnection* connection;
};

#endif

// *******

class LocalRun : public Run
{
  Q_OBJECT

public:
  explicit LocalRun( Run::RunMode mode_ = Run::Mode_Invalid, QObject* parent = NULL );
  virtual ~LocalRun();

public slots:
  virtual void SetMode( Run::RunMode mode_ );

  //virtual void Evaluate( const Project* project );
  virtual void Evaluate( const QFileInfo* file_info );
  virtual void Evaluate( const QString& code );

  virtual void Kill();

  virtual void Reset();

  void SetWorkingDir( const QString& working_directory );

private slots:
  void OnProcessOutput();
  void OnProcessError( QProcess::ProcessError error_ );

  void Exit( int exit_code );
  void PrintExitMessage( int exit_code );

private:
  void CreateProcess();
  void StartJulia( QStringList args = QStringList() );

  QString process_string;
  QProcess* process;
  QString working_dir;
};

}

#endif // RUN_HPP
