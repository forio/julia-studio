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
  if (evaluator->isRunning())
    GetAvailable();
  else
  {
    connect(evaluator, SIGNAL(ready()), SLOT(GetAvailable()));
  }
}

void PackageController::OnNewPackageView(PackageView *package_view)
{
  package_view->SetPackageModel(model);
}

void PackageController::GetAvailable()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.type = JM_PACKAGE;
  msg.params.push_back("available");

  evaluator->eval(msg);
}

void PackageController::AddPackage(const QModelIndex &index)
{
}

void PackageController::UpdateAvailable()
{
}

void PackageController::EvaluatorOutput(const ProjectExplorer::EvaluatorMessage *msg)
{
  if ( msg->type != JM_OUTPUT_PACKAGE )
    return;

  qDebug() << "RECEIVING REQUESTED PACKAGE OUTPUT";
}
