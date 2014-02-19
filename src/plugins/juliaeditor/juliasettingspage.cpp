#include "juliasettingspage.h"
#include "juliaeditor_constants.h"
#include "ui_juliasettingspage.h"
#include "singleton.h"

#include <coreplugin/icore.h>

#include <QSettings>
#include <QFileDialog>

using namespace JuliaPlugin;

// JuliaSettings *******
JuliaSettings::JuliaSettings(QObject* parent)
  : QObject(parent)
{}

void JuliaSettings::ToSettings(QSettings *settings) const
{
  settings->beginGroup( QLatin1String(Constants::JULIA_SETTINGS_GROUP) );
  settings->setValue( QLatin1String("path_to_binaries"), path_to_binaries );
  settings->setValue( QLatin1String("executable_name"), executable_name );
  settings->endGroup();
}

void JuliaSettings::FromSettings(QSettings *settings)
{
  settings->beginGroup(QLatin1String(Constants::JULIA_SETTINGS_GROUP));

  QString default_path = QApplication::applicationDirPath();
  QString default_name;

#if defined(Q_OS_WIN)
  default_name = QString( "julia-basic.exe" );
#else
  default_name = QString( "julia-basic" );
#endif

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
  QDir dir( default_path );
  dir.cdUp();
  dir.cd( "julia/bin" );
  default_path = dir.absolutePath();
#else
  QDir dir( default_path );
  dir.cdUp();
  dir.cdUp();
  dir.cd( "julia/bin" );
  default_path = dir.absolutePath();
#endif

  path_to_binaries = settings->value(QLatin1String("path_to_binaries"), default_path.toLatin1()).toString();
  executable_name = settings->value(QLatin1String("executable_name"), default_name.toLatin1()).toString();
  settings->endGroup();
}

const QString &JuliaSettings::GetPathToBinaries() const
{
  return path_to_binaries;
}

void JuliaSettings::SetPathToBinaries(const QString &path)
{
  if (path_to_binaries != path) {
    path_to_binaries = path;
    emit PathToBinariesChanged(path_to_binaries, executable_name);
  }
}

const QString &JuliaSettings::GetExecutableName() const
{
  return executable_name;
}

void JuliaSettings::SetExecutableName(const QString &name)
{
  if (executable_name != name) {
    executable_name = name;
    emit PathToBinariesChanged(path_to_binaries, executable_name);
  }
}

// JuliaSettingsWidget *******
JuliaSettingsWidget::JuliaSettingsWidget(QWidget *parent)
  : QWidget(parent),
  ui(new Ui::JuliaSettingsWidget)
{
  ui->setupUi(this);

  connect( ui->select_binaries_path_button, SIGNAL(clicked()), SLOT(SelectJuliaBinaries()) );
}

JuliaSettingsWidget::~JuliaSettingsWidget()
{
  delete ui;
}

void JuliaSettingsWidget::ApplySettings() const
{
  JuliaSettings* settings = Singleton<JuliaSettings>::GetInstance();
  QString fullPath = ui->path_to_binaries->text();
  int slpos = fullPath.lastIndexOf( "/" );
  QString name = fullPath.section('/', -1);
  QString path = fullPath.left( slpos );

  settings->SetPathToBinaries( path );
  settings->SetExecutableName( name );
}

void JuliaSettingsWidget::SetSettings(const JuliaSettings &settings)
{
  ui->path_to_binaries->setText( settings.GetPathToBinaries() + "/" + settings.GetExecutableName() );
}

void JuliaSettingsWidget::SelectJuliaBinaries()
{
  //QString dir = QFileDialog::getExistingDirectory(this, tr("Select Julia Folder"),
  //                                                Singleton<JuliaSettings>::GetInstance()->GetPathToBinaries());
  QString dir = QFileDialog::getOpenFileName(this, tr("Select Julia Executable"),
                                                  Singleton<JuliaSettings>::GetInstance()->GetPathToBinaries());

  if ( dir.length() ) {
    ui->path_to_binaries->setText(dir);
  }
}


// JuliaSettingsPage *******
JuliaSettingsPage::JuliaSettingsPage(QObject *parent)
  : widget(NULL)
{
  setId(QLatin1String(Constants::JULIA_SETTINGS_ID));
  setDisplayName(QCoreApplication::translate("Julia", Constants::JULIA_SETTINGS_NAME));
  setCategory(QLatin1String(Constants::JULIA_SETTINGS_CATEGORY));
  setDisplayCategory(QCoreApplication::translate("Julia", Constants::JULIA_SETTINGS_TR_CATEGORY));
  setCategoryIcon(QLatin1String(Constants::SETTINGS_CATEGORY_JULIA_ICON));
}

QWidget *JuliaSettingsPage::createPage(QWidget *parent)
{
  widget = new JuliaSettingsWidget( parent );
  widget->SetSettings( *Singleton<JuliaSettings>::GetInstance() );
  return widget;
}

void JuliaSettingsPage::apply()
{
  if (widget) {
    widget->ApplySettings();
    Singleton<JuliaSettings>::GetInstance()->ToSettings( Core::ICore::settings() );
  }
}

void JuliaSettingsPage::finish()
{
}
