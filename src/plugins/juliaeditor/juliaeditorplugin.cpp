#include "juliaeditorplugin.h"
#include "juliaeditor_constants.h"
#include "juliaeditor.h"
#include "juliasettingspage.h"
#include "singleton.h"
#include "juliaruncontrolfactory.h"
#include "juliarunconfigurationfactory.h"
#include "juliadummyproject.h"
#include "juliaprojectmanager.h"
#include "juliaconsolepane.h"
#include "localevaluator.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>
#include <texteditor/texteditorplugin.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/texteditoractionhandler.h>
#include <extensionsystem/pluginmanager.h>

#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>

#include <QtCore/QtPlugin>
#include <QCoreApplication>

using namespace JuliaPlugin;
using namespace JuliaPlugin::Internal;

// JuliaEditorPlugin *******

JuliaEditorPlugin::JuliaEditorPlugin()
  : action_handler(NULL), evaluator(NULL), console_pane(NULL)
{}

JuliaEditorPlugin::~JuliaEditorPlugin()
{
  // Unregister objects from the plugin manager's object pool
  // Delete members

  if (console_pane)
    ExtensionSystem::PluginManager::removeObject(console_pane);

  delete action_handler;
}

bool JuliaEditorPlugin::initialize(const QStringList &arguments, QString *errorString)
{
  Q_UNUSED(arguments)
  Q_UNUSED(errorString)

  // "In the initialize method, a plugin can be sure that the plugins it
  //  depends on have initialized their members."

  if (!Core::ICore::mimeDatabase()->addMimeTypes(QLatin1String(":/juliaeditor/juliaeditor.mimetypes.xml"), errorString))
      return false;

  addAutoReleasedObject( new JuliaSettingsPage() );
  Singleton<JuliaSettings>::GetInstance()->FromSettings(Core::ICore::settings());

  JuliaProjectManager* project_manager = new JuliaProjectManager();
  addAutoReleasedObject( project_manager );
  addAutoReleasedObject(new JuliaRunConfigurationFactory());
  addAutoReleasedObject(new JuliaRunControlFactory());

  JuliaEditorFactory* editor_factory = new JuliaEditorFactory(this);
  connect( editor_factory, SIGNAL(newEditor(JuliaEditorWidget*)), SLOT(initEditor(JuliaEditorWidget*)) );
  addAutoReleasedObject( editor_factory );

  action_handler = new TextEditor::TextEditorActionHandler( Constants::JULIAEDITOR,
    TextEditor::TextEditorActionHandler::Format
    | TextEditor::TextEditorActionHandler::UnCommentSelection
    | TextEditor::TextEditorActionHandler::UnCollapseAll
    | TextEditor::TextEditorActionHandler::FollowSymbolUnderCursor );

  action_handler->initializeActions();

  // Julia console -------
  evaluator = new LocalEvaluator(this);
  console_pane = new JuliaConsolePane(this);
  Console* console = console_pane->outputWidget();

  connect( evaluator, SIGNAL( ready() ), console, SLOT( BeginCommand() ) );
  connect( console, SIGNAL( NewCommand(QString) ), evaluator, SLOT( eval(const QString&) ) );
  connect( evaluator, SIGNAL( output(const QString&) ), console, SLOT( DisplayResult(const QString&) ) );
  //connect( console, SIGNAL( destroyed() ), evaluator, SLOT( kill() ) );
  connect( console, SIGNAL( Reseting(bool) ), evaluator, SLOT( reset() ) );
  connect( Singleton<JuliaSettings>::GetInstance(), SIGNAL(PathToBinariesChanged(const QString&)), console, SLOT(Reset()) );

  addAutoReleasedObject(evaluator);
  ExtensionSystem::PluginManager::addObject(console_pane);
  //addAutoReleasedObject(console_pane);
  // ------- */


  Core::ActionManager *am = Core::ICore::instance()->actionManager();

  QAction *action = new QAction(tr("Reset Console"), this);
  Core::Command *cmd = am->registerAction(action, Constants::ACTION_ID_RESET_CONSOLE,
                                          Core::Context(Core::Constants::C_GLOBAL));
  cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
  connect(action, SIGNAL(triggered()), console, SLOT(Reset()));

  Core::ActionContainer *menu = am->createMenu(Constants::MENU_ID);
  menu->menu()->setTitle(tr("Julia"));
  menu->addAction(cmd);
  am->actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
  
  return true;
}

void JuliaEditorPlugin::extensionsInitialized()
{
  // Retrieve objects from the plugin manager's object pool
  // "In the extensionsInitialized method, a plugin can be sure that all
  //  plugins that depend on it are completely initialized."
}

ExtensionSystem::IPlugin::ShutdownFlag JuliaEditorPlugin::aboutToShutdown()
{
  // Save settings
  // Disconnect from signals that are not needed during shutdown
  // Hide UI (if you add UI that is not in the main window directly)
  return SynchronousShutdown;
}

void JuliaEditorPlugin::triggerAction()
{
  QMessageBox::information(Core::ICore::instance()->mainWindow(),
                           tr("Action triggered"),
                           tr("This is an action from JuliaEditor."));
}

void JuliaEditorPlugin::initEditor( TextEditor::BaseTextEditorWidget* editor )
{
  action_handler->setupActions( editor );  // this should be a slot!
  //editor->setLanguageSettingsId( QLatin1String( Constants::JULIA_SETTINGS_ID ) );
  TextEditor::TextEditorSettings::instance()->initializeEditor( editor );
}


// JuliaEditorFactory *******

JuliaEditorFactory::JuliaEditorFactory(JuliaEditorPlugin *plugin_)
  : plugin(plugin_)
{
  mime_types << QLatin1String(Constants::JULIA_MIMETYPE)
             << QLatin1String(Constants::JULIA_PROJECT_MIMETYPE);
}

Core::IEditor* JuliaEditorFactory::createEditor(QWidget *parent)
{
  JuliaEditorWidget* editor = new JuliaEditorWidget(parent);
  editor->setRevisionsVisible(true);

  emit newEditor( editor );

  return editor->editor();
}

Core::Id JuliaEditorFactory::id() const
{
  return JuliaPlugin::Constants::JULIAEDITOR_ID;
}

QString JuliaEditorFactory::displayName() const
{
  return qApp->translate( "OpenWith::Editors", Constants::JULIAEDITOR_DISPLAY_NAME);
}


Q_EXPORT_PLUGIN2(JuliaEditor, JuliaEditorPlugin)

