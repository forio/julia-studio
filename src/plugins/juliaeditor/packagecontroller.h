#ifndef PACKAGECONTROLLER_H
#define PACKAGECONTROLLER_H

#include <QObject>

#include "packageview.h"
#include "packagemodel.h"
#include "console.h"

namespace JuliaPlugin {

class LocalTcpEvaluator;

class PackageController : public QObject
{
  Q_OBJECT

public:
  explicit PackageController(LocalTcpEvaluator* evaluator_, Console* console_, QObject *parent = 0);

public slots:
  void OnConsoleReset();
  void OnNewPackageView(PackageView* package_view);

  void GetAvailable();
  void GetInstalled();
  void AddPackage(const QModelIndex& index);
  void RemovePackage(const QModelIndex& index);

private slots:
  void UpdateAvailable();
  void TogglePackage(const QModelIndex& index);

  void EvaluatorOutput(const ProjectExplorer::EvaluatorMessage* msg);
  void ResetOnConsoleReady();

signals:

private:
  PackageModel* model;
  QStringList installed_packages;

  LocalTcpEvaluator* evaluator;
  Console* console;
  bool busy;
};

}

#endif // PACKAGECONTROLLER_H
