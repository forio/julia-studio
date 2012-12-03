#include "packagemodel.h"

#include <QDebug>

using namespace JuliaPlugin;

PackageModel::PackageModel(QObject *parent) :
  QAbstractListModel(parent)
{
  evaluator = new LocalEvaluator;
  connect( evaluator, SIGNAL(ready()), SLOT(GetAvailable()) );
}

int PackageModel::rowCount(const QModelIndex &parent) const
{
  return parent.isValid() ? 0 : packages.count();
}

QVariant PackageModel::data(const QModelIndex &index, int role) const
{
  if ( index.row() >= packages.count() || index.column() != 0 )
      return QVariant();

  if ( role == Qt::DisplayRole )
      return packages.at( index.row() );

  return QVariant();
}

bool PackageModel::insertRows(const QStringList &data, int row, const QModelIndex &parent)
{
  beginInsertRows( parent, row, data.size() );
  for ( int i = 0; i < data.size(); ++i )
    packages.insert(row + i, data.at(i));
  endInsertRows();

  qDebug() << packages;

  return true;
}

bool PackageModel::removeRows(int row, int count, const QModelIndex &parent)
{
  QStringList::iterator begin = packages.begin() + row;
  QStringList::iterator end = begin + count;

  beginRemoveRows( parent, row, row + count);
  packages.erase(begin, end);
  endRemoveRows();

  return true;
}

void PackageModel::clear()
{
  beginRemoveRows( QModelIndex(), 0, rowCount() );
  packages.clear();
  endRemoveRows();
}

void PackageModel::GetAvailable()
{
  disconnect(evaluator, SIGNAL(ready()), this, SLOT(GetAvailable()) );

  connect( evaluator, SIGNAL(output(QString)), SLOT(DisplayAvailable(QString)) );
  evaluator->eval( "load(\"/Users/westley/test-proj/available-packages.jl\")" );
}

void PackageModel::DisplayAvailable( QString output )
{
  QRegExp julia_prompt("julia>", Qt::CaseSensitive);
  QStringList package_list = output.remove(julia_prompt).split('\n');
  package_list.removeAll(QString(""));
  package_list.removeAll(QString(" "));
  insertRows(package_list, rowCount());
}
