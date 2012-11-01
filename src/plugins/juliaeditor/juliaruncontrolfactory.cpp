#include "juliaruncontrolfactory.h"
#include "juliarunconfiguration.h"
#include "juliaconsolemanager.h"

namespace JuliaPlugin {

    JuliaRunControlFactory::JuliaRunControlFactory(QObject *parent)
    {

    }

    JuliaRunControlFactory::~JuliaRunControlFactory() {

    }

    bool JuliaRunControlFactory::canRun(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode mode) const {
        return /*mode == NormalRunMode &&*/ qobject_cast<JuliaRunConfiguration *>(runConfiguration);
    }

    ProjectExplorer::RunControl *JuliaRunControlFactory::create(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode mode, QString *errorMessage) {
      return new JuliaConsoleManager( runConfiguration, mode );
    }

    QString JuliaRunControlFactory::displayName() const {
        return "julia!!!";
    }
}
