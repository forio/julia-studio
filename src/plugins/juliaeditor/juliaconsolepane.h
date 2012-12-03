#ifndef JULIACONSOLEPANE_H
#define JULIACONSOLEPANE_H

#include "console.h"

#include <coreplugin/ioutputpane.h>

#include <QSharedPointer>

QT_BEGIN_NAMESPACE
  class QToolButton;
QT_END_NAMESPACE

namespace JuliaPlugin {

class JuliaConsolePane : public Core::IOutputPane
{
  Q_OBJECT

public:
  explicit JuliaConsolePane(QObject *parent = 0);
  ~JuliaConsolePane();

public slots:
  QWeakPointer<Console> getConsoleHandle();

  virtual Console *outputWidget(QWidget *parent = NULL);
  virtual QList<QWidget *> toolBarWidgets() const;
  virtual QString displayName() const;

  // -1 don't show in statusBar
  // 100...0 show at front...end
  virtual int priorityInStatusBar() const;

  virtual void clearContents();
  virtual void visibilityChanged(bool visible);

  // This function is called to give the outputwindow focus
  virtual void setFocus();
  // Whether the outputpane has focus
  virtual bool hasFocus() const;
  // Whether the outputpane can be focused at the moment.
  // (E.g. the search result window does not want to be focused if the are no results.)
  virtual bool canFocus() const;

  virtual bool canNavigate() const;
  virtual bool canNext() const;
  virtual bool canPrevious() const;
  virtual void goToNext();
  virtual void goToPrev();

private:
  QSharedPointer<Console> console;

  QToolButton* reset_button;
};

}

#endif // JULIACONSOLEPANE_H
