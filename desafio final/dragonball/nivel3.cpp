#include "nivel3.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


Nivel3::Nivel3(QWidget *parent) : QGraphicsView(parent), gravedadActual(0.5), enZonaGravedad(false) {
    setupScene();
}

void Nivel3::setupScene() {
    // Crear escena
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(1000, 700);
    scene->setSceneRect(0, 0, 990, 690);

    // Fondo
    QPixmap fondo("C:/Users/juanm/Downloads/DragonBall/recursos/background3.jpg");
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(fondo.scaled(990, 690));
    background->setZValue(-1);
    scene->addItem(background);

    QPixmap kamiPixmap("C:/Users/juanm/Downloads/DragonBall/recursos/kamisama_sprite.png");
    if (!kamiPixmap.isNull()) {
        QGraphicsPixmapItem *kami = new QGraphicsPixmapItem(kamiPixmap.scaled(120, 120));
        kami->setPos(850, 100);
        kami->setZValue(1);
        scene->addItem(kami);
    }

    // Primero crear plataformas (ANTES que Goku)
    crearPlataformas();

    // Crear jugador Goku DESPUÉS de crear las plataformas
    goku = new Jugador();
    goku->setGravedad(gravedadActual);
    scene->addItem(goku);

    // Posicionarlo sobre la primera plataforma
    goku->setPos(
        plataformas[0]->x() + 10,
        plataformas[0]->y() - goku->boundingRect().height()
        );

    connect(goku, &Jugador::solicitarMenu, this, &Nivel3::volverAlMenu);


    // Crear sensores y timers
    crearSensores();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel3::actualizar);
    timer->start(30);

    gravityTimer = new QTimer(this);
    connect(gravityTimer, &QTimer::timeout, this, &Nivel3::cambiarGravedad);
    gravityTimer->start(10000);  // cada 10 segundos
}


void Nivel3::crearPlataformas() {
    // Plataformas normales
    for (int i = 0; i < 5; ++i) {
        QGraphicsRectItem *plat = new QGraphicsRectItem(0, 0, 150, 20);
        plat->setPos(100 + i * 150, 550 - i * 80);
        plat->setBrush(Qt::darkGray);
        scene->addItem(plat);
        plataformas.append(plat);
    }

    // Plataforma final
    QGraphicsRectItem *finalPlat = new QGraphicsRectItem(0, 0, 200, 20);
    finalPlat->setPos(600, 200);
    finalPlat->setBrush(Qt::green);
    scene->addItem(finalPlat);
    plataformas.append(finalPlat);
}

void Nivel3::crearSensores() {
    // Sensor de gravedad inversa
    QGraphicsEllipseItem *sensorGravedad = new QGraphicsEllipseItem(0, 0, 100, 100);
    sensorGravedad->setPos(400, 300);
    sensorGravedad->setBrush(Qt::blue);
    sensorGravedad->setOpacity(0.5);
    scene->addItem(sensorGravedad);
    sensores.append(sensorGravedad);

    // Sensor de meta
    QGraphicsEllipseItem *sensorMeta = new QGraphicsEllipseItem(0, 0, 50, 50);
    sensorMeta->setPos(650, 180);
    sensorMeta->setBrush(Qt::yellow);
    sensorMeta->setOpacity(0.5);
    scene->addItem(sensorMeta);
    sensores.append(sensorMeta);
}

void Nivel3::actualizar() {
    goku->moverConGravedad(plataformas, gravedadActual);
    verificarSensores();
}

void Nivel3::cambiarGravedad() {
    if (enZonaGravedad) return; // No cambiar si está en zona especial

    gravedadActual = (gravedadActual == 0.5) ? 0.2 : 0.5; // Alternar entre gravedad normal y baja
    goku->setGravedad(gravedadActual);
}

void Nivel3::verificarSensores() {
    // Sensor de gravedad inversa
    if (goku->collidesWithItem(sensores[0])) {
        if (!enZonaGravedad) {
            enZonaGravedad = true;
            gravedadActual = -0.3;
            goku->setGravedad(gravedadActual);
        }
    }

    else if (goku->collidesWithItem(sensores[1])) {
        PlaySound(TEXT("C:/Users/juanm/Downloads/DragonBall/recursos/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
        QTimer::singleShot(4000, this, [this]() {
            emit volverAlMenu();
        });
    }

    // Resto de casos (cuando ya no está en el sensor)
    else {
        if (enZonaGravedad) {
            enZonaGravedad = false;
            gravedadActual = 0.5;
            goku->setGravedad(gravedadActual);
        }
    }
}


Nivel3::~Nivel3() {
    if (timer) timer->stop();
    if (gravityTimer) gravityTimer->stop();
    delete timer;
    delete gravityTimer;

    scene->clear();
    delete scene;
}
