#ifndef PACKAGEVIEW_H
#define PACKAGEVIEW_H

#include <QAbstractItemView>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTreeView>
#else
#include <QtGui/QVBoxLayout>
#include <QtGui/QTreeView>
#endif
#include <QStyledItemDelegate>

#include <coreplugin/inavigationwidgetfactory.h>

namespace JuliaPlugin {

class PackageModel;

class PackageDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  PackageDelegate( PackageModel* model, QObject* parent ) : QStyledItemDelegate(parent), package_model(model)
  {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option_, const QModelIndex &index) const;

private:
  PackageModel* package_model;
};



class PackageView : public QWidget
{
  Q_OBJECT

public:
  explicit PackageView(QWidget *parent = 0);

  void SetPackageModel(PackageModel* model);
  
//private:
  QGridLayout* layout;
  QTreeView* list_view;

  PackageModel* package_model;
};



class PackageViewFactory : public Core::INavigationWidgetFactory
{
  Q_OBJECT

public:
  PackageViewFactory() {}

  QString displayName() const  { return "Packages"; }
  Core::Id id() const  { return Core::Id( displayName() ); }

  int priority() const  { return 300; }

  Core::NavigationView createWidget();

signals:
  void createdWidget(Core::NavigationView* widget);
};

}

#endif // PACKAGEVIEW_H
