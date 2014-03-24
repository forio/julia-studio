#include "juliaoutputconsolepane.h"
#include "juliaeditor_constants.h"

#include <texteditor/texteditorsettings.h>

#include <QAction>
#include <QToolButton>

using namespace JuliaPlugin;

JuliaOutputConsolePane::JuliaOutputConsolePane(QObject *parent) :
  Core::IOutputOutputPane(parent), console(new OutputConsole())
{
  console->setLanguageSettingsId( QLatin1String( Constants::JULIA_SETTINGS_ID ) );
  TextEditor::TextEditorSettings::instance()->initializeEditor( console.data() );
  console->setLineNumbersVisible( false );
}

JuliaOutputConsolePane::~JuliaOutputConsolePane()
{
  //delete console;
}

OutputConsole *JuliaOutputConsolePane::outputWidget(QWidget *parent)
{
  return console.data();
}

QString JuliaOutputConsolePane::displayName() const
{
  return tr("Output Console");
}

void JuliaOutputConsolePane::clearContents()
{
  console->clear();
}

void JuliaOutputConsolePane::visibilityChanged(bool visible)
{
  // that's nice
}

bool JuliaOutputConsolePane::canFocus() const
{
  return false;
}

bool JuliaOutputConsolePane::canNavigate() const
{
  return false;
}

bool JuliaOutputConsolePane::canNext() const
{
  return false;
}

bool JuliaOutputConsolePane::canPrevious() const
{
  return false;
}
