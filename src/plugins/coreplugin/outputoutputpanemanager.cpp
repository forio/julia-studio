#include "outputoutputpanemanager.h"
#include "outputoutputpane.h"
#include "coreconstants.h"
#include "findplaceholder.h"

#include "coreconstants.h"
#include "icore.h"
#include "ioutputoutputpane.h"
#include "mainwindow.h"
#include "modemanager.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/findplaceholder.h>
#include <coreplugin/editormanager/ieditor.h>

#include <extensionsystem/pluginmanager.h>

#include <utils/hostosinfo.h>
#include <utils/styledbar.h>
#include <utils/qtcassert.h>

#include <QDebug>

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QSplitter>
#include <QStyle>
#include <QStackedWidget>
#include <QToolButton>
#include <QTimeLine>
#include <QLabel>

namespace Core {
namespace Internal {

static char outputPaneSettingsKeyC[] = "OutputPaneVisibility";
static char outputPaneIdKeyC[] = "id";
static char outputPaneVisibleKeyC[] = "visible";

////
// OutputOutputPaneManager
////

static OutputOutputPaneManager *m_instance = 0;

void OutputOutputPaneManager::create()
{
   m_instance = new OutputOutputPaneManager;
}

void OutputOutputPaneManager::destroy()
{
    delete m_instance;
    m_instance = 0;
}

OutputOutputPaneManager *OutputOutputPaneManager::instance()
{
    return m_instance;
}

OutputOutputPaneManager::OutputOutputPaneManager(QWidget *parent) :
    QWidget(parent),
    m_titleLabel(new QLabel),
    m_closeButton(new QToolButton),
    m_minMaxAction(0),
    m_minMaxButton(new QToolButton),
    m_outputWidgetPane(new QStackedWidget),
    m_opToolBarWidgets(new QStackedWidget),
    m_minimizeIcon(QLatin1String(":/core/images/arrowdown.png")),
    m_maximizeIcon(QLatin1String(":/core/images/arrowup.png")),
    m_maximised(false)
{
    setWindowTitle(tr("Output"));

    m_titleLabel->setContentsMargins(5, 0, 5, 0);

    m_clearAction = new QAction(this);
    m_clearAction->setIcon(QIcon(QLatin1String(Constants::ICON_CLEAN_PANE)));
    m_clearAction->setText(tr("Clear"));
    connect(m_clearAction, SIGNAL(triggered()), this, SLOT(clearPage()));

    m_minMaxAction = new QAction(this);
    m_minMaxAction->setIcon(m_maximizeIcon);
    m_minMaxAction->setText(tr("Maximize Output Pane"));

    m_closeButton->setIcon(QIcon(QLatin1String(Constants::ICON_CLOSE_DOCUMENT)));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(slotHide()));

    connect(ICore::instance(), SIGNAL(saveSettingsRequested()), this, SLOT(saveSettings()));

    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    m_toolBar = new Utils::StyledBar;
    QHBoxLayout *toolLayout = new QHBoxLayout(m_toolBar);
    toolLayout->setMargin(0);
    toolLayout->setSpacing(0);
    toolLayout->addWidget(m_titleLabel);
    toolLayout->addWidget(new Utils::StyledSeparator);
    m_clearButton = new QToolButton;
    toolLayout->addWidget(m_clearButton);
    toolLayout->addWidget(m_opToolBarWidgets);
    toolLayout->addWidget(m_minMaxButton);
    toolLayout->addWidget(m_closeButton);
    mainlayout->addWidget(m_toolBar);
    mainlayout->addWidget(m_outputWidgetPane, 10);
    mainlayout->addWidget(new Core::FindToolBarPlaceHolder(this));
    setLayout(mainlayout);

