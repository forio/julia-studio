#include "packagemodel.h"

#include <QDebug>

using namespace JuliaPlugin;

QDebug& JuliaPlugin::operator<<(QDebug& d, const PackageData& p)
{
  return d << p.name;
}


PackageModel::PackageModel(QObject *parent) :
  QAbstractListModel(parent)
{}

int PackageModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : packages.count();
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
  if (index.row() >= packages.count() || index.column() != 0)
      return QVariant();

  if (role == Qt::DisplayRole)
    return packages.at(index.row()).name;
  else if (role == Qt::UserRole)
    return QVariant::fromValue(packages.at(index.row()));

  return QVariant();
}

bool PackageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.row() >= packages.count() || index.column() != 0)
    return false;

  QList<PackageData>::iterator data = packages.begin();
  data += index.row();

  qDebug() << value.value<PackageData>().name;
  *data = value.value<PackageData>();

  emit dataChanged(index, index);

  return true;
}

bool PackageModel::insertRows(const QList<PackageData> &data, int row, const QModelIndex &parent)
{
  beginInsertRows( parent, row, data.size() );
  for ( int i = 0; i < data.size(); ++i )
  {
    QList<PackageData>::iterator existing = qFind(packages.begin(), packages.end(), data.at(i));
    if (existing != packages.end())
      *existing = data.at(i);
    else
      packages.insert(row + i, data.at(i));
  }
  endInsertRows();

  return true;
}

bool PackageModel::removeRows(int row, int count, const QModelIndex &parent)
{
  QList<PackageData>::iterator begin = packages.begin() + row;
  QList<PackageData>::iterator end = begin + count;

  beginRemoveRows( parent, row, row + count);
  packages.erase(begin, end);
  endRemoveRows();

  return true;
}

void PackageModel::invalidateAll()
{
  for(QList<PackageData>::iterator pkg = packages.begin(); pkg != packages.end(); ++pkg)
    pkg->installed = false;

  emit dataChanged(index(0), index(rowCount()));

  for(QList<PackageData>::iterator pkg = packages.begin(); pkg != packages.end(); ++pkg)
    qDebug() << pkg->installed;
}

void PackageModel::clear()
{
  beginRemoveRows( QModelIndex(), 0, rowCount() );
  packages.clear();
  endRemoveRows();
}
