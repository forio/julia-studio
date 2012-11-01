#include "juliarunconfigurationfactory.h"
#include "juliarunconfiguration.h"
#include <QDebug>

using namespace ProjectExplorer;

namespace JuliaPlugin {

JuliaRunConfigurationFactory::JuliaRunConfigurationFactory(QObject *parent)  {

}
JuliaRunConfigurationFactory::~JuliaRunConfigurationFactory() {

}

QList<Core::Id> JuliaRunConfigurationFactory::availableCreationIds(Target *parent) const {
     QList<Core::Id> list;
     list << Core::Id("1");
     return list;
}

QString JuliaRunConfigurationFactory::displayNameForId(const Core::Id id) const {
    return "Ihhhaaa";
}

bool JuliaRunConfigurationFactory::canCreate(Target *parent, const Core::Id id) const {
    return true;
}

RunConfiguration *JuliaRunConfigurationFactory::create(Target *parent, const Core::Id id) {
  qDebug() << parent;
    return new JuliaRunConfiguration(parent, id);
}

bool JuliaRunConfigurationFactory::canRestore(Target *parent, const QVariantMap &map) const {
    return true;
}

RunConfiguration *JuliaRunConfigurationFactory::restore(Target *parent, const QVariantMap &map) {
    return NULL;
}

bool JuliaRunConfigurationFactory::canClone(Target *parent, RunConfiguration *product) const {
    return true;
}

RunConfiguration *JuliaRunConfigurationFactory::clone(Target *parent, RunConfiguration *product) {
    return NULL;
}
}
