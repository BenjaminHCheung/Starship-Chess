#include "teamonemenu.h"
#include "ui_teamonemenu.h"

TeamOneMenu::TeamOneMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamOneMenu)
{
    ui->setupUi(this);    
}

TeamOneMenu::~TeamOneMenu()
{
    delete ui;
}

void TeamOneMenu::on_ShipType_activated(const QString &shipClass)
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

void TeamOneMenu::on_XPosition_valueChanged(int xPosition)
{
    mXPosition = xPosition;
}

void TeamOneMenu::on_YPosition_valueChanged(int yPosition)
{
    mYPosition = yPosition;
}

void TeamOneMenu::on_ZPosition_valueChanged(int zPosition)
{
    mZPosition = zPosition;
}

void TeamOneMenu::on_addButton_clicked()
{
    emit(add_ship(mShipClass, mXPosition, mYPosition, mZPosition));
    ui->XPosition->setValue(0);
    ui->YPosition->setValue(0);
    ui->ZPosition->setValue(0);
    ui->ShipType->setCurrentIndex(0);
    mXPosition = 0;
    mYPosition = 0;
    mZPosition = 0;
}

void TeamOneMenu::on_PlayerOneFleet_clicked(const QModelIndex &index)
{

}

void TeamOneMenu::on_ClearButton_clicked()
{
    emit(clear_player_one_fleet());
}

void TeamOneMenu::on_RemoveButton_clicked()
{

}
