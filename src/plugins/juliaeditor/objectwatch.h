#ifndef OBJECTWATCH_H
#define OBJECTWATCH_H

#include <coreplugin/inavigationwidgetfactory.h>

#include <QWidget>
#include <QStyledItemDelegate>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QComboBox>
#else
#include <QtGui/QGridLayout>
#include <QtGui/QTreeView>
#include <QtGui/QComboBox>
#endif

#include "objectwatchmodel.h"

namespace ProjectExplorer
{
  class EvaluatorMessage;
}

namespace JuliaPlugin
{

class LocalTcpEvaluator;

class ObjectWatch : public QWidget
{
  Q_OBJECT

public:
  explicit ObjectWatch( LocalTcpEvaluator* pevaluator, bool pValues, QWidget *parent = 0 );

private slots:
  void EvaluatorOutput( const ProjectExplorer::EvaluatorMessage* msg );

private:
  ObjectWatchModel* model;

  QGridLayout *grid_layout;
  QTreeView *list_view;

  LocalTcpEvaluator* evaluator;
  void GetObjects();

  friend class ObjectWatchFactory;

  bool mValues;
};

class ObjectWatchFactory: public Core::INavigationWidgetFactory
{
  Q_OBJECT

public:
  ObjectWatchFactory( LocalTcpEvaluator* pevaluator, bool pValues );

  QString displayName() const  {
   return mValues ? "Objects" : "Functions"; }
  Core::Id id() const  { return Core::Id( displayName() ); }

  int priority() const  { return 300; }

  Core::NavigationView createWidget();

signals:
  void createdWidget(Core::NavigationView* widget);

private:
  LocalTcpEvaluator* evaluator;
  bool mValues;
};

} //namespace JuliaPlugin

#endif