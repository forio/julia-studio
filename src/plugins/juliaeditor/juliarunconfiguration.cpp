#include "juliarunconfiguration.h"
#include <projectexplorer/runconfiguration.h>

namespace JuliaPlugin {

JuliaRunConfiguration::JuliaRunConfiguration(ProjectExplorer::Target* parent, const Core::Id id) :
    RunConfiguration(parent, id)
{
}

JuliaRunConfiguration::JuliaRunConfiguration(ProjectExplorer::Target *parent, RunConfiguration *source) :
    RunConfiguration(parent, source)
{
}


QWidget *JuliaRunConfiguration::createConfigurationWidget()
{
    return NULL;
}

}
