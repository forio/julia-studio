#include "juliacompletionassist.h"
#include "juliaeditor_constants.h"

#include <texteditor/codeassist/basicproposalitem.h>
#include <texteditor/codeassist/basicproposalitemlistmodel.h>
#include <texteditor/codeassist/genericproposal.h>

using namespace JuliaPlugin;
using namespace TextEditor;

namespace
{

bool isIdentifierChar(QChar ch)
{
    return ch.isLetterOrNumber() || ch == QLatin1Char('_')
        || ch.unicode() > 0x00A0 || ch == QLatin1Char('!');
}

bool isDelimiter(QChar ch)
{
    switch (ch.unicode()) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ')':
    case '?':
    case ':':
    case ';':
    case ',':
    case '+':
    case '-':
    case '*':
    case '/':
        return true;

    default:
        return false;
    }
}

bool checkStartOfIdentifier( const QString &word )
{
    if ( ! word.isEmpty() )
    {
      const QChar ch = word.at(0);
      if ( ch.isLetter() || ch == QLatin1Char('_') || ch.unicode() > 0x00A0 )
        return true;
    }

    return false;
}

bool isActivationChar(const QChar &ch)
{
    return ch == QLatin1Char('(') || ch == QLatin1Char('.') || ch == QLatin1Char(',');
}

} // Anonymous

bool JuliaCompletionAssistProvider::supportsEditor(const Core::Id &editorId) const
{
    return editorId == Constants::JULIAEDITOR_ID;
}

IAssistProcessor *JuliaCompletionAssistProvider::createProcessor() const
{
    return new JuliaCompletionAssistProcessor;
}

JuliaCompletionAssistProcessor::JuliaCompletionAssistProcessor()
{
}

JuliaCompletionAssistProcessor::~JuliaCompletionAssistProcessor()
{}

IAssistProposal *JuliaCompletionAssistProcessor::perform(const IAssistInterface *interface)
{
  m_interface.reset(static_cast<const JuliaCompletionAssistInterface *>(interface));

  if ( interface->reason() == IdleEditor && !acceptsIdleEditor() )
    return 0;

  int pos = m_interface->position() - 1;
  QChar ch = m_interface->characterAt(pos);
  while (ch.isLetterOrNumber() || ch == QLatin1Char('_'))
    ch = m_interface->characterAt(--pos);

  return createContentProposal();
}

IAssistProposal *JuliaCompletionAssistProcessor::createContentProposal() const
{
  IGenericProposalModel *model = new BasicProposalItemListModel(m_completions);
  IAssistProposal *proposal = new GenericProposal(m_startPosition, model);
  return proposal;
}

JuliaCompletionAssistInterface::JuliaCompletionAssistInterface( QTextDocument *textDocument,
                                                                int position,
                                                                Core::IDocument *document,
                                                                TextEditor::AssistReason reason )
    : DefaultAssistInterface(textDocument, position, document, reason)
{
}

bool JuliaCompletionAssistProcessor::acceptsIdleEditor() const
{
    const int cursorPosition = m_interface->position();
    const QChar ch = m_interface->characterAt(cursorPosition - 1);

    const QChar characterUnderCursor = m_interface->characterAt(cursorPosition);

    if (isIdentifierChar(ch) && (characterUnderCursor.isSpace() ||
                                 characterUnderCursor.isNull() ||
                                 isDelimiter(characterUnderCursor))) {
        int pos = m_interface->position() - 1;
        for (; pos != -1; --pos) {
            if (! isIdentifierChar(m_interface->characterAt(pos)))
                break;
        }
        ++pos;

        const QString word = m_interface->textAt(pos, cursorPosition - pos);
        if (word.length() > 2 && checkStartOfIdentifier(word)) {
            for (int i = 0; i < word.length(); ++i) {
                if (! isIdentifierChar(word.at(i)))
                    return false;
            }
            return true;
        }
    }

    return isActivationChar(ch);
}
