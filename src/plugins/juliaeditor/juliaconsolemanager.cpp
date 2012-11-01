#include "juliaconsolemanager.h"
#include "console.h"
#include "run.h"
#include "juliaeditor_constants.h"
#include "singleton.h"
#include "juliasettingspage.h"

#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorsettings.h>

#include <QDir>

using namespace JuliaPlugin;

JuliaConsoleManager::JuliaConsoleManager(ProjectExplorer::RunConfiguration* config, ProjectExplorer::RunMode mode)
  : ProjectExplorer::RunControl(config, mode), run(NULL), console(NULL)
{}

JuliaConsoleManager::~JuliaConsoleManager()
{
  stop();
}

void JuliaConsoleManager::start()
{
  init();

  console_pane = new JuliaConsolePane( console, this );
  ExtensionSystem::PluginManager::addObject( console_pane );

  connect( Singleton<JuliaSettings>::GetInstance(), SIGNAL(PathToBinariesChanged(const QString&)), SLOT(ResetConsole()) );
}

ProjectExplorer::RunControl::StopResult JuliaConsoleManager::stop()
{
  if ( console_pane )
    ExtensionSystem::PluginManager::removeObject( console_pane );

  return ProjectExplorer::RunControl::AsynchronousStop;
}

bool JuliaConsoleManager::isRunning() const
{
  return true;
}

QIcon JuliaConsoleManager::icon() const
{
  return QIcon();
}

void JuliaConsoleManager::showConsolePane()
{
  if ( console_pane )
    console_pane->popup(Core::IOutputPane::ModeSwitch);
}

void JuliaConsoleManager::reset( bool keep_history )
{
  console->Reset(keep_history);
}

void JuliaConsoleManager::init()
{
  if ( console )
    console->deleteLater();

  console = new Console();
  Run* run = InternalCreateRun( QDir().absolutePath() );

  console->setLanguageSettingsId( QLatin1String( Constants::JULIA_SETTINGS_ID ) );
  TextEditor::TextEditorSettings::instance()->initializeEditor( console );
  //console->SetOutputColor( QColor( Qt::blue ) );
  //console->setFont( QFont( "Monaco, Consolas, Courier New, Courier", 12 ) );

  connect( run, SIGNAL( Ready() ), console, SLOT( BeginCommand() ) );
  connect( console, SIGNAL( NewCommand(QString) ), run, SLOT( Evaluate(QString) ) );
  connect( run, SIGNAL( Output(QString) ), console, SLOT( DisplayResult(QString) ) );
  connect( console, SIGNAL( destroyed() ), run, SLOT( Kill() ) );
  connect( console, SIGNAL( Reseting(bool) ), run, SLOT( Reset() ) );

  run->SetMode( Run::Mode_KeepAlive );
}

Run *JuliaConsoleManager::InternalCreateRun(const QString &working_dir)
{
  if ( run )
    run->deleteLater();

  run = new LocalRun();
  //connect( run, SIGNAL( Finished(Run*) ), SLOT( OnRunFinished(Run*) ) );
  run->SetWorkingDir( working_dir );

  return run;
}
