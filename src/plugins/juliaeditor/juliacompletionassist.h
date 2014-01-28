#ifndef JULIACOMPLETIONASSIST_H
#define JULIACOMPLETIONASSIST_H

#include "juliaeditor.h"

#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>
#include <texteditor/codeassist/defaultassistinterface.h>
#include <texteditor/codeassist/ifunctionhintproposalmodel.h>

namespace TextEditor {
class BasicProposalItem;
}

namespace JuliaPlugin
{

  class JuliaCompletionAssistInterface : public TextEditor::DefaultAssistInterface
  {
  public:
    JuliaCompletionAssistInterface( QTextDocument *textDocument,
                                    int position, Core::IDocument *document,
                                    TextEditor::AssistReason reason );
  };

  class JuliaCompletionAssistProvider : public TextEditor::CompletionAssistProvider
  {
  public:
    virtual bool supportsEditor(const Core::Id &editorId) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;
  };

  class JuliaCompletionAssistProcessor : public TextEditor::IAssistProcessor
  {
  public:
    JuliaCompletionAssistProcessor();
    virtual ~JuliaCompletionAssistProcessor();

    virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *interface);

  private:
    TextEditor::IAssistProposal *createContentProposal() const;
    bool acceptsIdleEditor() const;

    int m_startPosition;
    QScopedPointer<const JuliaCompletionAssistInterface> m_interface;
    QList<TextEditor::BasicProposalItem *> m_completions;
  };
}

#endif // JULIACOMPLETIONASSIST_H