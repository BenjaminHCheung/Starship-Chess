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
    void on_ShipType_activated(const QString &shipClass);
    void on_XPosition_valueChanged(int xPosition);
    void on_YPosition_valueChanged(int yPosition);
    void on_ZPosition_valueChanged(int zPosition);
    void on_addButton_clicked();
    void on_PlayerOneFleet_clicked(const QModelIndex &index);
    void on_ClearButton_clicked();
    void on_RemoveButton_clicked();
    void on_tableWidget_clicked(const QModelIndex &index);
};

#endif // TEAMONEMENU_H
