#ifndef JULIAFILEWIZARD_H
#define JULIAFILEWIZARD_H

#include <coreplugin/basefilewizard.h>

namespace JuliaPlugin {

class JuliaFileWizard : public Core::StandardFileWizard
{
  Q_OBJECT

public:
  typedef Core::BaseFileWizardParameters BaseFileWizardParameters;

  explicit JuliaFileWizard(const BaseFileWizardParameters &parameters,
                           QObject *parent = 0);

protected:
  Core::GeneratedFiles generateFilesFromPath(const QString &path,
                                             const QString &fileName,
                                             QString *errorMessage) const;
};

}

#endif // JULIAFILEWIZARD_H
