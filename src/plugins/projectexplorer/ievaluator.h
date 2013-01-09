#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QDataStream>
#include <QDebug>

#include "projectexplorer_export.h"

QT_BEGIN_NAMESPACE
  class QFileInfo;
  class QString;
QT_END_NAMESPACE

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT EvaluatorMessage
{
public:
  quint8 type;
  QVector<QString> params;


  void from( QDataStream& stream )
  {
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
  }

  void from( QByteArray& bytes )
  {
    QDataStream stream(&bytes, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    stream.setByteOrder(QDataStream::LittleEndian);

    from(stream);
  }

  void toBytes(QByteArray& bytes) const
  {
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << type;
    stream << static_cast<qint8>(params.size());

    // when qt streams a char* it does so as the size and then the data
    for (int i = 0; i < params.size(); ++i)
      stream << params[i].toStdString().c_str();
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
