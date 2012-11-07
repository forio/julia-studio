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
#include <coreplugin/editormanager/editormanager.h>

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
  : action_handler(NULL), evaluator(NULL), console_pane(NULL), load_action(NULL)
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

  // Types and settings -------
  if (!Core::ICore::mimeDatabase()->addMimeTypes(QLatin1String(":/juliaeditor/juliaeditor.mimetypes.xml"), errorString))
      return false;

  addAutoReleasedObject( new JuliaSettingsPage() );
  Singleton<JuliaSettings>::GetInstance()->FromSettings(Core::ICore::settings());
  // ------- */

  // Editors -------
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
  // ------- */

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
  // ------- */

  Core::ActionManager *am = Core::ICore::instance()->actionManager();

  // Menu -------
  Core::ActionContainer *menu = am->createMenu(Constants::MENU_ID);
  menu->menu()->setTitle(tr("Julia"));
  am->actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);
  // ------- */

  // Actions -------
  QAction *action = new QAction(tr("Reset Console"), this);
  Core::Command *cmd = am->registerAction(action, Constants::ACTION_ID_RESET_CONSOLE, Core::Context(Core::Constants::C_GLOBAL));
  cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Alt+Meta+A")));
  connect(action, SIGNAL(triggered()), console, SLOT(Reset()));
  menu->addAction(cmd);

  cmd = am->command( ProjectExplorer::Constants::RUN );
  load_action = cmd->action();
  connect( load_action, SIGNAL(triggered()), SLOT(evalCurrFile()) );
  connect( Core::EditorManager::instance(), SIGNAL(editorOpened(Core::IEditor*)), SLOT(updateLoadAction()) );
  connect( Core::EditorManager::instance(), SIGNAL(editorsClosed(QList<Core::IEditor*>)), SLOT(updateLoadAction()) );
  updateLoadAction();
  // ------- */
  
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

void JuliaEditorPlugin::initEditor( JuliaEditorWidget* editor )
{
  action_handler->setupActions( editor );  // setupActions should be a slot!
  TextEditor::TextEditorSettings::instance()->initializeEditor( editor );
}

void JuliaEditorPlugin::evalCurrFile()
{
  // popup regardless, the user obviously wants to execute some code
  console_pane->popup( Core::IOutputPane::ModeSwitch | Core::IOutputPane::WithFocus );

  Core::IEditor* editor = Core::EditorManager::currentEditor();
  if ( editor )
  {
    Core::EditorManager* manager = Core::EditorManager::instance();
    foreach( Core::IEditor* editor, manager->openedEditors() )
    {
      if ( editor->document()->isModified() )
        manager->saveEditor( editor );
    }

    Core::IDocument* document = editor->document();
    QFileInfo file_info(document->fileName());
    evaluator->eval(&file_info);
  }
}

void JuliaEditorPlugin::updateLoadAction()
{
  Core::EditorManager* manager = Core::EditorManager::instance();
  load_action->setEnabled( manager->openedEditors().size() );
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

