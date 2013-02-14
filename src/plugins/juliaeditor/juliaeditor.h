#ifndef JULIAEDITOR_H
#define JULIAEDITOR_H

#include <texteditor/basetexteditor.h>

namespace JuliaPlugin {

class JuliaEditorWidget;

class JuliaEditor : public TextEditor::BaseTextEditor
{
  Q_OBJECT

public:
  JuliaEditor(TextEditor::BaseTextEditorWidget *widget);

  bool duplicateSupported() const  { return true; }
  Core::IEditor* duplicate(QWidget *parent);

  Core::Id id() const;

  bool isTemporary() const { return false; }

  bool open(QString *errorString, const QString &fileName, const QString &realFileName);

public slots:
  void runThisFile();

signals:

public slots:
  
};


// *******

class JuliaEditorWidget : public TextEditor::BaseTextEditorWidget
{
  Q_OBJECT

public:
  JuliaEditorWidget(QWidget *parent);
  ~JuliaEditorWidget();

  virtual void setFontSettings(const TextEditor::FontSettings &);

  virtual void unCommentSelection();

protected:
  TextEditor::BaseTextEditor* createEditor();

};

}


#endif // JULIAEDITOR_H
