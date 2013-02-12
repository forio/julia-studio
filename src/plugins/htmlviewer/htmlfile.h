#ifndef HTMLFILE_H
#define HTMLFILE_H

#include <coreplugin/idocument.h>

namespace HtmlViewerPlugin {

class HtmlViewer;

class HtmlFile : public Core::IDocument
{
    Q_OBJECT

public:
    explicit HtmlFile(HtmlViewer* editor_, QObject *parent = 0);

public slots:
    virtual bool save(QString *errorString, const QString &fileName = QString(), bool autoSave = false);
    virtual QString fileName() const;

    virtual QString defaultPath() const;
    virtual QString suggestedFileName() const;
    virtual QString mimeType() const;

    virtual bool isModified() const;
    virtual bool isSaveAsAllowed() const;

    virtual bool reload(QString *errorString, ReloadFlag flag, ChangeType type);
    virtual void rename(const QString &newName);
    
signals:

private:
    HtmlViewer* editor;
    QString file_name;
};

}
#endif // HTMLFILE_H
