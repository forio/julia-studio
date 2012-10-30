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

JuliaConsoleManager::JuliaConsoleManager(QObject *parent)
  : QObject(parent), run(NULL), console(NULL)
{
  InitConsole();

  console_pane = new JuliaConsolePane( console, this );
  ExtensionSystem::PluginManager::addObject( console_pane );

  connect( Singleton<JuliaSettings>::GetInstance(), SIGNAL(PathToBinariesChanged(const QString&)), SLOT(ResetConsole()) );
}

JuliaConsoleManager::~JuliaConsoleManager()
{
  if ( console_pane )
    ExtensionSystem::PluginManager::removeObject( console_pane );
}

void JuliaConsoleManager::InitConsole()
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

void JuliaConsoleManager::ShowConsolePane()
{
  if ( console_pane )
    console_pane->popup(Core::IOutputPane::ModeSwitch);
}

void JuliaConsoleManager::ResetConsole()
{
  console->Reset(false);
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
