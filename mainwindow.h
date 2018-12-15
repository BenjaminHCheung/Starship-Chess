#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

class OutputWindow;
class OSGWidget;

namespace Ui {
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void toggle_players_editor();
    void toggle_planet_editor();
    void reset_simulation();

public slots:
    void on_actionExit_triggered();

signals:
    void restart_simulation();

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget* mOSGWidget{nullptr};
    QDockWidget* mDockWidgetOne{nullptr};
    QDockWidget* mDockWidgetTwo{nullptr};
    QDockWidget* mDockWidgetThree{nullptr};
    QWidget* mTeamOneMenu{nullptr};
    QWidget* mTeamTwoMenu{nullptr};
    QWidget* mPlanetMenu{nullptr};
protected:
    void create_toolbar();
    QAction *create_player_menu_action();
    QAction *create_planet_menu_action();
    QAction *create_reset_action();
};

#endif // MAINWINDOW_H
