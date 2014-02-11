#include "ProposalCarrier.h"

using namespace TextEditor;

ProposalCarrier::ProposalCarrier()
  : m_proposal(0)
{}

ProposalCarrier::~ProposalCarrier()
{
  //if ( m_proposal )
  //  delete m_proposal;
}

IAssistProposal *ProposalCarrier::proposal() const
{
  return m_proposal;
}

void ProposalCarrier::setReason( AssistReason reason )
{
  m_reason = reason;
}

AssistReason ProposalCarrier::reason() const
{
  return m_reason;
}

void ProposalCarrier::start()
{
}
