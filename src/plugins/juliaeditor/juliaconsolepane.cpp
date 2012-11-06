#include "juliaconsolepane.h"
#include "juliaeditor_constants.h"
#include <texteditor/texteditorsettings.h>

using namespace JuliaPlugin;

JuliaConsolePane::JuliaConsolePane(QObject *parent) :
  Core::IOutputPane(parent)
{
  console = new Console();

  console->setLanguageSettingsId( QLatin1String( Constants::JULIA_SETTINGS_ID ) );
  TextEditor::TextEditorSettings::instance()->initializeEditor( console );
}

JuliaConsolePane::~JuliaConsolePane()
{
  delete console;
}

Console *JuliaConsolePane::outputWidget(QWidget *parent)
{
  return console;
}

QList<QWidget *> JuliaConsolePane::toolBarWidgets() const
{
  return QList<QWidget*>();
}

QString JuliaConsolePane::displayName() const
{
  return tr("Console");
}

int JuliaConsolePane::priorityInStatusBar() const
{
  return 21;
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