    m_buttonsWidget = new QWidget;
    m_buttonsWidget->setLayout(new QHBoxLayout);
    m_buttonsWidget->layout()->setContentsMargins(5,0,0,0);
    m_buttonsWidget->layout()->setSpacing(4);
}

OutputOutputPaneManager::~OutputOutputPaneManager()
{
}

QWidget *OutputOutputPaneManager::buttonsWidget()
{
    return m_buttonsWidget;
}

// Return shortcut as Ctrl+<number>
static inline int paneShortCut(int number)
{
    const int modifier = Utils::HostOsInfo::isMacHost() ? Qt::CTRL : Qt::ALT;
    return modifier | (Qt::Key_0 + number);
}

void OutputOutputPaneManager::init()
{
    ActionContainer *mwindow = ActionManager::actionContainer(Constants::M_WINDOW);
    const Context globalContext(Constants::C_GLOBAL);

    // Window->Output Panes
    ActionContainer *mpanes = ActionManager::createMenu(Constants::M_WINDOW_PANES);
    mwindow->addMenu(mpanes, Constants::G_WINDOW_PANES);
    mpanes->menu()->setTitle(tr("Output &Panes"));
    mpanes->appendGroup("Coreplugin.OutputPane.ActionsGroup");
    mpanes->appendGroup("Coreplugin.OutputPane.PanesGroup");

    Command *cmd;

    cmd = ActionManager::registerAction(m_clearAction, "Coreplugin.OutputPane.clear", globalContext);
    m_clearButton->setDefaultAction(cmd->action());
    mpanes->addAction(cmd, "Coreplugin.OutputPane.ActionsGroup");

    mpanes->addAction(cmd, "Coreplugin.OutputPane.ActionsGroup");

    cmd = ActionManager::registerAction(m_minMaxAction, "Coreplugin.OutputPane.minmax", globalContext);
    cmd->setDefaultKeySequence(QKeySequence(UseMacShortcuts ? tr("Ctrl+9") : tr("Alt+9")));
    cmd->setAttribute(Command::CA_UpdateText);
    cmd->setAttribute(Command::CA_UpdateIcon);
    mpanes->addAction(cmd, "Coreplugin.OutputPane.ActionsGroup");
    connect(m_minMaxAction, SIGNAL(triggered()), this, SLOT(slotMinMax()));
    m_minMaxButton->setDefaultAction(cmd->action());

    mpanes->addSeparator(globalContext, "Coreplugin.OutputPane.ActionsGroup");

    QFontMetrics titleFm = m_titleLabel->fontMetrics();
    int minTitleWidth = 0;

    QList<IOutputOutputPane *>panes = ExtensionSystem::PluginManager::getObjects<IOutputOutputPane>();
    m_pane = panes.at(0);
    m_outputWidgetPane->addWidget(m_pane->outputWidget(this));

    m_titleLabel->setMinimumWidth(minTitleWidth + m_titleLabel->contentsMargins().left()
                                  + m_titleLabel->contentsMargins().right());
    readSettings();

    OutputOutputPanePlaceHolder *ph = OutputOutputPanePlaceHolder::getCurrent();
    if (ph)
    {
      showPage();
      ph->setHalfMast();
    }

}

bool OutputOutputPaneManager::isMaximized()const
{
    return m_maximised;
}

void OutputOutputPaneManager::slotMinMax()
{
    OutputOutputPanePlaceHolder *ph = OutputOutputPanePlaceHolder::getCurrent();
    QTC_ASSERT(ph, return);

    if (!ph->isVisible()) // easier than disabling/enabling the action
        return;
    m_maximised = !m_maximised;
    ph->maximizeOrMinimize(m_maximised);
    m_minMaxAction->setIcon(m_maximised ? m_minimizeIcon : m_maximizeIcon);
    m_minMaxAction->setText(m_maximised ? tr("Minimize Output Pane")
                                            : tr("Maximize Output Pane"));
}

void OutputOutputPaneManager::readSettings()
{
    QMap<QString, bool> visibility;
    QSettings *settings = ICore::settings();
    int num = settings->beginReadArray(QLatin1String(outputPaneSettingsKeyC));
    for (int i = 0; i < num; ++i) {
        settings->setArrayIndex(i);
        visibility.insert(settings->value(QLatin1String(outputPaneIdKeyC)).toString(),
                          settings->value(QLatin1String(outputPaneVisibleKeyC)).toBool());
    }
    settings->endArray();
}
void OutputOutputPaneManager::showPage()
{
    OutputOutputPanePlaceHolder *ph = OutputOutputPanePlaceHolder::getCurrent();

    if (ph) {
        // make the page visible
        m_titleLabel->setText(m_pane->displayName());
        ph->setVisible(true);
        m_pane->visibilityChanged(true);
    }
}

void OutputOutputPaneManager::saveSettings() const
{
    QSettings *settings = ICore::settings();
    settings->beginWriteArray(QLatin1String(outputPaneSettingsKeyC),
                              m_ids.size());
    for (int i = 0; i < m_ids.size(); ++i) {
        settings->setArrayIndex(i);
        settings->setValue(QLatin1String(outputPaneIdKeyC), m_ids.at(i).toString());
        //settings->setValue(QLatin1String(outputPaneVisibleKeyC), m_buttons.at(i)->isVisible());
    }
    settings->endArray();
}

void OutputOutputPaneManager::clearPage()
{
  m_pane->clearContents();
}

} // namespace Internal
} // namespace Core


