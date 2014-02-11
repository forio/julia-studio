#ifndef JULIACOMPLETIONASSIST_H
#define JULIACOMPLETIONASSIST_H

#include "juliaeditor.h"
#include "projectexplorer/ievaluator.h"

#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/iassistprocessor.h>
#include <texteditor/codeassist/defaultassistinterface.h>
#include <texteditor/codeassist/ifunctionhintproposalmodel.h>
#include <texteditor/codeassist/ProposalCarrier.h>

namespace TextEditor {
class BasicProposalItem;
class BaseTextEditorWidget;
}

namespace JuliaPlugin
{
  class LocalTcpEvaluator;

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
    JuliaCompletionAssistProvider( LocalTcpEvaluator* p_evaluator );
    virtual bool isAsynchronous() const;
    virtual bool returnsCarrier() const;
    virtual bool supportsEditor(const Core::Id &editorId) const;
    virtual TextEditor::IAssistProcessor *createProcessor() const;
    virtual void setWidget( TextEditor::BaseTextEditorWidget* p_widget );

  private:
    LocalTcpEvaluator* m_evaluator;
    TextEditor::BaseTextEditorWidget* m_widget;
  };

  class JuliaCompletionAssistProcessor : public TextEditor::IAssistProcessor
  {
  public:
    JuliaCompletionAssistProcessor( LocalTcpEvaluator* p_evaluator, TextEditor::BaseTextEditorWidget* p_widget );
    virtual ~JuliaCompletionAssistProcessor();

    virtual TextEditor::IAssistProposal *perform(const TextEditor::IAssistInterface *interface);
    virtual TextEditor::ProposalCarrier *getCarrier( const TextEditor::IAssistInterface *interface );

  private:
    TextEditor::IAssistProposal *createContentProposal() const;
    bool acceptsIdleEditor() const;
    void addCompletion(const QString &text, const QIcon &icon, int order = 0);

    int m_startPosition;
    QScopedPointer<const JuliaCompletionAssistInterface> m_interface;
    QList<TextEditor::BasicProposalItem *> m_completions;

    QIcon m_attributeIcon;

    LocalTcpEvaluator* m_evaluator;
    TextEditor::BaseTextEditorWidget* m_widget;
  };

  class JuliaProposalCarrier : public TextEditor::ProposalCarrier
  {
    Q_OBJECT

  public:
    JuliaProposalCarrier();
    virtual ~JuliaProposalCarrier();

    virtual void start();
    void setPrefix( QString& p_prefix );
    void setEvaluator( LocalTcpEvaluator* p_evaluator );
    void setStartPosition( int p_startPosition );
    void setAttributeIcon( QIcon& p_icon );

public slots:
  void receivecompletions( const ProjectExplorer::EvaluatorMessage* msg );
  private:
    QString m_prefix;
    LocalTcpEvaluator* m_evaluator;
    QList<TextEditor::BasicProposalItem *> m_completions;
    TextEditor::IAssistProposal *createContentProposal() const;
    int m_startPosition;
    QIcon m_attributeIcon;
    void addCompletion(const QString &text, const QIcon &icon, int order = 0);
  };
}

#endif // JULIACOMPLETIONASSIST_H