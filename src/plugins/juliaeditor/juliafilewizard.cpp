#include "juliafilewizard.h"
#include "juliaeditor_constants.h"

using namespace JuliaPlugin;

JuliaFileWizard::JuliaFileWizard(const JuliaFileWizard::BaseFileWizardParameters &parameters, QObject *parent)
  : Core::StandardFileWizard(parameters, parent)
{}

Core::GeneratedFiles JuliaFileWizard::generateFilesFromPath(const QString &path, const QString &name, QString *errorMessage) const
{
  const QString mimeType = QLatin1String(Constants::JULIA_MIMETYPE);
  const QString fileName = Core::BaseFileWizard::buildFileName(path, name, preferredSuffix(mimeType));

  Core::GeneratedFile file(fileName);
  file.setAttributes(Core::GeneratedFile::OpenEditorAttribute);
  return Core::GeneratedFiles() << file;
}
