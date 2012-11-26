#include "packagenavigationwidget.h"

using namespace JuliaPlugin;

PackageNavigationWidget::PackageNavigationWidget(QWidget *parent)
{
}

// -------

PackageNavigationWidgetFactory::PackageNavigationWidgetFactory(QWidget *parent) :
  Core::INavigationWidgetFactory()
{}

Core::NavigationView PackageNavigationWidgetFactory::createWidget()
{
  Core::NavigationView nav_view;
  nav_view.widget = new PackageNavigationWidget;

  return nav_view;
}



