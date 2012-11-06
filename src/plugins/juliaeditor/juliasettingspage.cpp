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
  settings->endGroup();
}

void JuliaSettings::FromSettings(QSettings *settings)
{
  settings->beginGroup(QLatin1String(Constants::JULIA_SETTINGS_GROUP));
  path_to_binaries = settings->value(QLatin1String("path_to_binaries"), QLatin1String("./julia")).toString();
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
    emit PathToBinariesChanged(path_to_binaries);
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
  settings->SetPathToBinaries( ui->path_to_binaries->text() );
}

void JuliaSettingsWidget::SetSettings(const JuliaSettings &settings)
{
  ui->path_to_binaries->setText( settings.GetPathToBinaries() );
}

void JuliaSettingsWidget::SelectJuliaBinaries()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Julia Folder"),
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
