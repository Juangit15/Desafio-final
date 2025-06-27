#include "nivel1.h"
#include "mainmenu.h"
#include <QGraphicsPixmapItem>

Nivel1::Nivel1(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(810, 610);
    scene->setSceneRect(0, 0, 800, 600);

    // Fondo
    QGraphicsPixmapItem *fondo = new QGraphicsPixmapItem(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/background.png").scaled(790, 590));
    fondo->setZValue(-1);
    scene->addItem(fondo);


    goku = new Jugador();
    scene->addItem(goku);
    goku->setPos(100, 400);

    // Conectar la señal de "volverAMenu" de goku (no crear otro Jugador)
    connect(goku, &Jugador::volverAMenu, this, [this]() {
        MainMenu *menu = new MainMenu();
        menu->show();
        this->close();  // Cierra el nivel actual
    });

    // Timer para actualizar movimiento
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);
    timer->start(30);
}

void Nivel1::actualizar() {
    goku->mover();
}
