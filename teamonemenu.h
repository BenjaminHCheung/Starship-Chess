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

signals:
    void add_ship(int, int, int, int);
    void clear_player_one_fleet();

private:
    Ui::TeamOneMenu *ui;

    int mShipClass{4};
    int mXPosition{0};
    int mYPosition{0};
    int mZPosition{0};

private slots:
    void on_ShipType_activated(const QString &shipClass);
    void on_XPosition_valueChanged(int xPosition);
    void on_YPosition_valueChanged(int yPosition);
    void on_ZPosition_valueChanged(int zPosition);
    void on_addButton_clicked();
    void on_PlayerOneFleet_clicked(const QModelIndex &index);
    void on_ClearButton_clicked();
    void on_RemoveButton_clicked();
};

#endif // TEAMONEMENU_H
