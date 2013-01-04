#include "packagecontroller.h"
#include "localtcpevaluator.h"
#include "juliamsg.h"

#include <extensionsystem/iplugin.h>

using namespace JuliaPlugin;

PackageController::PackageController(LocalTcpEvaluator* evaluator_, QObject *parent) :
  QObject(parent), evaluator(evaluator_)
{
  model = new PackageModel;

  connect(evaluator, SIGNAL(output(const ProjectExplorer::EvaluatorMessage*)), SLOT(EvaluatorOutput(const ProjectExplorer::EvaluatorMessage*)));
  GetAvailable();
  GetInstalled();
}

void PackageController::OnNewPackageView(PackageView *package_view)
{
  package_view->SetPackageModel(model);

  connect(package_view->list_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(TogglePackage(QModelIndex)));
}

void PackageController::GetAvailable()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("available");

  evaluator->eval(msg);
}

void PackageController::GetInstalled()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("installed");

  evaluator->eval(msg);
}

void PackageController::AddPackage(const QModelIndex &index)
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("add");

  PackageData data = model->data(index, Qt::UserRole).value<PackageData>();
  msg.params.push_back(data.name);

  data.installed = true;
  qDebug() << data.name << data.installed;
  qDebug() << model->setData(index, QVariant::fromValue(data));

  evaluator->eval(msg);
}

void PackageController::RemovePackage(const QModelIndex &index)
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("remove");

  PackageData data = model->data(index, Qt::UserRole).value<PackageData>();
  msg.params.push_back( data.name );

  data.installed = false;
  qDebug() << model->setData(index, QVariant::fromValue(data));

  evaluator->eval(msg);
}

void PackageController::UpdateAvailable()
{
}

void PackageController::TogglePackage(const QModelIndex &index)
{
  PackageData pdata = model->data(index, Qt::UserRole).value<PackageData>();
  if (pdata.installed)
    RemovePackage(index);
  else
    AddPackage(index);
}

void PackageController::EvaluatorOutput(const ProjectExplorer::EvaluatorMessage *msg)
{
  if ( msg->type != JM_OUTPUT_PACKAGE )
    return;

  if ( msg->params[0] == "available" )
  {
    qDebug() << "RECEIVING AVAILABLE PACKAGE OUTPUT";
    QList<PackageData> package_list;
    for ( int i = 1; i < msg->params.size(); ++i )
      package_list.append(PackageData(msg->params[i]));

    model->insertRows(package_list, model->rowCount());
  }
  if ( msg->params[0] == "installed" )
  {
    QList<PackageData> package_list;
    for ( int i = 1; i < msg->params.size(); ++i )
      package_list.append(PackageData(msg->params[i], true));

    model->insertRows(package_list, model->rowCount());
  }
}
