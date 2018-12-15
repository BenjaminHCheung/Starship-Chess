#ifndef TEAMTWOMENU_H
#define TEAMTWOMENU_H

#include <QDialog>

namespace Ui {
class TeamTwoMenu;
}

class TeamTwoMenu : public QDialog
{
    Q_OBJECT

public:
    explicit TeamTwoMenu(QWidget *parent = nullptr);
    ~TeamTwoMenu();

private:
    Ui::TeamTwoMenu *ui;
};

#endif // TEAMTWOMENU_H
