#include "outputoutputpane.h"
#include "outputoutputpanemanager.h"

#include "coreconstants.h"
#include "icore.h"
#include "ioutputoutputpane.h"
#include "modemanager.h"

#include <QSplitter>
#include <QVBoxLayout>

namespace Core {

struct OutputOutputPanePlaceHolderPrivate {
    explicit OutputOutputPanePlaceHolderPrivate(Core::IMode *mode, QSplitter *parent);

    Core::IMode *m_mode;
    QSplitter *m_splitter;
    int m_lastNonMaxSize;
    static OutputOutputPanePlaceHolder* m_current;
};

OutputOutputPanePlaceHolderPrivate::OutputOutputPanePlaceHolderPrivate(Core::IMode *mode, QSplitter *parent) :
    m_mode(mode), m_splitter(parent), m_lastNonMaxSize(0)
{
}

OutputOutputPanePlaceHolder *OutputOutputPanePlaceHolderPrivate::m_current = 0;

OutputOutputPanePlaceHolder::OutputOutputPanePlaceHolder(Core::IMode *mode, QSplitter* parent)
   : QWidget(parent), d(new OutputOutputPanePlaceHolderPrivate(mode, parent))
{
    setVisible(false);
    setLayout(new QVBoxLayout);
    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Preferred);
    sp.setVerticalPolicy(QSizePolicy::Preferred);
    sp.setHorizontalStretch(0);
    setSizePolicy(sp);
    layout()->setMargin(0);
    connect(Core::ModeManager::instance(), SIGNAL(currentModeChanged(Core::IMode*)),
            this, SLOT(currentModeChanged(Core::IMode*)));
}

OutputOutputPanePlaceHolder::~OutputOutputPanePlaceHolder()
{
    if (d->m_current == this) {
        if (Internal::OutputOutputPaneManager *om = Internal::OutputOutputPaneManager::instance()) {
            om->setParent(0);
            om->hide();
        }
    }
    delete d;
}

void OutputOutputPanePlaceHolder::currentModeChanged(Core::IMode *mode)
{
    if (d->m_current == this) {
        d->m_current = 0;
        Internal::OutputOutputPaneManager *om = Internal::OutputOutputPaneManager::instance();
        om->setParent(0);
        om->hide();
        //om->updateStatusButtons(false);
    }
    if (d->m_mode == mode) {
        d->m_current = this;
        Internal::OutputOutputPaneManager *om = Internal::OutputOutputPaneManager::instance();
        layout()->addWidget(om);
        om->show();
        //om->updateStatusButtons(isVisible());
    }
}

void OutputOutputPanePlaceHolder::maximizeOrMinimize(bool maximize)
{
    if (!d->m_splitter)
        return;
    int idx = d->m_splitter->indexOf(this);
    if (idx < 0)
        return;

    QList<int> sizes = d->m_splitter->sizes();

    if (maximize) {
        d->m_lastNonMaxSize = sizes[idx];
        int sum = 0;
        foreach(int s, sizes)
            sum += s;
        for (int i = 0; i < sizes.count(); ++i) {
            sizes[i] = 32;
        }
        sizes[idx] = sum - (sizes.count()-1) * 32;
    } else {
        int target = d->m_lastNonMaxSize > 0 ? d->m_lastNonMaxSize : sizeHint().height();
        int space = sizes[idx] - target;
        if (space > 0) {
            for (int i = 0; i < sizes.count(); ++i) {
                sizes[i] += space / (sizes.count()-1);
            }
            sizes[idx] = target;
        }
    }

    d->m_splitter->setSizes(sizes);

}

void OutputOutputPanePlaceHolder::setHalfMast()
{
    if (!d || !d->m_splitter)
        return;

    int idx = d->m_splitter->indexOf(this);
    if (idx < 0)
        return;

    QList<int> sizes = d->m_splitter->sizes();

    int sum = 0;
    foreach(int s, sizes)
        sum += s;

    sizes[idx] = sum - (sizes.count()-1) * 100;
    d->m_lastNonMaxSize = sizes[idx];
    d->m_splitter->setSizes(sizes);
}

bool OutputOutputPanePlaceHolder::isMaximized() const
{
    return Internal::OutputOutputPaneManager::instance()->isMaximized();
}

void OutputOutputPanePlaceHolder::ensureSizeHintAsMinimum()
{
    if (!d->m_splitter)
        return;
    int idx = d->m_splitter->indexOf(this);
    if (idx < 0)
        return;

    QList<int> sizes = d->m_splitter->sizes();
    Internal::OutputOutputPaneManager *om = Internal::OutputOutputPaneManager::instance();
    int minimum = (d->m_splitter->orientation() == Qt::Vertical
                   ? om->sizeHint().height() : om->sizeHint().width());
    int difference = minimum - sizes.at(idx);
    if (difference <= 0) // is already larger
        return;
    for (int i = 0; i < sizes.count(); ++i) {
        sizes[i] += difference / (sizes.count()-1);
    }
    sizes[idx] = minimum;
    d->m_splitter->setSizes(sizes);
}

void OutputOutputPanePlaceHolder::unmaximize()
{
    if (Internal::OutputOutputPaneManager::instance()->isMaximized())
        Internal::OutputOutputPaneManager::instance()->slotMinMax();
}

OutputOutputPanePlaceHolder *OutputOutputPanePlaceHolder::getCurrent()
{
    return OutputOutputPanePlaceHolderPrivate::m_current;
}

bool OutputOutputPanePlaceHolder::canMaximizeOrMinimize() const
{
    return d->m_splitter != 0;
}

bool OutputOutputPanePlaceHolder::isCurrentVisible()
{
    return OutputOutputPanePlaceHolderPrivate::m_current && OutputOutputPanePlaceHolderPrivate::m_current->isVisible();
}

} // namespace Core


