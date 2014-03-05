#include "juliacompletionassist.h"
#include "juliaeditor_constants.h"

#include "localtcpevaluator.h"
#include "juliamsg.h"

#include <extensionsystem/pluginmanager.h>

#include <texteditor/codeassist/basicproposalitem.h>
#include <texteditor/codeassist/basicproposalitemlistmodel.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/codeassist/ProposalCarrier.h>

using namespace JuliaPlugin;
using namespace TextEditor;

namespace
{

bool isIdentifierChar(QChar ch)
{
    return ch.unicode() != 0x2029 && ( ch.isLetterOrNumber() || ch == QLatin1Char('_')
        || ch.unicode() > 0x00A0 || ch == QLatin1Char('!') );
}

bool isDelimiter(QChar ch)
{
    switch (ch.unicode()) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ')':
    case '(':
    case '?':
    case ':':
    case ';':
    case ',':
    case '+':
    case '-':
    case '*':
    case '/':
    case '\\':
    case '%':
    case '^':
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
    //return ch == QLatin1Char('(') || ch == QLatin1Char('.') || ch == QLatin1Char(',');
    return ch == QLatin1Char('.');
}

} // Anonymous

JuliaCompletionAssistProvider::JuliaCompletionAssistProvider( LocalTcpEvaluator* p_evaluator )
{
  m_evaluator = p_evaluator;
}

bool JuliaCompletionAssistProvider::isAsynchronous() const
{
    return false;
}

bool JuliaCompletionAssistProvider::returnsCarrier() const
{
    return true;
}

bool JuliaCompletionAssistProvider::supportsEditor(const Core::Id &editorId) const
{
    return editorId == Constants::JULIAEDITOR_ID;
}

IAssistProcessor *JuliaCompletionAssistProvider::createProcessor() const
{
    return new JuliaCompletionAssistProcessor( m_evaluator, m_widget );
}

void JuliaCompletionAssistProvider::setWidget( BaseTextEditorWidget* p_widget )
{
  m_widget = p_widget;
}

JuliaCompletionAssistProcessor::JuliaCompletionAssistProcessor( LocalTcpEvaluator* p_evaluator, BaseTextEditorWidget* p_widget )
    : m_startPosition(0)
    , m_attributeIcon(":/juliaeditor/images/attribute.png")
    , m_evaluator( p_evaluator )
    , m_widget( p_widget )
{
}

JuliaCompletionAssistProcessor::~JuliaCompletionAssistProcessor()
{}

ProposalCarrier *JuliaCompletionAssistProcessor::getCarrier( const TextEditor::IAssistInterface *interface )
{
  m_interface.reset(static_cast<const JuliaCompletionAssistInterface *>(interface));

  if ( interface->reason() == IdleEditor && !acceptsIdleEditor() )
    return 0;

    int nulpos = m_widget->homePosition();

  int pos = m_interface->position() - 1;
  QChar ch = m_interface->characterAt(pos);
  while ( pos >= nulpos && ( isIdentifierChar( ch ) || isActivationChar( ch ) ) )
    ch = m_interface->characterAt(--pos);
  m_startPosition = pos + 1;

  QString prefix = m_interface->textAt( m_startPosition, m_interface->position() - m_startPosition );

  JuliaProposalCarrier* carrier = new JuliaProposalCarrier();
  carrier->setReason( interface->reason() );
  carrier->setPrefix( prefix );
  carrier->setEvaluator( m_evaluator );
  carrier->setStartPosition( m_startPosition );
  carrier->setAttributeIcon( m_attributeIcon );
  return carrier;
}

IAssistProposal *JuliaCompletionAssistProcessor::perform(const IAssistInterface *interface)
{
  QString prefix;

  m_interface.reset(static_cast<const JuliaCompletionAssistInterface *>(interface));

  if ( interface->reason() == IdleEditor && !acceptsIdleEditor() )
    return 0;

  int pos = m_interface->position() - 1;
  QChar ch = m_interface->characterAt(pos);
  while ( isIdentifierChar( ch ) )
    ch = m_interface->characterAt(--pos);
  m_startPosition = pos + 1;
  prefix = m_interface->textAt( m_startPosition, m_interface->position() - m_startPosition );
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

void JuliaCompletionAssistProcessor::addCompletion(const QString &text,
                                                  const QIcon &icon,
                                                  int order)
{
    BasicProposalItem *item = new BasicProposalItem;
    item->setText(text);
    item->setIcon(icon);
    item->setOrder(order);
    m_completions.append(item);
}

JuliaProposalCarrier::JuliaProposalCarrier()
{
}

JuliaProposalCarrier::~JuliaProposalCarrier()
{
}

void JuliaProposalCarrier::setPrefix( QString& p_prefix )
{
  m_prefix = p_prefix;
}

void JuliaProposalCarrier::setEvaluator( LocalTcpEvaluator* p_evaluator )
{
  m_evaluator = p_evaluator;
}

void JuliaProposalCarrier::setStartPosition( int p_startPosition )
{
  m_startPosition = p_startPosition;
}

void JuliaProposalCarrier::setAttributeIcon( QIcon& p_icon )
{
  m_attributeIcon = p_icon;
}

void JuliaProposalCarrier::addCompletion(const QString &text,
                                                  const QIcon &icon,
                                                  int order)
{
    BasicProposalItem *item = new BasicProposalItem;
    item->setText(text);
    item->setIcon(icon);
    item->setOrder(order);
    m_completions.append(item);
}

void JuliaProposalCarrier::start()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.typnam = QString( COMPLETE_name );
  msg.params.push_back( m_prefix );
  connect( m_evaluator, SIGNAL( output(const ProjectExplorer::EvaluatorMessage*) ), this, SLOT( receivecompletions( const ProjectExplorer::EvaluatorMessage* ) ) );
  m_evaluator->eval(msg);
}

void JuliaProposalCarrier::receivecompletions( const ProjectExplorer::EvaluatorMessage* msg )
{
  if ( msg->typnam != OUTPUT_COMPLETE_name )
    return;
  int i0 =  msg->params[0].toInt();
  int i1 =  msg->params[1].toInt();
  for ( int i = 2; i < msg->params.size(); i++ )
  {
    addCompletion( m_prefix.left( i0-1 ) + msg->params[i], m_attributeIcon );
  }
  m_proposal = createContentProposal();
  emit ready();
}

IAssistProposal *JuliaProposalCarrier::createContentProposal() const
{
  IGenericProposalModel *model = new BasicProposalItemListModel(m_completions);
  IAssistProposal *proposal = new GenericProposal(m_startPosition, model);
  return proposal;
}
