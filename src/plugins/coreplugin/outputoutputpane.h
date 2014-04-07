#ifndef OUTPUTOUTPUTPANE_H
#define OUTPUTOUTPUTPANE_H

#include "core_global.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSplitter;
QT_END_NAMESPACE

namespace Core {

class IMode;

namespace Internal {
class OutputOutputPaneManager;
}
struct OutputOutputPanePlaceHolderPrivate;

class CORE_EXPORT OutputOutputPanePlaceHolder : public QWidget
{
    Q_OBJECT
    friend class Core::Internal::OutputOutputPaneManager; // needs to set m_visible and thus access m_current

public:
    explicit OutputOutputPanePlaceHolder(Core::IMode *mode, QSplitter *parent = 0);
    ~OutputOutputPanePlaceHolder();

    static OutputOutputPanePlaceHolder *getCurrent();
    static bool isCurrentVisible();

    void unmaximize();
    bool isMaximized() const;
    void ensureSizeHintAsMinimum();

private slots:
    void currentModeChanged(Core::IMode *);

private:
    bool canMaximizeOrMinimize() const;
    void maximizeOrMinimize(bool maximize);
    void setHalfMast();

    OutputOutputPanePlaceHolderPrivate *d;
};

} // namespace Core

#endif // OUTPUTOUTPUTPANE_H
