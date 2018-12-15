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

void TeamOneMenu::on_addButton_pressed(std::string shipType, int XPosition, int YPosition, int ZPosition)
{
    int classNumber{0};
    if(shipType == "BattleShip")
    {
        classNumber = 4;
    }
    else if(shipType == "Cruiser")
    {
        classNumber = 3;
    }
    else if(shipType == "Destroyer")
    {
        classNumber = 2;
    }
    else if(shipType == "Corvette")
    {
        classNumber = 1;
    }

    this->open().
}
