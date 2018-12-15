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
    this->addDockWidget(Qt::TopDockWidgetArea, mDockWidgetThree);
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
    mOSGWidget->setMinimumWidth( 1300 );
    mOSGWidget->setMinimumHeight( 800 );

    connect(mTeamOneMenu, SIGNAL(add_ship(int, int, int, int)), mOSGWidget, SLOT(add_ship_team_one(int, int, int, int)));
    connect(mTeamTwoMenu, SIGNAL(add_ship(int, int, int, int)), mOSGWidget, SLOT(add_ship_team_two(int, int, int, int)));
    connect(mTeamOneMenu, SIGNAL(clear_player_one_fleet()), mOSGWidget, SLOT(clear_team_one()));
    connect(mTeamTwoMenu, SIGNAL(clear_player_two_fleet()), mOSGWidget, SLOT(clear_team_two()));
    connect(mPlanetMenu, SIGNAL(add_planets(int, int, int, int)), mOSGWidget, SLOT(add_planet(int, int ,int, int)));
    connect(mPlanetMenu, SIGNAL(clear_planets()), mOSGWidget, SLOT(clear_planets()));
    connect(this, SIGNAL(restart_simulation()), mOSGWidget, SLOT(reset_simulation()));

    create_toolbar();
}

void MainWindow::toggle_players_editor()
{
    if(mDockWidgetOne->isHidden() || mDockWidgetTwo->isHidden())
    {
        mDockWidgetOne->setVisible(true);
        mDockWidgetTwo->setVisible(true);
    }
    else
    {
        mDockWidgetOne->setVisible(false);
        mDockWidgetTwo->setVisible(false);
    }
}

void MainWindow::toggle_planet_editor()
{
    if(mDockWidgetThree->isHidden())
    {
        mDockWidgetThree->setVisible(true);
    }
    else
    {
        mDockWidgetThree->setVisible(false);
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
    emit(restart_simulation());
}

void MainWindow::create_toolbar()
{
    QToolBar *toolBar = addToolBar(tr("My Actions"));
    QAction *playerAction = create_player_menu_action();
    toolBar->addAction(playerAction);
    QAction *planetAction = create_planet_menu_action();
    toolBar->addAction(planetAction);
    QAction *reset = create_reset_action();
    toolBar->addAction(reset);
}

QAction * MainWindow::create_player_menu_action()
{
    const QIcon playerIcon = QIcon(":icons/Player.png");
    QAction *playerAction = new QAction(playerIcon, tr("&Toggle Player Menues"), this);
    playerAction->setShortcut(QKeySequence{tr("Ctrl+M")});
    playerAction->setStatusTip(tr("Edit Teams"));
    connect(playerAction, &QAction::triggered, this, &MainWindow::toggle_players_editor);

    return playerAction;
}

QAction * MainWindow::create_planet_menu_action()
{
    const QIcon planetIcon = QIcon(":icons/Planet.jpeg");
    QAction *planetAction = new QAction(planetIcon, tr("&Play/Pause"),this);
    planetAction->setShortcut(QKeySequence{tr("Ctrl+P")});
    connect(planetAction, &QAction::triggered, this, &MainWindow::toggle_planet_editor);
    return planetAction;
}

QAction * MainWindow::create_reset_action()
{
    const QIcon resetIcon = QIcon(":icons/reset.jpeg");
    QAction *resetAction = new QAction(resetIcon, tr("&Reset"),this);
    resetAction->setShortcut(QKeySequence{tr("Ctrl+R")});
    connect(resetAction, &QAction::triggered, this, &MainWindow::reset_simulation);
    return resetAction;
}

