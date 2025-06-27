#include "mainmenu.h"
#include "nivel1.h"
#include "nivel2.h"
#include "nivel3.h"
#include <Qlabel>
#include <QVBoxLayout>


MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Desafío Final: Dragon Ball");
    resize(800, 300);

    // Crear un widget contenedor para los botones
    QWidget *buttonContainer = new QWidget(this);

    // Configurar el layout para los botones
    QVBoxLayout *layout = new QVBoxLayout(buttonContainer);
    layout->setContentsMargins(310, 100, 100, 100); // Ajustar márgenes para que no ocupen todo el espacio

    QPushButton *nivel1Btn = new QPushButton("Nivel 1: Ascenso al Templo");
    QPushButton *nivel2Btn = new QPushButton("Nivel 2: Entrenamiento");
    QPushButton *nivel3Btn = new QPushButton("Nivel 3: Encuentro KamiSama");

    // Estilo para los botones
    QString buttonStyle = "QPushButton { background: solid #19b72d; border: 2px solid #C0C0C0; padding: 10px; }";
    nivel1Btn->setStyleSheet(buttonStyle);
    nivel2Btn->setStyleSheet(buttonStyle);
    nivel3Btn->setStyleSheet(buttonStyle);

    layout->addWidget(nivel1Btn);
    layout->addWidget(nivel2Btn);
    layout->addWidget(nivel3Btn);

    // Fondo como QLabel
    QLabel *fondo = new QLabel(this);
    fondo->setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/backgroun.png").scaled(800, 300));
    fondo->setScaledContents(true);
    fondo->lower();  // Asegura que quede detrás de los botones

    // Conectar señales de los botones
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

