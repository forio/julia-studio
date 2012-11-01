#ifndef JULIARUNCONFIGURATIONFACTORY_H
#define JULIARUNCONFIGURATIONFACTORY_H

#include <projectexplorer/runconfiguration.h>

namespace JuliaPlugin {

class JuliaRunConfiguration;

class JuliaRunConfigurationFactory : public ProjectExplorer::IRunConfigurationFactory
{
    Q_OBJECT
public:
    explicit JuliaRunConfigurationFactory(QObject *parent = 0);
    virtual ~JuliaRunConfigurationFactory();

    virtual QList<Core::Id> availableCreationIds(ProjectExplorer::Target *parent) const;
    virtual QString displayNameForId(const Core::Id id) const;

    virtual bool canCreate(ProjectExplorer::Target *parent, const Core::Id id) const;
    virtual ProjectExplorer::RunConfiguration *create(ProjectExplorer::Target *parent, const Core::Id id);
    virtual bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const;
    virtual ProjectExplorer::RunConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map);
    virtual bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product) const;
    virtual ProjectExplorer::RunConfiguration *clone(ProjectExplorer::Target *parent, ProjectExplorer::RunConfiguration *product);
};

}
#endif // JULIARUNCONFIGURATIONFACTORY_H
