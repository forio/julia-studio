#include "juliaindenter.h"
#include "texteditor/tabsettings.h"

#include <QTextDocument>

JuliaIndenter::JuliaIndenter()
{
}

JuliaIndenter::~JuliaIndenter()
{
}

bool JuliaIndenter::isElectricLine(const QString &line) const
{
  QString trs = line.trimmed();
  return trs.startsWith( QLatin1String( "begin" ) ) ||
         trs.startsWith( QLatin1String( "do" ) ) ||
         trs.startsWith( QLatin1String( "for" ) ) ||
         trs.startsWith( QLatin1String( "function" ) ) ||
         trs.startsWith( QLatin1String( "if" ) ) ||
         trs.startsWith( QLatin1String( "immutable" ) ) ||
         trs.startsWith( QLatin1String( "let" ) ) ||
         trs.startsWith( QLatin1String( "quote" ) ) ||
         trs.startsWith( QLatin1String( "try" ) ) ||
         trs.startsWith( QLatin1String( "type" ) ) ||
         trs.startsWith( QLatin1String( "while" ) );
}

void JuliaIndenter::indentBlock( QTextDocument *doc,
                                 const QTextBlock &block,
                                 const QChar &typedChar,
                                 const TextEditor::TabSettings &tabSettings )
{
  Q_UNUSED(doc)
  Q_UNUSED(typedChar)

  if ( block == doc->begin() )
    return;

  const QTextBlock previousBlock = block.previous();
  if ( previousBlock.isValid() )
  {
    QString previousLine = previousBlock.text();
    int indentation = tabSettings.indentationColumn(previousLine);
    if (isElectricLine(previousLine))
      indentation += tabSettings.m_tabSize;
    tabSettings.indentLine(block, indentation);
  }
  else
  {
    tabSettings.indentLine(block, 0);
  }
}

