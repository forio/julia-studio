#ifndef IOUTPUTOUTPUTPANE_H
#define IOUTPUTOUTPUTPANE_H

#include "core_global.h"

#include <QObject>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace Core {

class CORE_EXPORT IOutputOutputPane : public QObject
{
    Q_OBJECT

public:
    enum Flag { NoModeSwitch = 0, ModeSwitch = 1, WithFocus = 2, EnsureSizeHint = 4};
    Q_DECLARE_FLAGS(Flags, Flag)

    IOutputOutputPane(QObject *parent = 0) : QObject(parent) {}

    virtual QWidget *outputWidget(QWidget *parent) = 0;
    virtual QString displayName() const = 0;

    virtual int startingFlags() const { return NoModeSwitch; }

    virtual void clearContents() = 0;
    virtual void visibilityChanged(bool visible) = 0;

    virtual bool canFocus() const = 0;

    virtual bool canNavigate() const = 0;
    virtual bool canNext() const = 0;
    virtual bool canPrevious() const = 0;
};

} // namespace Core


#endif // IOUTPUTOUTPUTPANE_H
