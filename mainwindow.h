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

    void update_settings();
    void update_play_pause();
    void transfer_settings_to_physics_space();
    void reset_simulation();

public slots:
    void on_actionExit_triggered();

signals:
    void update_simulation();

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget* mOSGWidget{nullptr};
    QDockWidget* mDockWidget;
    QWidget* mSettings;
    QAction* mPlayPause{nullptr};

    bool mPause{true};

protected:
    void create_toolbar();
    QAction *create_settings_action();
    QAction *create_play_pause_action();
    QAction *create_reset_action();
};

#endif // MAINWINDOW_H
