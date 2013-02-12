#include "htmlviewerplugin.h"
#include "htmlviewerconstants.h"
#include "htmlviewerfactory.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace HtmlViewerPlugin::Internal;

HtmlViewPlugin::HtmlViewPlugin()
{
    // Create your members
}

HtmlViewPlugin::~HtmlViewPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool HtmlViewPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    if (!Core::ICore::mimeDatabase()->addMimeTypes(":/htmlviewer/HtmlViewer.mimetypes.xml", errorString))
        return false;

    factory = new HtmlViewerFactory();
    addAutoReleasedObject(factory);
    
    return true;
}

void HtmlViewPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized method, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag HtmlViewPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void HtmlViewPlugin::triggerAction()
{
    QMessageBox::information(Core::ICore::mainWindow(),
                             tr("Action triggered"),
                             tr("This is an action from HtmlViewer."));
}

Q_EXPORT_PLUGIN2(HtmlViewer, HtmlViewPlugin)

