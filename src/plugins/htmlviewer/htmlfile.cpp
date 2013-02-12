#include "htmlfile.h"
#include "htmlviewer.h"

#include <QFileInfo>

using namespace HtmlViewerPlugin;

HtmlFile::HtmlFile(HtmlViewer* editor_, QObject *parent) :
    Core::IDocument(parent), editor(editor_)
{
}

bool HtmlFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    return false;
}

QString HtmlFile::fileName() const
{
    return file_name;
}

QString HtmlFile::defaultPath() const
{
    return QString();
}

QString HtmlFile::suggestedFileName() const
{
    return QString();
}

QString HtmlFile::mimeType() const
{
    return "web/html";
}

bool HtmlFile::isModified() const
{
    return false;
}

bool HtmlFile::isSaveAsAllowed() const
{
    return false;
}

bool HtmlFile::reload(QString *errorString, Core::IDocument::ReloadFlag flag, Core::IDocument::ChangeType type)
{
    if (flag == FlagIgnore)
        return true;
    if (type == TypePermissions) {
        emit changed();
        return true;
    }
    return editor->open(errorString, file_name, file_name);
}

void HtmlFile::rename(const QString &new_name)
{
    const QString old_name = file_name;
    file_name = new_name;
    editor->setDisplayName(QFileInfo(file_name).fileName());

    emit fileNameChanged(old_name, new_name);
    emit changed();
}
