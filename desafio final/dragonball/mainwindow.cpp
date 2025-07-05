#include "mainwindow.h"
#include "mainmenu.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include <QStyle>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupWindow();
    centerWindow();
}

void MainWindow::setupWindow() {
    setWindowTitle("Desafío Final: Dragon Ball");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    mainMenu = new MainMenu();
    nivel1 = nullptr;
    nivel2 = nullptr;
    nivel3 = nullptr;

    stackedWidget->addWidget(mainMenu);
    ajustarVentana(); // Ajusta el tamaño inicial al menú

    connect(mainMenu, &MainMenu::nivel1Requested, [this]() {
        if (!nivel1) {
            nivel1 = new Nivel1();
            stackedWidget->addWidget(nivel1);

            // Conexión para volver al menú
            connect(nivel1, &Nivel1::solicitarMenu, [this]() {
                stackedWidget->setCurrentWidget(mainMenu);

                nivel1->deleteLater();
                nivel1 = nullptr;
                ajustarVentana();
            });

            // Conexión para completar nivel 1
            connect(nivel1, &Nivel1::nivelCompletado, [this]() {
                if (!nivel2) {
                    nivel2 = new Nivel2();
                    stackedWidget->addWidget(nivel2);

                    connect(nivel2, &Nivel2::solicitarMenu, [this]() {
                        stackedWidget->setCurrentWidget(mainMenu);
                        nivel2->deleteLater();
                        nivel2 = nullptr;
                        ajustarVentana();
                    });

                    connect(nivel2, &Nivel2::nivelCompletado, [this]() {
                        if (!nivel3) {
                            nivel3 = new Nivel3();
                            stackedWidget->addWidget(nivel3);
                            connect(nivel3, &Nivel3::volverAlMenu, [this]() {
                                stackedWidget->setCurrentWidget(mainMenu);
                                nivel3->deleteLater();
                                nivel3 = nullptr;
                                ajustarVentana();
                            });
                        }
                        stackedWidget->setCurrentWidget(nivel3);
                        nivel2->deleteLater();
                        nivel2 = nullptr;
                        ajustarVentana();
                    });
                }
                stackedWidget->setCurrentWidget(nivel2);
                nivel1->deleteLater();
                nivel1 = nullptr;
                ajustarVentana();
            });
        }
        stackedWidget->setCurrentWidget(nivel1);
        ajustarVentana();
    });

    // Conexión DIRECTA del menú al nivel 2 (debe estar FUERA del connect de nivel1Requested)
    connect(mainMenu, &MainMenu::nivel2Requested, [this]() {
        if (!nivel2) {
            nivel2 = new Nivel2();
            stackedWidget->addWidget(nivel2);

            connect(nivel2, &Nivel2::solicitarMenu, [this]() {
                stackedWidget->setCurrentWidget(mainMenu);
                ajustarVentana();
                nivel2->deleteLater();
                nivel2 = nullptr;
            });
        }
        stackedWidget->setCurrentWidget(nivel2);
        ajustarVentana();
    });

    connect(mainMenu, &MainMenu::nivel3Requested, [this]() {
        if (!nivel3) {
            nivel3 = new Nivel3();
            stackedWidget->addWidget(nivel3);
            connect(nivel3, &Nivel3::volverAlMenu, [this]() {
                stackedWidget->setCurrentWidget(mainMenu);
                ajustarVentana();
                nivel3->deleteLater();
                nivel3 = nullptr;
            });
        }
        stackedWidget->setCurrentWidget(nivel3);
        ajustarVentana();
    });
}

// Nuevo método para ajustar el tamaño de la ventana
void MainWindow::ajustarVentana() {
    QWidget *currentWidget = stackedWidget->currentWidget();
    if (currentWidget) {
        resize(currentWidget->size());
        centerWindow();
    }
}

void MainWindow::centerWindow() {
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    move(screenGeometry.center() - rect().center());
}

MainWindow::~MainWindow() {
    if (nivel1) nivel1->deleteLater();
    if (nivel2) nivel2->deleteLater();
    if (nivel3) nivel3->deleteLater();
}
