#include "planetmenu.h"
#include "ui_planetmenu.h"

PlanetMenu::PlanetMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanetMenu)
{
    ui->setupUi(this);
}

PlanetMenu::~PlanetMenu()
{
    delete ui;
}

void PlanetMenu::on_PlanetSize_valueChanged(int size)
{

}


void PlanetMenu::on_xPosition_valueChanged(int xPosition)
{

}

void PlanetMenu::on_yPosition_valueChanged(int yPosition)
{

}

void PlanetMenu::on_zPosition_valueChanged(int zPosition)
{

}

void PlanetMenu::on_AddButton_clicked()
{

}

void PlanetMenu::on_ClearButton_clicked()
{

}
