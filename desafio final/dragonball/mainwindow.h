#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSize>

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

    QStackedWidget *stackedWidget;
    MainMenu *mainMenu;
    Nivel1 *nivel1;
    Nivel2 *nivel2;
    Nivel3 *nivel3;

    const QSize menuSize = QSize(800, 600); // Tamaño fijo para el menú
};

#endif // MAINWINDOW_H
