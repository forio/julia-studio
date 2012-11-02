#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <QObject>
#include "projectexplorer_export.h"

QT_BEGIN_NAMESPACE
  class QFileInfo;
  class QString;
QT_END_NAMESPACE

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT IEvaluator : public QObject
{
  Q_OBJECT

public:
  explicit IEvaluator(QObject *parent = 0) : QObject(parent) {}
  virtual ~IEvaluator()  {}

public slots:
  virtual void eval(const QFileInfo* script) = 0;
  virtual void eval(const QString& expression) = 0;

  virtual void reset() = 0;

  virtual bool canRun(const QString& extension) = 0;
  virtual bool isRunning() = 0;
};

}

#endif // IEXECUTOR_H
