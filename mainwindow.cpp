#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"
#include "teamonemenu.h"
#include "teamtwomenu.h"
#include "planetmenu.h"

#include <QDockWidget>
#include <QToolBar>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);

    mDockWidgetOne = new QDockWidget("PlayerOneMenu", this);
    mDockWidgetTwo = new QDockWidget("PlayerTwoMenu", this);
    mDockWidgetThree = new QDockWidget("PlanetMenu", this);
    this->addDockWidget(Qt::RightDockWidgetArea, mDockWidgetOne);
    this->addDockWidget(Qt::LeftDockWidgetArea, mDockWidgetTwo);
    this->addDockWidget(Qt::BottomDockWidgetArea, mDockWidgetThree);
    mTeamOneMenu = new TeamOneMenu(this);
    mTeamTwoMenu = new TeamTwoMenu(this);
    mPlanetMenu = new PlanetMenu(this);
    mDockWidgetOne->setWidget(mTeamOneMenu);
    mDockWidgetTwo->setWidget(mTeamTwoMenu);
    mDockWidgetThree->setWidget(mPlanetMenu);

    mDockWidgetOne->setMinimumWidth( 250 );
    mDockWidgetTwo->setMinimumWidth( 250 );

    mOSGWidget = new OSGWidget{this};
    this->setCentralWidget(mOSGWidget);
    mOSGWidget->setMinimumWidth( 1400 );
    mOSGWidget->setMinimumHeight( 1000 );

    connect(mTeamOneMenu, SIGNAL(add_ship(int, int, int, int)), mOSGWidget, SLOT(add_ship_team_one(int, int, int, int)));
    connect(mTeamTwoMenu, SIGNAL(add_ship(int, int, int, int)), mOSGWidget, SLOT(add_ship_team_two(int, int, int, int)));
    connect(mTeamOneMenu, SIGNAL(clear_player_one_fleet()), mOSGWidget, SLOT(clear_team_one()));
    connect(mTeamTwoMenu, SIGNAL(clear_player_two_fleet()), mOSGWidget, SLOT(clear_team_two()));
    connect(mPlanetMenu, SIGNAL(add_planets(int, int, int, int)), mOSGWidget, SLOT(add_planet(int, int ,int, int)));
    connect(mPlanetMenu, SIGNAL(clear_planets()), mOSGWidget, SLOT(clear_planets()));
}

//void MainWindow::update_settings()
//{
//    if(mDockWidget->isHidden())
//    {
//        mDockWidget->setVisible(true);
//    }
//    else
//    {
//        mDockWidget->setVisible(false);
//    }
//}

/*void MainWindow::update_play_pause()
{
    if(mPause)
    {
        mPause = false;
        const QIcon pauseIcon = QIcon(":newIcons/Pause.png");
        mPlayPause->setIcon(pauseIcon);
    }
    else
    {
        mPause = true;
        const QIcon playIcon = QIcon(":newIcons/PlayButton.png");
        mPlayPause->setIcon(playIcon);
    }
}
*/
MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

//void MainWindow::reset_simulation()
//{
//    emit(update_simulation());
//}

//void MainWindow::create_toolbar()
//{
//    QToolBar *toolBar = addToolBar(tr("My Actions"));
//    QAction *settingsAction = create_settings_action();
//    toolBar->addAction(settingsAction);
//    QAction *playPause = create_play_pause_action();
//    toolBar->addAction(playPause);
//    mPlayPause = playPause;
//    QAction *reset = create_reset_action();
//    toolBar->addAction(reset);
//}

//QAction * MainWindow::create_settings_action()
//{
//    const QIcon settingsIcon = QIcon(":myicons/Settings-icon.png");
//    QAction *settingsAction = new QAction(settingsIcon, tr("&Change Settings"), this);
//    settingsAction->setShortcut(QKeySequence{tr("Ctrl+B")});
//    settingsAction->setStatusTip(tr("Change Settings"));
//    connect(settingsAction, &QAction::triggered, this, &MainWindow::update_settings);

//    return settingsAction;
//}

//QAction * MainWindow::create_play_pause_action()
//{
//    const QIcon playPauseIcon = QIcon(":newIcons/PlayButton.png");
//    QAction *playPauseAction = new QAction(playPauseIcon, tr("&Play/Pause"),this);
//    playPauseAction->setShortcut(QKeySequence{tr("Ctrl+P")});
//    connect(playPauseAction, &QAction::triggered, this, &MainWindow::update_play_pause);
//    return playPauseAction;
//}

//QAction * MainWindow::create_reset_action()
//{
//    const QIcon resetIcon = QIcon(":newIcons/Reset.jpg");
//    QAction *resetAction = new QAction(resetIcon, tr("&Reset"),this);
//    resetAction->setShortcut(QKeySequence{tr("Ctrl+R")});
//    connect(resetAction, &QAction::triggered, this, &MainWindow::reset_simulation);
//    return resetAction;
//}

