#ifndef JULIAINDENTOR_H
#define JULIAINDENTOR_H

#include "texteditor/indenter.h"

class JuliaIndenter : public TextEditor::Indenter
{
public:
  JuliaIndenter();
  virtual ~JuliaIndenter();

  virtual void indentBlock( QTextDocument *doc,
                            const QTextBlock &block,
                            const QChar &typedChar,
                            const TextEditor::TabSettings &tabSettings );
protected:
  bool isElectricLine(const QString &line) const;
};

#endif