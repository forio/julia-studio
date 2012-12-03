#ifndef PACKAGEMODEL_H
#define PACKAGEMODEL_H

#include "LocalEvaluator.h"

#include <QAbstractListModel>

namespace JuliaPlugin {

class PackageData
{
public:
  QString name;
};

class PackageModel : public QAbstractListModel
{
  Q_OBJECT

public:
  explicit PackageModel(QObject *parent = 0);

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  virtual bool insertRows(const QStringList& data, int row, const QModelIndex &parent = QModelIndex());
  virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

  virtual void clear();

private slots:
  void GetAvailable();
  void DisplayAvailable(QString output);

private:

  QStringList packages;

  LocalEvaluator* evaluator;
  
};

}

#endif // PACKAGEMODEL_H
