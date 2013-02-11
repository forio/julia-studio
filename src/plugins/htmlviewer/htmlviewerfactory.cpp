#include "htmlviewerfactory.h"
#include "htmlviewer.h"
#include "htmlviewerconstants.h"

using namespace HtmlViewerPlugin;

HtmlViewerFactory::HtmlViewerFactory(QObject *parent) :
    IEditorFactory(parent)
{
    mimeTypeList.append("web/html");
}


Core::IEditor *HtmlViewerPlugin::HtmlViewerFactory::createEditor(QWidget *parent)
{
    return new HtmlViewer(parent);
}

QStringList HtmlViewerPlugin::HtmlViewerFactory::mimeTypes() const
{
    return mimeTypes();
}

Core::Id HtmlViewerPlugin::HtmlViewerFactory::id() const
{
    return Core::Id(Constants::HTMLVIEWER_ID);
}

QString HtmlViewerPlugin::HtmlViewerFactory::displayName() const
{
    return Constants::HTMLVIEWER_DISPLAY_NAME;
}

Core::IDocument *HtmlViewerPlugin::HtmlViewerFactory::open(const QString &fileName)
{
    return NULL;
}
