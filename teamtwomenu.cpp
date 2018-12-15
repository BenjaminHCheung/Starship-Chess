#include "teamtwomenu.h"
#include "ui_teamtwomenu.h"

TeamTwoMenu::TeamTwoMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamTwoMenu)
{
    ui->setupUi(this);
}

TeamTwoMenu::~TeamTwoMenu()
{
    delete ui;
}

void TeamTwoMenu::on_ShipType_currentIndexChanged(const QString &shipClass)
{
    if(shipClass == "BattleShip")
    {
        mShipClass = 4;
    }
    else if(shipClass == "Cruiser")
    {
        mShipClass = 3;
    }
    else if(shipClass == "Destroyer")
    {
        mShipClass = 2;
    }
    else if(shipClass == "Corvette")
    {
        mShipClass = 1;
    }
}

void TeamTwoMenu::on_XPosition_valueChanged(int xPosition)
{
    mXPosition = xPosition;
}

void TeamTwoMenu::on_YPosition_valueChanged(int yPosition)
{
    mYPosition = yPosition;
}

void TeamTwoMenu::on_ZPosition_valueChanged(int zPosition)
{
    mZPosition = zPosition;
}

void TeamTwoMenu::on_pushButton_clicked()
{
    emit(add_ship(mShipClass, mXPosition, mYPosition, mZPosition));
    ui->XPosition->setValue(0);
    ui->YPosition->setValue(0);
    ui->ZPosition->setValue(0);
    ui->ShipType->setCurrentIndex(0);
}

void TeamTwoMenu::on_PlayerTwoFleet_clicked(const QModelIndex &index)
{

}

void TeamTwoMenu::on_ClearButton_clicked()
{
        emit(clear_player_one_fleet());
}

void TeamTwoMenu::on_RemoveButton_clicked()
{

}
