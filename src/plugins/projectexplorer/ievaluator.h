#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDataStream>
#include <QDebug>
#include <QRegExp>

#include "projectexplorer_export.h"
#include "rapidjson/document.h"

QT_BEGIN_NAMESPACE
  class QFileInfo;
  class QString;
QT_END_NAMESPACE

namespace ProjectExplorer {

using namespace rapidjson;

class PROJECTEXPLORER_EXPORT EvaluatorMessage
{
public:
  quint8 type;
  QString typnam;
  QVector<QString> params;


  void from( QDataStream& stream, int nbytes )
  {
    char* buff = new char[nbytes + 1];
    stream.readRawData( buff, nbytes );
    buff[nbytes] = 0;

    Document jsondoc;
    if ( jsondoc.ParseInsitu<0>(buff).HasParseError() )
    {
      qDebug() << "ERROR JSON parsing on output-eval" << buff;
      return;
    }
    if ( !( jsondoc.HasMember( "msgtype" ) && jsondoc.HasMember( "data" ) ) )
    {
      qDebug() << "ERROR no \"msgtype\" or \"data\" in JSON output" << buff;
      return;
    }
    if ( !jsondoc["msgtype"].IsString() )
    {
      qDebug() << "ERROR \"msgtype\" in JSON not string" << buff;
      return;
    }
    if ( !( jsondoc["data"].IsString() || jsondoc["data"].IsArray() ) )
    {
      qDebug() << "ERROR \"data\" in JSON not string or array" << buff;
      return;
    }
    typnam = QString::fromUtf8( jsondoc["msgtype"].GetString() );
    if ( jsondoc["data"].IsString() )
    {
      params.push_back( QString::fromUtf8( jsondoc["data"].GetString() ) );
    }
    else // array
    {
      const Value& data = jsondoc["data"];
      for ( unsigned int i = 0; i < data.Size(); i++ )
      {
        params.push_back( QString::fromUtf8( data[i].GetString() ) );
      }
    }
    delete [] buff;
  }

  void from( QByteArray& bytes )
  {
    QDataStream stream(&bytes, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    stream.setByteOrder(QDataStream::LittleEndian);

    from( stream, bytes.count() );
  }

  void toBytes(QByteArray& bytes) const
  {
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    stream.setByteOrder(QDataStream::LittleEndian);

    QString msg;
    msg += "{\"msgtype\": \"";
    msg += typnam;
    msg +=  "\",\"data\": [";
    for ( int i = 0; i < params.size(); ++i )
    {
      msg += "\"";
      QString parstr = params[i];
      int k;
      while( ( k = parstr.indexOf( QChar::ParagraphSeparator ) ) >= 0 )
      {
        parstr.remove( k, 1 );
        //parstr.insert( k, " " );
        parstr.insert( k, "\n" );
      }
      QRegExp rx("[\\\\\"]");
      k = -1;
      while ( ( k = parstr.lastIndexOf( rx, k ) ) != -1 )
      {
        parstr.insert( k--, '\\' );
        if ( k == -1 )
          break;
      }
      msg += parstr;
      msg += "\"";
      if ( i + 1 < params.size() )
        msg += ",";
    }
    msg += " ]}";
    if ( msg.size() == 90 )
      msg += " ";
    stream << msg.toLocal8Bit().data();
  }
};

class PROJECTEXPLORER_EXPORT IEvaluator : public QObject
{
  Q_OBJECT

public:
  explicit IEvaluator(QObject *parent = 0) : QObject(parent) {}
  virtual ~IEvaluator()  {}

public slots:
  virtual void eval(const QFileInfo* file) = 0;
  virtual void eval(const EvaluatorMessage& msg) = 0;

  virtual void reset() = 0;

  virtual bool canRun(const QString& extension) = 0;
  virtual bool isRunning() = 0;

signals:

  // For now, things will register directly with output for all messages
  // and internally they will filter. When I have time I'll either write
  // a nice little message system that could be used alongside qt or I'll
  // find a way to generate slots at compile time that people can bind
  // against (as in, generate a slot for each msg type).
  void output(const EvaluatorMessage* msg);

};

}

#endif // IEXECUTOR_H
