#ifndef PACKAGEMODEL_H
#define PACKAGEMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QList>


namespace JuliaPlugin {

struct PackageData
{
  PackageData() : required(false)  {}
  PackageData(const QString name_) : name(name_), required(false)  {}
  PackageData(const QString name_, bool installed_) : name(name_), required(installed_)  {}

  bool operator < (const PackageData& rhs) const  { return name < rhs.name; }
  bool operator == (const PackageData& rhs) const  { return name == rhs.name; }

  QString name;
  bool required;
};

QDebug& operator << (QDebug& d, const PackageData& p);



class PackageModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit PackageModel(QObject *parent = 0);

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::UserRole);

  virtual bool insertRows(const QList<PackageData>& data, int row, const QModelIndex &parent = QModelIndex());
  virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

  virtual void invalidateAll();

  virtual void clear();

private:

  //QStringList packages;
  QList<PackageData> packages;
  
};

}

Q_DECLARE_METATYPE(JuliaPlugin::PackageData)

#endif // PACKAGEMODEL_H
