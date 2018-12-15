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

}

void TeamOneMenu::on_XPosition_valueChanged(int arg1)
{

}

void TeamOneMenu::on_YPosition_valueChanged(int arg1)
{

}

void TeamOneMenu::on_ZPosition_valueChanged(int arg1)
{

}

void TeamOneMenu::on_addButton_clicked()
{

    ui->XPosition->setValue(0);
    ui->YPosition->setValue(0);
    ui->ZPosition->setValue(0);
    ui->ShipType->setCurrentIndex(0);
}

void TeamOneMenu::on_PlayerOneFleet_clicked(const QModelIndex &index)
{

}

void TeamOneMenu::on_ClearButton_clicked()
{

}

void TeamOneMenu::on_RemoveButton_clicked()
{

}

void TeamOneMenu::on_tableWidget_clicked(const QModelIndex &index)
{

}
