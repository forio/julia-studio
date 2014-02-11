#include "juliaautocompleter.h"

using namespace JuliaPlugin;

JuliaCompleter::JuliaCompleter()
{}

JuliaCompleter::~JuliaCompleter()
{}

bool JuliaCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor,
                                                 const QString &textToInsert) const
{
    QChar ch;

/*
    if (! textToInsert.isEmpty())
        ch = textToInsert.at(0);

    if (! (MatchingText::shouldInsertMatchingText(cursor)
           || ch == QLatin1Char('\'')
           || ch == QLatin1Char('"')))
        return false;
    else if (isInComment(cursor))
        return false;
*/
    return true;
}
