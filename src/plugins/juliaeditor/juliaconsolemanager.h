#ifndef JULIACONSOLEMANAGER_H
#define JULIACONSOLEMANAGER_H

#include "juliaconsolepane.h"

#include <QObject>

namespace JuliaPlugin {

class Run;
class LocalRun;
class Console;

class JuliaConsoleManager : public QObject
{
  Q_OBJECT
public:
  explicit JuliaConsoleManager(QObject *parent = 0);
  ~JuliaConsoleManager();

public slots:
  void InitConsole();
  void ShowConsolePane();

signals:
  
public slots:

private:
  Run* InternalCreateRun( const QString& working_dir );

  LocalRun* run;
  Console* console;
  JuliaConsolePane* console_pane;
};

}
#endif // JULIAMANAGER_H
