#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDataStream>
#include <QDebug>

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
    /*
    stream >> type;

    quint8 num_params;
    stream >> num_params;
    params.reserve(num_params);

    quint32 param_size;
    for (quint8 i = 0; i < num_params; ++i)
    {
      stream >> param_size;
      char* buff = new char[param_size + 1];
      buff[param_size] = 0;
      stream.readRawData(buff, param_size);
      params.push_back(QString::fromUtf8(buff, param_size));
      delete buff;
    }
    */
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
      for ( int i = 0; i < data.Size(); i++ )
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

    /*
    stream << type;
    stream << static_cast<qint8>(params.size());

    // when qt streams a char* it does so as the size and then the data
    for (int i = 0; i < params.size(); ++i)
    {
      QByteArray mangled_param = params[i].toLocal8Bit();
      stream << mangled_param.data();
      //stream << mangled_param.toStdString().c_str();
    }

    */
    /*
    stream << QByteArray( "{\n  \"type\": \"" );
    stream << typnam.toLocal8Bit().data();
    stream << QByteArray( "\",\n  \"data\": [ " );
    for ( int i = 0; i < params.size(); ++i )
    {
      stream << QByteArray( "\"" );
      stream << params[i].toLocal8Bit().data();
      stream << QByteArray( "\"" );
      if ( i + 1 < params.size() )
        stream << QByteArray( ", " );
    }
    stream << QByteArray( " ]\n}\n" );
    */
    QString msg;
    msg += "{\"msgtype\": \"";
    msg += typnam;
    msg +=  QByteArray( "\",\"data\": [" );
    for ( int i = 0; i < params.size(); ++i )
    {
      msg += "\"";
      msg += params[i];
      msg += "\"";
      if ( i + 1 < params.size() )
        msg += ",";
    }
    msg += " ]}";
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
