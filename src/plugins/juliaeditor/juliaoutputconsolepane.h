#ifndef JULIAOUTPUTCONSOLEPANE_H
#define JULIAOUTPUTCONSOLEPANE_H

#include "outputconsole.h"

#include <coreplugin/ioutputoutputpane.h>

#include <QSharedPointer>

QT_BEGIN_NAMESPACE
  class QToolButton;
QT_END_NAMESPACE

namespace JuliaPlugin {

class JuliaOutputConsolePane : public Core::IOutputOutputPane
{
  Q_OBJECT

public:
  explicit JuliaOutputConsolePane(QObject *parent = 0);
  ~JuliaOutputConsolePane();

public slots:
  virtual OutputConsole *outputWidget(QWidget *parent = NULL);
  virtual QString displayName() const;

  virtual void clearContents();
  virtual void visibilityChanged(bool visible);

  virtual bool canFocus() const;

  virtual bool canNavigate() const;
  virtual bool canNext() const;
  virtual bool canPrevious() const;

private:
  QSharedPointer<OutputConsole> console;
};

}

#endif // JULIAOUTPUTCONSOLEPANE_H
