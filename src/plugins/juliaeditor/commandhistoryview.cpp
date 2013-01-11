#include "commandhistoryview.h"
#include "console.h"

#include <QHeaderView>


using namespace JuliaPlugin;

CommandHistoryDelegate::CommandHistoryDelegate(QObject *parent)
  : QStyledItemDelegate(parent)
{
}



CommandHistoryView::CommandHistoryView(QWeakPointer<Console> console_handle, QWidget *parent)
  : QWidget(parent), console(console_handle)
{
  setWindowTitle( "Command History" );

  // layout -----
  grid_layout = new QGridLayout(this);
  grid_layout->setObjectName( QString::fromUtf8("grid_layout") );

  grid_layout->setSpacing(0);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  // -----

  // list customization -----
  list_view = new QTreeView(this);
  list_view->setObjectName( QString::fromUtf8("list_view") );
  list_view->setModel( console.data()->GetHistoryModel() );

  list_view->setItemDelegate( delegate = new CommandHistoryDelegate(this) );
  list_view->setIndentation(0);
  list_view->setFrameStyle(QFrame::NoFrame);
  list_view->setUniformRowHeights(true);
  list_view->setAttribute(Qt::WA_MacShowFocusRect, false);
  list_view->setTextElideMode(Qt::ElideMiddle);
  list_view->setSelectionMode(QAbstractItemView::SingleSelection);
  list_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  list_view->setContextMenuPolicy(Qt::CustomContextMenu);
  list_view->installEventFilter(this);

  list_view->header()->hide();
  list_view->header()->setStretchLastSection(false);
  list_view->header()->setResizeMode(0, QHeaderView::Stretch);
  //list_view->header()->setResizeMode(1, QHeaderView::Fixed);
  //list_view->header()->resizeSection(1, 16);

  list_view->viewport()->setAttribute(Qt::WA_Hover);
  list_view->viewport()->installEventFilter(this);
  // -----

  connect( console.data(), SIGNAL(SetCommandFromHistory(QModelIndex)), list_view, SLOT(setCurrentIndex(QModelIndex)) );
  connect( console.data(), SIGNAL(NewCommand(const ProjectExplorer::EvaluatorMessage&)), list_view, SLOT(clearSelection()) );
  connect( list_view, SIGNAL(clicked(QModelIndex)), console.data(), SLOT(SetCurrCommand(QModelIndex)) );

  grid_layout->addWidget(list_view, 0, 0, 1, 1);

}

CommandHistoryView::~CommandHistoryView()
{
  // everything is parented so we should be good...
}



CommandHistoryViewFactory::CommandHistoryViewFactory(QWeakPointer<Console> console)
  : console_handle(console)
{}

Core::NavigationView CommandHistoryViewFactory::createWidget()
{
  Core::NavigationView view;
  view.widget = new CommandHistoryView( console_handle );
  return view;
}



