#include "juliaconsolepane.h"

using namespace JuliaPlugin;

JuliaConsolePane::JuliaConsolePane(Console* console_, QObject *parent) :
  Core::IOutputPane(parent)
{
  SetConsole( console_ );
}

JuliaConsolePane::~JuliaConsolePane()
{}

QWidget *JuliaConsolePane::outputWidget(QWidget *parent)
{
  return console;
}

QList<QWidget *> JuliaConsolePane::toolBarWidgets() const
{
  return QList<QWidget*>();
}

QString JuliaConsolePane::displayName() const
{
  return tr("Julia Console");
}

int JuliaConsolePane::priorityInStatusBar() const
{
  return 19;
}

void JuliaConsolePane::clearContents()
{
  console->clear();
}

void JuliaConsolePane::visibilityChanged(bool visible)
{
  // that's nice
}

void JuliaConsolePane::setFocus()
{
  console->setFocus();
}

bool JuliaConsolePane::hasFocus() const
{
  return console->hasFocus();
}

bool JuliaConsolePane::canFocus() const
{
  return true;
}

bool JuliaConsolePane::canNavigate() const
{
  return false;
}

bool JuliaConsolePane::canNext() const
{
  return false;
}

bool JuliaConsolePane::canPrevious() const
{
  return false;
}

void JuliaConsolePane::goToNext()
{
  // no
}

void JuliaConsolePane::goToPrev()
{
}

