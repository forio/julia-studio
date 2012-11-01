#ifndef JULIARUNCONTROLFACTORY_H
#define JULIARUNCONTROLFACTORY_H

#include <projectexplorer/runconfiguration.h>
namespace JuliaPlugin {

    class JuliaRunControlFactory : public ProjectExplorer::IRunControlFactory
    {
        Q_OBJECT
    public:
        explicit JuliaRunControlFactory(QObject *parent = 0);
        virtual ~JuliaRunControlFactory();

        virtual bool canRun(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode mode) const;
        virtual ProjectExplorer::RunControl *create(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode mode, QString *errorMessage);

        virtual QString displayName() const;
    };

}
#endif // JULIARUNCONTROLFACTORY_H
