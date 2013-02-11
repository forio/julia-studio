#include "htmlviewer.h"

using namespace HtmlViewerPlugin;

HtmlViewer::HtmlViewer(QObject *parent) :
    IEditor(parent)
{
}

bool HtmlViewer::createNew(const QString &contents)
{
}

bool HtmlViewer::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
}

Core::IDocument *HtmlViewer::document()
{
}

Core::Id HtmlViewer::id() const
{
}

QString HtmlViewer::displayName() const
{
}

void HtmlViewer::setDisplayName(const QString &title)
{
}

bool HtmlViewer::duplicateSupported() const
{
}

Core::IEditor *HtmlViewer::duplicate(QWidget *parent)
{
}

QByteArray HtmlViewer::saveState() const
{
    return QByteArray();
}

bool HtmlViewer::restoreState(const QByteArray &state)
{
}

bool HtmlViewer::isTemporary() const
{
}

QWidget *HtmlViewer::toolBar()
{
}
