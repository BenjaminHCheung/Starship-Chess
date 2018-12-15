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
