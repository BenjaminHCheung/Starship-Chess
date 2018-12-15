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
    void on_PlanetSize_valueChanged(int size);
    void on_xPosition_valueChanged(int xPosition);
    void on_yPosition_valueChanged(int yPosition);
    void on_zPosition_valueChanged(int zPosition);
    void on_AddButton_clicked();
    void on_ClearButton_clicked();

private:
    Ui::PlanetMenu *ui;

    int mSize{0};
    int mXPosition{0};
    int mYPosition{0};
    int mZPosition{0};
};

#endif // PLANETMENU_H
