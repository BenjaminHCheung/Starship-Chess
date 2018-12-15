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

void PlanetMenu::on_PlanetSize_valueChanged(int arg1)
{

}

