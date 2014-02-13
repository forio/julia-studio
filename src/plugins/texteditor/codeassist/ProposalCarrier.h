#ifndef PROPOSALCARRIER_H
#define PROPOSALCARRIER_H

#include <QObject>

#include "iassistproposal.h"
#include "assistenums.h"

namespace TextEditor
{

class TEXTEDITOR_EXPORT ProposalCarrier : public QObject
{
  Q_OBJECT

public:
  ProposalCarrier();
  virtual ~ProposalCarrier();
  IAssistProposal *proposal() const;
  void setReason(AssistReason reason);
  AssistReason reason() const;
  virtual void start();

public slots:

signals:
  void ready();
protected:
  IAssistProposal *m_proposal;
  AssistReason m_reason;
};

}

#endif