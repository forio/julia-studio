#ifndef JULIARUNCONFIGURATION_H
#define JULIARUNCONFIGURATION_H

#include <projectexplorer/runconfiguration.h>

namespace JuliaPlugin
{

class JuliaRunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT
    friend class JuliaRunConfigurationFactory;
public:
    JuliaRunConfiguration(ProjectExplorer::Target* parent, const Core::Id id);

    virtual QWidget *createConfigurationWidget();

protected:
    JuliaRunConfiguration(ProjectExplorer::Target *parent, RunConfiguration *source);

};
}
#endif // JULIARUNCONFIGURATION_H
