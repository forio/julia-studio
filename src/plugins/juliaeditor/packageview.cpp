#include "packageview.h"
#include "packagemodel.h"

#include <QHeaderView>
#include <QPainter>

using namespace JuliaPlugin;

void PackageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option_, const QModelIndex &index) const
{
  painter->save();

  QStyleOptionViewItem option = option_;
  PackageData data = package_model->data(index, Qt::UserRole).value<PackageData>();
  if (data.installed)
    option.font.setBold(true);

  QStyledItemDelegate::paint(painter, option, index);

  painter->restore();
}


PackageView::PackageView(QWidget *parent) :
  QWidget(parent)
{
  setWindowTitle( "Packages" );

  // layout -----
  layout = new QVBoxLayout(this);
  layout->setObjectName( QString::fromUtf8("package_grid_layout") );

  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  // -----

  // list customization -----
  list_view = new QTreeView(this);
  list_view->setObjectName( QString::fromUtf8("package_list_view") );

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
  //list_view->header()->setResizeMode(1, QHeaderView::Stretch);
  //list_view->header()->resizeSection(1, 16);

  list_view->viewport()->setAttribute(Qt::WA_Hover);
  list_view->viewport()->installEventFilter(this);
  list_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  // -----

  layout->addWidget(list_view, 1);
}

void PackageView::SetPackageModel(PackageModel *model)
{
  list_view->setModel( package_model = model );
  list_view->setItemDelegate( new PackageDelegate(package_model, this) );
  //connect( list_view, SIGNAL(doubleClicked(QModelIndex)), package_model, SLOT(AddPackage(const QModelIndex&)) );
}


Core::NavigationView PackageViewFactory::createWidget()
{
  Core::NavigationView view;
  PackageView* package_view = new PackageView;
  view.widget = package_view;

  emit createdWidget(package_view);
  return view;
}


