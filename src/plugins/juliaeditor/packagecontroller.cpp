#include "packagecontroller.h"
#include "localtcpevaluator.h"
#include "juliamsg.h"

#include <extensionsystem/iplugin.h>

#include <QToolButton>

using namespace JuliaPlugin;

PackageController::PackageController(LocalTcpEvaluator* evaluator_, Console* console_, QObject *parent) :
  QObject(parent), evaluator(evaluator_), console(console_), busy(false)
{
  model = new PackageModel;

  connect(evaluator, SIGNAL(output(const ProjectExplorer::EvaluatorMessage*)), SLOT(EvaluatorOutput(const ProjectExplorer::EvaluatorMessage*)));
  GetAvailable();
  GetRequired();
}

void PackageController::OnConsoleReset()
{
  connect(console, SIGNAL(Ready(Console*)), SLOT(ResetOnConsoleReady()));
}

void PackageController::OnNewPackageView(Core::NavigationView* view)
{
  PackageView* package_view = qobject_cast<PackageView*>(view->widget);
  package_view->SetPackageModel(model);
  connect(package_view->list_view, SIGNAL(doubleClicked(QModelIndex)), SLOT(TogglePackage(QModelIndex)));

  QToolButton* update_buttong = view->dockToolBarWidgets.front();
  connect(update_buttong, SIGNAL(clicked()), SLOT(UpdatePackages()));
}

void PackageController::GetAvailable()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("available");

  evaluator->eval(msg);
}

void PackageController::GetRequired()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("required");

  evaluator->eval(msg);
}

void PackageController::AddPackage(const QModelIndex &index)
{
  if (busy)
    return;

  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("add");

  PackageData data = model->data(index, Qt::UserRole).value<PackageData>();
  msg.params.push_back(data.name);

  busy = true;
  console->SetBusy("adding " + data.name);
  evaluator->eval(msg);

  GetRequired();
}

void PackageController::RemovePackage(const QModelIndex &index)
{
  if (busy)
    return;

  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("remove");

  PackageData data = model->data(index, Qt::UserRole).value<PackageData>();
  msg.params.push_back( data.name );

  busy = true;
  console->SetBusy("removing " + data.name);
  evaluator->eval(msg);

  GetRequired();
}

void PackageController::UpdatePackages()
{
  if (busy == true)
    return;

  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("update");

  busy = true;
  console->SetBusy("Updating packages");
  evaluator->eval(msg);

  GetAvailable();
  GetRequired();
}

void PackageController::TogglePackage(const QModelIndex &index)
{
  PackageData pdata = model->data(index, Qt::UserRole).value<PackageData>();
  if (pdata.required)
    RemovePackage(index);
  else
    AddPackage(index);
}

void PackageController::EvaluatorOutput(const ProjectExplorer::EvaluatorMessage *msg)
{
  if ( msg->type != JM_OUTPUT_PACKAGE )
    return;

  else if ( msg->params[0] == "available" )
  {
    busy = false;

    QList<PackageData> package_list;
    for ( int i = 1; i < msg->params.size(); ++i )
      package_list.append(PackageData(msg->params[i]));

    model->insertRows(package_list, model->rowCount());
  }
  else if ( msg->params[0] == "required" )
  {
    busy = false;
    model->invalidateAll();

    QList<PackageData> package_list;
    for ( int i = 1; i < msg->params.size(); ++i )
      package_list.append(PackageData(msg->params[i], true));

    model->insertRows(package_list, model->rowCount());
  }
}

void PackageController::ResetOnConsoleReady()
{
  disconnect(console, SIGNAL(Ready(Console*)), this, SLOT(ResetOnConsoleReady()));
  busy = false;
  GetAvailable();
  GetRequired();
}
