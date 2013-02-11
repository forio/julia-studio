#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/idocument.h>
#include <coreplugin/coreconstants.h>

namespace HtmlViewerPlugin {

class HtmlViewer : public Core::IEditor
{
    Q_OBJECT

public:
    explicit HtmlViewer(QObject *parent = 0);

public slots:
    virtual bool createNew(const QString &contents = QString());
    virtual bool open(QString *errorString, const QString &fileName, const QString &realFileName);
    virtual Core::IDocument *document();
    virtual Core::Id id() const;
    virtual QString displayName() const;
    virtual void setDisplayName(const QString &title);

    virtual bool duplicateSupported() const;
    virtual IEditor *duplicate(QWidget *parent);

    virtual QByteArray saveState() const;
    virtual bool restoreState(const QByteArray &state);

    virtual bool isTemporary() const;

    virtual QWidget *toolBar();

    virtual Core::Id preferredOpenMode() const { return Core::Id(Core::Constants::O_SEPERATE); }
    
signals:
    
private:

};

}

#endif // HTMLVIEWER_H
