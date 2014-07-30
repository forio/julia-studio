#ifndef OUTPUTOUTPUTPANEMANAGER_H
#define OUTPUTOUTPUTPANEMANAGER_H

#include <coreplugin/id.h>

#include <QMap>
#include <QToolButton>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QLabel;
class QSplitter;
class QStackedWidget;
class QTimeLine;
class QLabel;
QT_END_NAMESPACE

namespace Core {

class IOutputOutputPane;

namespace Internal {

class MainWindow;

class OutputOutputPaneManager : public QWidget
{
    Q_OBJECT

public:
    void init();
    static OutputOutputPaneManager *instance();
    QWidget *buttonsWidget();

    bool isMaximized()const;

public slots:
    void slotMinMax();

protected:

    void showPage();
private slots:
    void clearPage();
    void saveSettings() const;

private:
    friend class MainWindow;

    static void create();
    static void destroy();

    explicit OutputOutputPaneManager(QWidget *parent = 0);
    ~OutputOutputPaneManager();

    void ensurePageVisible(int idx);
    void readSettings();

    QLabel *m_titleLabel;
    QAction *m_clearAction;
    QToolButton *m_clearButton;
    QToolButton *m_closeButton;

    QAction *m_minMaxAction;
    QToolButton *m_minMaxButton;

    QWidget *m_toolBar;

    IOutputOutputPane* m_pane;
    QVector<QAction *> m_actions;
    QVector<Id> m_ids;

    QStackedWidget *m_outputWidgetPane;
    QStackedWidget *m_opToolBarWidgets;
    QWidget *m_buttonsWidget;
    QPixmap m_minimizeIcon;
    QPixmap m_maximizeIcon;
    bool m_maximised;
};

} // namespace Internal
} // namespace Core

#endif // OUTPUTOUTPUTPANEMANAGER_H
