#ifndef TEAMONEMENU_H
#define TEAMONEMENU_H

#include <QDialog>
#include <string>

namespace Ui {
class TeamOneMenu;
}

class TeamOneMenu : public QDialog
{
    Q_OBJECT

public:
    explicit TeamOneMenu(QWidget *parent = nullptr);
    ~TeamOneMenu();

private:
    Ui::TeamOneMenu *ui;

private slots:
    void on_addButton_pressed(std::string shipType, int XPosition, int YPosition, int ZPosition);
};

#endif // TEAMONEMENU_H
