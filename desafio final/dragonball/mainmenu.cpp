#include "mainmenu.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include <Qlabel>
#include <QVBoxLayout>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Desafío Final: Dragon Ball");
    resize(800, 600);

    // Fondo como QLabel
    QLabel *fondo = new QLabel(this);
    fondo->setPixmap(QPixmap(":/recursos/background.png").scaled(size()));
    fondo->setScaledContents(true);
    fondo->lower();  // Asegura que quede detrás de los botones

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *nivel1Btn = new QPushButton("Nivel 1: Ascenso al Templo");
    QPushButton *nivel2Btn = new QPushButton("Nivel 2: Entrenamiento");
    QPushButton *nivel3Btn = new QPushButton("Nivel 3: Encuentro KamiSama");

    layout->addWidget(nivel1Btn);
    layout->addWidget(nivel2Btn);
    layout->addWidget(nivel3Btn);

    connect(nivel1Btn, &QPushButton::clicked, this, [=]() {
        Nivel1 *nivel = new Nivel1();
        nivel->show();
        this->close();
    });

    connect(nivel2Btn, &QPushButton::clicked, this, [=]() {
        Nivel2 *nivel = new Nivel2();
        nivel->show();
        this->close();
    });

    connect(nivel3Btn, &QPushButton::clicked, this, [=]() {
        Nivel3 *nivel = new Nivel3();
        nivel->show();
        this->close();
    });
}
