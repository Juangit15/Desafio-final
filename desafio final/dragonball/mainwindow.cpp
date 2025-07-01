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
    resize(menuSize); // Usar el tamaño definido para el menú

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    mainMenu = new MainMenu();
    nivel1 = nullptr;
    nivel2 = nullptr;
    nivel3 = nullptr;

    stackedWidget->addWidget(mainMenu);

    connect(mainMenu, &MainMenu::nivel1Requested, [this]() {
        if (!nivel1) {
            nivel1 = new Nivel1();
            stackedWidget->addWidget(nivel1);

            // Ajustar al tamaño del nivel 1
            resize(nivel1->size());
            centerWindow();

            connect(nivel1, &Nivel1::solicitarMenu, [this]() {
                stackedWidget->setCurrentWidget(mainMenu);
                // Restaurar tamaño del menú
                resize(menuSize);
                centerWindow();
                nivel1->deleteLater();
                nivel1 = nullptr;
            });

            connect(nivel1, &Nivel1::nivelCompletado, [this]() {
                if (!nivel2) nivel2 = new Nivel2();
                stackedWidget->addWidget(nivel2);
                stackedWidget->setCurrentWidget(nivel2);
                // Ajustar al tamaño del nivel 2
                resize(nivel2->size());
                centerWindow();
                nivel1->deleteLater();
                nivel1 = nullptr;
            });
        }
        stackedWidget->setCurrentWidget(nivel1);
    });

    connect(mainMenu, &MainMenu::nivel2Requested, [this]() {
        if (!nivel2) {
            nivel2 = new Nivel2();
            stackedWidget->addWidget(nivel2);
            // Ajustar al tamaño del nivel 2
            resize(nivel2->size());
            centerWindow();
        }
        stackedWidget->setCurrentWidget(nivel2);
    });

    connect(mainMenu, &MainMenu::nivel3Requested, [this]() {
        if (!nivel3) {
            nivel3 = new Nivel3();
            stackedWidget->addWidget(nivel3);
            // Ajustar al tamaño del nivel 3
            resize(nivel3->size());
            centerWindow();
        }
        stackedWidget->setCurrentWidget(nivel3);
    });
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
