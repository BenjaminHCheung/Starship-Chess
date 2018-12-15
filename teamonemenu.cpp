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
