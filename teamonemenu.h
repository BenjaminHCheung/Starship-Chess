#ifndef TEAMONEMENU_H
#define TEAMONEMENU_H

#include <QDialog>

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
};

#endif // TEAMONEMENU_H
