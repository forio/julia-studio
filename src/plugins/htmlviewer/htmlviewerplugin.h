#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

#include "htmlviewer_global.h"

#include <extensionsystem/iplugin.h>

namespace HtmlViewerPlugin {
    class HtmlViewerFactory;

namespace Internal {

class HtmlViewPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    
public:
    HtmlViewPlugin();
    ~HtmlViewPlugin();
    
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();
    
private slots:
    void triggerAction();

private:
    HtmlViewerFactory* factory;
};

} // namespace Internal
} // namespace HtmlViewer

#endif // HTMLVIEWER_H

