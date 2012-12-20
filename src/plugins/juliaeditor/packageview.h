#ifndef PACKAGEVIEW_H
#define PACKAGEVIEW_H

#include <QAbstractItemView>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeView>
#include <QStyledItemDelegate>

#include <coreplugin/inavigationwidgetfactory.h>

namespace JuliaPlugin {

class PackageModel;

class PackageDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  PackageDelegate( QObject* parent ) : QStyledItemDelegate(parent)
  {}

};



class PackageView : public QWidget
{
  Q_OBJECT

public:
  explicit PackageView(QWidget *parent = 0);
  
private:
  QGridLayout* grid_layout;
  QTreeView* list_view;

  PackageModel* package_model;
  
};



class PackageViewFactory : public Core::INavigationWidgetFactory
{
  Q_OBJECT

public:
  PackageViewFactory() {};

  QString displayName() const  { return "Packages"; }
  Core::Id id() const  { return Core::Id( displayName() ); }

  int priority() const  { return 300; }

  Core::NavigationView createWidget();

};

}

#endif // PACKAGEVIEW_H
