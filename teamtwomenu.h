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

signals:
    void add_ship(int, int, int, int);
    void clear_player_two_fleet();

private:
    Ui::TeamTwoMenu *ui;

    int mShipClass{4};
    int mXPosition{0};
    int mYPosition{0};
    int mZPosition{0};

private slots:

    void on_ShipType_currentIndexChanged(const QString &shipClass);
    void on_XPosition_valueChanged(int xPosition);
    void on_YPosition_valueChanged(int yposition);
    void on_ZPosition_valueChanged(int zPosition);
    void on_pushButton_clicked();
    void on_PlayerTwoFleet_clicked(const QModelIndex &index);
    void on_ClearButton_clicked();
    void on_RemoveButton_clicked();
};

#endif // TEAMTWOMENU_H
