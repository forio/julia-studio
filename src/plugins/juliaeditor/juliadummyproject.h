#ifndef JULIADUMMYPROJECT_H
#define JULIADUMMYPROJECT_H

#include <projectexplorer/project.h>

namespace ProjectExplorer { class IProjectManager; }

namespace JuliaPlugin {

class JuliaDummyProject : public ProjectExplorer::Project
{
  Q_OBJECT

public:
  explicit JuliaDummyProject(QObject *parent = 0);

  virtual QString displayName() const;
  virtual Core::Id id() const;
  virtual Core::IDocument *document() const;
  virtual ProjectExplorer::IProjectManager *projectManager() const;

  virtual ProjectExplorer::ProjectNode *rootProjectNode() const;

  virtual QStringList files(FilesMode fileMode) const;

signals:
  
public slots:
  
};

}

#endif // JULIADUMMYPROJECT_H
