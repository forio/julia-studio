#include "juliadummyproject.h"

#include <texteditor/basetextdocument.h>

namespace JuliaPlugin {

JuliaDummyProject::JuliaDummyProject(QObject *parent) :
  ProjectExplorer::Project()
{}

QString JuliaDummyProject::displayName() const
{
  return "JULIA DUMMY PROJECT";
}

Core::Id JuliaDummyProject::id() const
{
  return Core::Id("22");
}

Core::IDocument *JuliaDummyProject::document() const
{
  return new TextEditor::BaseTextDocument();
}

ProjectExplorer::IProjectManager *JuliaDummyProject::projectManager() const
{
  return NULL;
}

ProjectExplorer::ProjectNode *JuliaDummyProject::rootProjectNode() const
{
  return NULL;
}

QStringList JuliaDummyProject::files(ProjectExplorer::Project::FilesMode fileMode) const
{
  return QStringList();
}

}
