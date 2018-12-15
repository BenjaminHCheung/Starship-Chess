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
    mSize = size;
}

void PlanetMenu::on_xPosition_valueChanged(int xPosition)
{
    mXPosition = xPosition;
}

void PlanetMenu::on_yPosition_valueChanged(int yPosition)
{
    mYPosition = yPosition;
}

void PlanetMenu::on_zPosition_valueChanged(int zPosition)
{
    mZPosition = zPosition;
}

void PlanetMenu::on_AddButton_clicked()
{
    emit(add_planets(mSize, mXPosition, mYPosition, mZPosition));
    ui->PlanetSize->setValue(0);
    ui->xPosition->setValue(0);
    ui->yPosition->setValue(0);
    ui->zPosition->setValue(0);
    mXPosition = 0;
    mYPosition = 0;
    mZPosition = 0;
}

void PlanetMenu::on_ClearButton_clicked()
{
    emit(clear_planets());
}
