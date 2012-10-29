#include "juliaeditor.h"
#include "juliaeditorconstants.h"

#include <coreplugin/mimedatabase.h>
#include <coreplugin/icore.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/generichighlighter/highlighter.h>
#include <texteditor/generichighlighter/manager.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>

#include <texteditor/generichighlighter/highlightdefinition.h>

#include <QDebug>

namespace JuliaPlugin {

// JuliaEditor *******

JuliaEditor::JuliaEditor(TextEditor::BaseTextEditorWidget *widget)
  : TextEditor::BaseTextEditor( widget )
{
  m_context.add(Constants::JULIAEDITOR);
  //m_context.add(ProjectExplorer::Constants::LANG_JULIA);
  //m_context.add(TextEditor::Constants::JULIA_TEXTEDITOR);
}

Core::Id JuliaEditor::id() const
{
  return Constants::JULIAEDITOR_ID;
}

bool JuliaEditor::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
  bool success = TextEditor::BaseTextEditor::open( errorString, fileName, realFileName );
  editorWidget()->setMimeType( Core::ICore::mimeDatabase()->findByFile( QFileInfo( fileName ) ).type() );
  return success;
}


// JuliaEditorWidget *******

JuliaEditorWidget::JuliaEditorWidget(QWidget *parent)
  : TextEditor::BaseTextEditorWidget( parent )
{
  TextEditor::Internal::Highlighter* highlighter = new TextEditor::Internal::Highlighter();
  baseTextDocument()->setSyntaxHighlighter( highlighter );

  const QSharedPointer<TextEditor::Internal::HighlightDefinition> &definition
      = TextEditor::Internal::Manager::instance()->definition(":/juliaeditor/julia.xml");
  if (!definition.isNull() && definition->isValid()) {
      highlighter->setDefaultContext(definition->initialContext());

      //m_commentDefinition.setAfterWhiteSpaces(definition->isCommentAfterWhiteSpaces());
      //m_commentDefinition.setSingleLine(definition->singleLineComment());
      //m_commentDefinition.setMultiLineStart(definition->multiLineCommentStart());
      //m_commentDefinition.setMultiLineEnd(definition->multiLineCommentEnd());

      setCodeFoldingSupported(true);
  }
}

JuliaEditorWidget::~JuliaEditorWidget()
{
}

void JuliaEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
  using namespace TextEditor;
  using namespace TextEditor::Internal;

  BaseTextEditorWidget::setFontSettings(fs);

  if (baseTextDocument()->syntaxHighlighter()) {
      Highlighter *highlighter = static_cast<Highlighter *>(baseTextDocument()->syntaxHighlighter());

      highlighter->configureFormat(Highlighter::VisualWhitespace, fs.toTextCharFormat(C_VISUAL_WHITESPACE));
      highlighter->configureFormat(Highlighter::Keyword, fs.toTextCharFormat(C_KEYWORD));
      highlighter->configureFormat(Highlighter::DataType, fs.toTextCharFormat(C_TYPE));
      highlighter->configureFormat(Highlighter::Comment, fs.toTextCharFormat(C_COMMENT));
      // Using C_NUMBER for all kinds of numbers.
      highlighter->configureFormat(Highlighter::Decimal, fs.toTextCharFormat(C_NUMBER));
      highlighter->configureFormat(Highlighter::BaseN, fs.toTextCharFormat(C_NUMBER));
      highlighter->configureFormat(Highlighter::Float, fs.toTextCharFormat(C_NUMBER));
      // Using C_STRING for strings and chars.
      highlighter->configureFormat(Highlighter::Char, fs.toTextCharFormat(C_STRING));
      highlighter->configureFormat(Highlighter::String, fs.toTextCharFormat(C_STRING));

      highlighter->rehighlight();
  }
}

TextEditor::BaseTextEditor *JuliaEditorWidget::createEditor()
{
  return new JuliaEditor(this);
}

}
