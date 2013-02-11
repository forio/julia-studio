#ifndef HTMLVIEWERFACTORY_H
#define HTMLVIEWERFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/idocument.h>

#include <QStringList>

namespace HtmlViewerPlugin {

class HtmlViewerFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    explicit HtmlViewerFactory(QObject *parent = 0);

public slots:
    Core::IEditor* createEditor(QWidget* parent);

    QStringList mimeTypes() const;
    Core::Id id() const;
    QString displayName() const;
    Core::IDocument *open(const QString &fileName);
    
signals:

private:
    QStringList mimeTypeList;
};

}

#endif // HTMLVIEWERFACTORY_H
