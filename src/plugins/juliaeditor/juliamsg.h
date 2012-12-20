#ifndef JULIAMSG_H
#define JULIAMSG_H

#include <QByteArray>
#include <QVector>
#include <QString>
#include <QDataStream>


const quint8 JM_NULL = 0;
const quint8 JM_EVAL = 1;

const quint8 JM_OUTPUT_ERROR = 129;
const quint8 JM_OUTPUT_EVAL = 130;


struct JuliaMsg
{
  quint8 type;
  QVector<QString> params;

  void fromBytes( QByteArray& bytes )
  {
    QDataStream stream(&bytes, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_4_0);
    stream.setByteOrder(QDataStream::LittleEndian);

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

  void toBytes(QByteArray& bytes)
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



#endif // JULIAMSG_H
