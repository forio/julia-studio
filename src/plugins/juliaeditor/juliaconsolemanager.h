#ifndef JULIACONSOLEMANAGER_H
#define JULIACONSOLEMANAGER_H

#include "juliaconsolepane.h"
#include "juliarunconfiguration.h"

#include <projectexplorer/runconfiguration.h>

#include <QObject>

namespace JuliaPlugin {

class Run;
class LocalRun;
class Console;

class JuliaConsoleManager : public ProjectExplorer::RunControl
{
  Q_OBJECT
public:
  explicit JuliaConsoleManager(ProjectExplorer::RunConfiguration* config, ProjectExplorer::RunMode mode);
  ~JuliaConsoleManager();

public slots:
  virtual void start();
  virtual ProjectExplorer::RunControl::StopResult stop();
  virtual bool isRunning() const;
  virtual QIcon icon() const;

  virtual void reset(bool keep_history = false);
  virtual void showConsolePane();

private slots:
  void init();

private:
  Run* InternalCreateRun( const QString& working_dir );

  LocalRun* run;
  Console* console;
  JuliaConsolePane* console_pane;
};

}
#endif // JULIAMANAGER_H
