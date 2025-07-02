#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainMenu;
class Nivel1;
class Nivel2;
class Nivel3;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupWindow();
    void centerWindow();
    void ajustarVentana();

    QStackedWidget *stackedWidget;
    MainMenu *mainMenu;
    Nivel1 *nivel1;
    Nivel2 *nivel2;
    Nivel3 *nivel3;
};

#endif
