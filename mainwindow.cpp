#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"
#include "settings.h"

#include <QDockWidget>
#include <QToolBar>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);

    mDockWidget = new QDockWidget("Settings",this);
    this->addDockWidget(Qt::LeftDockWidgetArea,mDockWidget);
    mSettings = new Settings(this);
    mDockWidget->setWidget(mSettings);

    mOSGWidget = new OSGWidget{this};
    this->setCentralWidget(mOSGWidget);

    create_toolbar();

    connect(this, SIGNAL(update_simulation()), mSettings, SLOT(transfer_data()));
    connect(mSettings, SIGNAL(settings_data(unsigned int,
                                           double, double, double,
                                           double, double, double,
                                           double, double, double,
                                           double, double, double))
            , mOSGWidget, SLOT(apply_settings(unsigned int,
                                              double, double, double,
                                              double, double, double,
                                              double, double, double,
                                              double, double, double)));
}

void MainWindow::update_settings()
{
    if(mDockWidget->isHidden())
    {
        mDockWidget->setVisible(true);
    }
    else
    {
        mDockWidget->setVisible(false);
    }
}

void MainWindow::update_play_pause()
{
    if(mPause)
    {
        mOSGWidget->set_pause(false);
        mPause = false;
        const QIcon pauseIcon = QIcon(":newIcons/Pause.png");
        mPlayPause->setIcon(pauseIcon);
    }
    else
    {
        mOSGWidget->set_pause(true);
        mPause = true;
        const QIcon playIcon = QIcon(":newIcons/PlayButton.png");
        mPlayPause->setIcon(playIcon);
    }
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::reset_simulation()
{
    emit(update_simulation());
}

void MainWindow::create_toolbar()
{
    QToolBar *toolBar = addToolBar(tr("My Actions"));
    QAction *settingsAction = create_settings_action();
    toolBar->addAction(settingsAction);
    QAction *playPause = create_play_pause_action();
    toolBar->addAction(playPause);
    mPlayPause = playPause;
    QAction *reset = create_reset_action();
    toolBar->addAction(reset);
}

QAction * MainWindow::create_settings_action()
{
    const QIcon settingsIcon = QIcon(":myicons/Settings-icon.png");
    QAction *settingsAction = new QAction(settingsIcon, tr("&Change Settings"), this);
    settingsAction->setShortcut(QKeySequence{tr("Ctrl+B")});
    settingsAction->setStatusTip(tr("Change Settings"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::update_settings);

    return settingsAction;
}

QAction * MainWindow::create_play_pause_action()
{
    const QIcon playPauseIcon = QIcon(":newIcons/PlayButton.png");
    QAction *playPauseAction = new QAction(playPauseIcon, tr("&Play/Pause"),this);
    playPauseAction->setShortcut(QKeySequence{tr("Ctrl+P")});
    connect(playPauseAction, &QAction::triggered, this, &MainWindow::update_play_pause);
    return playPauseAction;
}

QAction * MainWindow::create_reset_action()
{
    const QIcon resetIcon = QIcon(":newIcons/Reset.jpg");
    QAction *resetAction = new QAction(resetIcon, tr("&Reset"),this);
    resetAction->setShortcut(QKeySequence{tr("Ctrl+R")});
    connect(resetAction, &QAction::triggered, this, &MainWindow::reset_simulation);
    return resetAction;
}


