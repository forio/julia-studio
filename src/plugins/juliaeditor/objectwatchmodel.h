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
  ObjectWatchData( QString pm, QString pn, QString pt ) :
    module( pm ), name( pn ), type( pt )
  {};
  QString module;
  QString name;
  QString type;
};

class ObjectWatchModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit ObjectWatchModel( QObject *parent = 0 );

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  void clear();
  void addobj( QString mod, QString name, QString type );
  void display();
private:
  QVector<ObjectWatchData> names;
};

}

Q_DECLARE_METATYPE(JuliaPlugin::ObjectWatchData)

#endif