#include "htmlviewer.h"
#include "htmlviewerconstants.h"

using namespace HtmlViewerPlugin;

HtmlViewer::HtmlViewer(QObject *parent) :
    IEditor(parent)
{
    file = new HtmlFile(this);
    webView = new QWebView();

    setWidget(webView);
}

bool HtmlViewer::createNew(const QString &contents)
{
    return false;
}

bool HtmlViewer::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    webView->load(realFileName);
    setDisplayName(fileName);
    emit changed();
    return true;
}

Core::IDocument *HtmlViewer::document()
{
    return file;
}

Core::Id HtmlViewer::id() const
{
    return Core::Id(Constants::HTMLVIEWER_ID);
}

QString HtmlViewer::displayName() const
{
    return name;
}

void HtmlViewer::setDisplayName(const QString &title)
{
    name = title;
}

bool HtmlViewer::duplicateSupported() const
{
    return false;
}

Core::IEditor *HtmlViewer::duplicate(QWidget *parent)
{
    return NULL;
}

QByteArray HtmlViewer::saveState() const
{
    return QByteArray();
}

bool HtmlViewer::restoreState(const QByteArray &state)
{
    return true;
}

bool HtmlViewer::isTemporary() const
{
    return false;
}

QWidget *HtmlViewer::toolBar()
{
    return NULL;
}

Core::Id HtmlViewer::preferredOpenMode() const
{
    return Core::Id(Core::Constants::O_SEPERATE);
}
