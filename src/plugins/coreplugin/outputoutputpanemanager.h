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
//class OutputPaneToggleButton;
//class OutputPaneManageButton;

class OutputOutputPaneManager : public QWidget
{
    Q_OBJECT

public:
    void init();
    static OutputOutputPaneManager *instance();
    QWidget *buttonsWidget();
    //void updateStatusButtons(bool visible);

    bool isMaximized()const;

public slots:
    //void slotHide();
    //void slotNext();
    //void slotPrev();
    //void shortcutTriggered();
    void slotMinMax();

protected:
    //void focusInEvent(QFocusEvent *e);

    void showPage();
private slots:
    //void togglePage(int flags);
    void clearPage();
    //void buttonTriggered();
    //void updateNavigateState();
    //void popupMenu();
    void saveSettings() const;
    //void flashButton();
    //void setBadgeNumber(int number);

private:
    // the only class that is allowed to create and destroy
    friend class MainWindow;
    //friend class OutputPaneManageButton;

    static void create();
    static void destroy();

    explicit OutputOutputPaneManager(QWidget *parent = 0);
    ~OutputOutputPaneManager();

    //void showPage(int idx, int flags);
    void ensurePageVisible(int idx);
    //int findIndexForPage(IOutputOutputPane *out);
    //int currentIndex() const;
    //void setCurrentIndex(int idx);
    //void buttonTriggered(int idx);
    void readSettings();

    QLabel *m_titleLabel;
    //OutputPaneManageButton *m_manageButton;
    QAction *m_clearAction;
    QToolButton *m_clearButton;
    QToolButton *m_closeButton;

    QAction *m_minMaxAction;
    QToolButton *m_minMaxButton;

    //QAction *m_nextAction;
    //QAction *m_prevAction;
    //QToolButton *m_prevToolButton;
    //QToolButton *m_nextToolButton;
    QWidget *m_toolBar;

    IOutputOutputPane* m_pane;
    //QVector<OutputPaneToggleButton *> m_buttons;
    QVector<QAction *> m_actions;
    QVector<Id> m_ids;

    QStackedWidget *m_outputWidgetPane;
    QStackedWidget *m_opToolBarWidgets;
    QWidget *m_buttonsWidget;
    QPixmap m_minimizeIcon;
    QPixmap m_maximizeIcon;
    bool m_maximised;
};

/*
class OutputPaneToggleButton : public QToolButton
{
    Q_OBJECT
public:
    OutputPaneToggleButton(int number, const QString &text, QAction *action,
                           QWidget *parent = 0);
    QSize sizeHint() const;
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void flash(int count = 3);
    void setIconBadgeNumber(int number);

private slots:
    void updateToolTip();

private:
    void checkStateSet();

    QString m_number;
    QString m_text;
    QAction *m_action;
    QTimeLine *m_flashTimer;
    QLabel *m_label;
};

class OutputPaneManageButton : public QToolButton
{
    Q_OBJECT
public:
    OutputPaneManageButton();
    QSize sizeHint() const;
    void paintEvent(QPaintEvent *event);
};
*/
} // namespace Internal
} // namespace Core

#endif // OUTPUTOUTPUTPANEMANAGER_H
