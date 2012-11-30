#include "juliaconsolepane.h"
#include "juliaeditor_constants.h"

#include <texteditor/texteditorsettings.h>

#include <QAction>
#include <QToolButton>

using namespace JuliaPlugin;

JuliaConsolePane::JuliaConsolePane(QObject *parent) :
  Core::IOutputPane(parent), console(new Console())
{
  console->setLanguageSettingsId( QLatin1String( Constants::JULIA_SETTINGS_ID ) );
  TextEditor::TextEditorSettings::instance()->initializeEditor( console.data() );

  reset_button = new QToolButton(console.data());
  reset_button->setAutoRaise(true);

  QAction* reset_action = new QAction(reset_button);
  //reset_action->setDefaultValue(true);
  //reset_action->setSettingsKey(QLatin1String(CONSOLE), QLatin1String(SHOW_LOG));
  reset_action->setToolTip(tr("Reset Julia environment"));
  //reset_action->setCheckable(true);
  //reset_action->setIcon(QIcon(QLatin1String(":/qmljstools/images/log.png")));
  reset_action->setText("Reset");
  connect(reset_action, SIGNAL(triggered()), console.data(), SLOT(Reset()));
  reset_button->setDefaultAction(reset_action);
}

JuliaConsolePane::~JuliaConsolePane()
{
  //delete console;
}

QWeakPointer<Console> JuliaConsolePane::getConsoleHandle()
{
  return console.toWeakRef();
}

Console *JuliaConsolePane::outputWidget(QWidget *parent)
{
  return console.data();
}

QList<QWidget *> JuliaConsolePane::toolBarWidgets() const
{
  return QList<QWidget*>() << reset_button;
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
  console->insertPlainText( "julia> " );  // TODO: this is a horrible hack
  console->BeginCommand();
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

