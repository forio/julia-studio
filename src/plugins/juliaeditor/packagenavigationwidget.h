#ifndef PACKAGENAVIGATIONWIDGET_H
#define PACKAGENAVIGATIONWIDGET_H

#include <coreplugin/inavigationwidgetfactory.h>

#include <QWidget>
#include <QListView>

namespace JuliaPlugin {

class PackageNavigationModel : public QWidget
{

};

// -------

class PackageNavigationWidget : public QWidget
{
  Q_OBJECT

public:
  PackageNavigationWidget(QWidget* parent = 0);


private:
  QListView* package_list_view;

};

// -------

class PackageNavigationWidgetFactory : public Core::INavigationWidgetFactory
{
  Q_OBJECT
public:
  explicit PackageNavigationWidgetFactory(QWidget *parent = 0);

  QString displayName() const  { return tr("Packages"); }
  Core::Id id() const          { return "Packages"; }
  int priority() const         { return 500; }

  Core::NavigationView createWidget();
  
};

}

#endif // PACKAGENAVIGATIONWIDGET_H
