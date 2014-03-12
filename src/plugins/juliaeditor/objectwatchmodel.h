#ifndef OBJECTWATCHMODEL_H
#define OBJECTWATCHMODEL_H

#include <QAbstractTableModel>
#include <QList>

namespace JuliaPlugin
{

struct ObjectWatchData
{
  ObjectWatchData()
  {};
  ObjectWatchData( QString pm, QString pn, QString pt, QString vl ) :
    module( pm ), name( pn ), type( pt ), value( vl )
  {};
  QString module;
  QString name;
  QString type;
  QString value;
};

class ObjectWatchModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit ObjectWatchModel( bool pValues, QObject *parent = 0 );

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  void clear();
  void addobj( QString mod, QString name, QString type, QString val );
  void display();
private:
  QVector<ObjectWatchData> names;
  bool mValues;
};

}

Q_DECLARE_METATYPE(JuliaPlugin::ObjectWatchData)

#endif