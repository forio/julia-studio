#ifndef COMMANDHISTORYVIEW_H
#define COMMANDHISTORYVIEW_H

#include "console.h"

#include <coreplugin/inavigationwidgetfactory.h>

#include <QWidget>
#include <QStyledItemDelegate>
#include <QtGui/QGridLayout>
#include <QtGui/QTreeView>
#include <QWeakPointer>


namespace JuliaPlugin {

class CommandHistoryDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  explicit CommandHistoryDelegate(QObject* parent = 0);
};



class CommandHistoryView : public QWidget
{
  Q_OBJECT

public:
  explicit CommandHistoryView(QWeakPointer<Console> console_handle, QWidget *parent = 0);
  virtual ~CommandHistoryView();

private:
  QGridLayout *grid_layout;
  QTreeView *list_view;
  CommandHistoryDelegate* delegate;

  QWeakPointer<Console> console;

};



class CommandHistoryViewFactory : public Core::INavigationWidgetFactory
{
  Q_OBJECT

public:
  CommandHistoryViewFactory( QWeakPointer<Console> console );

  QString displayName() const  { return "Command History"; }
  Core::Id id() const  { return Core::Id( displayName() ); }

  int priority() const  { return 175; }

  Core::NavigationView createWidget();
  QWeakPointer<Console> console_handle;
};

}

#endif // COMMANDHISTORYVIEW_H
