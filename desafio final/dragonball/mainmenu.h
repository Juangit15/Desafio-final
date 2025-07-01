#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QPainter> // Añadir esta línea

class MainMenu : public QWidget {
    Q_OBJECT
public:
    MainMenu(QWidget *parent = nullptr);

signals:
    void nivel1Requested();
    void nivel2Requested();
    void nivel3Requested();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUI();
    QLabel *background;
};

#endif // MAINMENU_H
