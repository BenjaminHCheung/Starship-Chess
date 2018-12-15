#ifndef PLANETMENU_H
#define PLANETMENU_H

#include <QDialog>

namespace Ui {
class PlanetMenu;
}

class PlanetMenu : public QDialog
{
    Q_OBJECT

public:
    explicit PlanetMenu(QWidget *parent = nullptr);
    ~PlanetMenu();

private slots:
    void on_PlanetSize_valueChanged(int arg1);

private:
    Ui::PlanetMenu *ui;
};

#endif // PLANETMENU_H
