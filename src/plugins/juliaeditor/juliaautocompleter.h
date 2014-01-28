#ifndef JULIAAUTOCOMPLETER_H
#define JULIAAUTOCOMPLETER_H

#include <texteditor/autocompleter.h>

namespace JuliaPlugin
{

  class JuliaCompleter : public TextEditor::AutoCompleter
  {
  public:
    JuliaCompleter();
    virtual ~JuliaCompleter();

    virtual bool contextAllowsAutoParentheses(const QTextCursor &cursor,
                                              const QString &textToInsert = QString()) const;
  };

}
#endif // JULIAAUTOCOMPLETER_H