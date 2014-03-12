#include "objectwatch.h"
#include "objectwatchmodel.h"
#include "localtcpevaluator.h"
#include "juliamsg.h"
#include "projectexplorer/ievaluator.h"

#include <QtWidgets/QLabel>

using namespace JuliaPlugin;

ObjectWatch::ObjectWatch( LocalTcpEvaluator* pevaluator, bool pValues, QWidget *parent ) :
  QWidget(parent),
  evaluator( pevaluator ),
  mValues( pValues )
{
  setWindowTitle( mValues ? "Objects" : "Functions" );

  // layout -----
  grid_layout = new QGridLayout(this);
  grid_layout->setObjectName( QString::fromUtf8("grid_layout") );

  // -----

  // list customization -----
  list_view = new QTreeView(this);
  model = new ObjectWatchModel( mValues );
  list_view->setModel( model );

  grid_layout->setSpacing(0);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  grid_layout->addWidget(list_view, 0, 0, 1, 1);

  GetObjects();
  connect( evaluator, SIGNAL(output(const ProjectExplorer::EvaluatorMessage*)), SLOT(EvaluatorOutput(const ProjectExplorer::EvaluatorMessage*)));
}

void ObjectWatch::EvaluatorOutput(const ProjectExplorer::EvaluatorMessage *msg)
{
  if ( msg->typnam == OUTPUT_WATCH_name && 
       ( ( msg->params[0] == "Objects" && mValues ) ||
         ( msg->params[0] == "Functions" && !mValues ) ) )
  {
    int sz = ( msg->params.size() - 1 ) / 4;
    model->clear();
    int i = 1;
    for ( int o = 0; o < sz; o++ )
    {
      model->addobj( msg->params[i], msg->params[i+1], msg->params[i+2], msg->params[i+3] );
      i += 4;
    }
    model->display();
  }
  else if ( msg->typnam == OUTPUT_EVAL_name || msg->typnam == OUTPUT_PACKAGE_name )
    GetObjects();
}

ObjectWatchFactory::ObjectWatchFactory( LocalTcpEvaluator* pevaluator, bool pValues ) :
  evaluator( pevaluator ),
  mValues( pValues )
{
}

Core::NavigationView ObjectWatchFactory::createWidget()
{
  Core::NavigationView view;
  ObjectWatch* object_watch = new ObjectWatch( evaluator, mValues );
  view.widget = object_watch;

  //emit createdWidget(&view);
  return view;
}

void ObjectWatch::GetObjects()
{
  ProjectExplorer::EvaluatorMessage msg;
  msg.typnam = QString( WATCH_name );
  msg.params.push_back( mValues ? "Objects" : "Functions" );
  evaluator->eval(msg);
}
