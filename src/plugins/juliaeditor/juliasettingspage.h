#ifndef JULIASETTINGSPAGE_H
#define JULIASETTINGSPAGE_H

#include <coreplugin/dialogs/ioptionspage.h>

#include <QWidget>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace Ui {
  class JuliaSettingsWidget;
}

namespace JuliaPlugin {

class JuliaSettings : public QObject
{
  Q_OBJECT
public:
  explicit JuliaSettings(QObject* parent = NULL);

  void ToSettings(QSettings* settings) const;
  void FromSettings(QSettings* settings );

  const QString& GetPathToBinaries() const;
  void SetPathToBinaries( const QString& path );

signals:
  void PathToBinariesChanged( const QString& new_path );

private:
  QString path_to_binaries;
};


class JuliaSettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit JuliaSettingsWidget(QWidget *parent = 0);
  ~JuliaSettingsWidget();

  void ApplySettings() const;
  void SetSettings(const JuliaSettings& settings);

private slots:
  void SelectJuliaBinaries();
  
private:
  Ui::JuliaSettingsWidget *ui;
};


class JuliaSettingsPage : public Core::IOptionsPage
{
  Q_OBJECT

public:
  explicit JuliaSettingsPage(QObject* parent = 0);

  virtual QWidget *createPage(QWidget *parent);
  virtual void apply();
  virtual void finish();

private:
  JuliaSettingsWidget* widget;
};

}

#endif // JULIASETTINGSPAGE_H
