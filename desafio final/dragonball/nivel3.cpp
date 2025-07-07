#include "nivel3.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QGraphicsColorizeEffect>
#include <windows.h>
#include <mmsystem.h>
#include <QFile>


Nivel3::Nivel3(QWidget *parent): QGraphicsView(parent), gravedadActual(0.5), enZonaGravedad(false), juegoTerminado(false) {
    setupScene();
}

void Nivel3::setupScene() {
    // Crear escena
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(1000, 700);
    scene->setSceneRect(0, 0, 990, 690);

    // Fondo
    QPixmap fondo("C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/background3.jpg");
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(fondo.scaled(990, 690));
    background->setZValue(-1);
    scene->addItem(background);

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
    // Plataforma base
    QGraphicsRectItem *base = new QGraphicsRectItem(0, 0, 200, 20);
    base->setPos(100, 550);  // Nivel inferior
    base->setBrush(Qt::darkGray);
    scene->addItem(base);
    plataformas.append(base);

    // Segunda plataforma (salta hacia ella)
    QGraphicsRectItem *segunda = new QGraphicsRectItem(0, 0, 150, 20);
    segunda->setPos(300, 450);
    segunda->setBrush(Qt::darkGray);
    scene->addItem(segunda);
    plataformas.append(segunda);

    // Plataformas arriba que detienen al subir
    QGraphicsRectItem *arriba1 = new QGraphicsRectItem(0, 0, 150, 20);
    arriba1->setPos(320, 200);
    arriba1->setBrush(Qt::darkGray);
    scene->addItem(arriba1);
    plataformas.append(arriba1);

    QGraphicsRectItem *arriba2 = new QGraphicsRectItem(0, 0, 150, 20);
    arriba2->setPos(500, 150);
    arriba2->setBrush(Qt::darkGray);
    scene->addItem(arriba2);
    plataformas.append(arriba2);

    // Plataforma final (al volver a bajar)
    QGraphicsRectItem *finalPlat = new QGraphicsRectItem(0, 0, 200, 20);
    finalPlat->setPos(700, 550);
    finalPlat->setBrush(Qt::green);
    scene->addItem(finalPlat);
    plataformas.append(finalPlat);
}

void Nivel3::crearSensores() {
    // Sensor de gravedad inversa (entre plataforma base y segunda)
    QGraphicsEllipseItem *sensorSubida = new QGraphicsEllipseItem(0, 0, 50, 50);
    sensorSubida->setPos(350, 410);
    sensorSubida->setBrush(Qt::blue);
    sensorSubida->setOpacity(0.5);
    scene->addItem(sensorSubida);
    sensores.append(sensorSubida);  // index 0

    // Sensor para revertir gravedad (en la parte superior)
    QGraphicsEllipseItem *sensorBajada = new QGraphicsEllipseItem(0, 0, 50, 50);
    sensorBajada->setPos(550, 150);
    sensorBajada->setBrush(Qt::cyan);
    sensorBajada->setOpacity(0.5);
    scene->addItem(sensorBajada);
    sensores.append(sensorBajada);  // index 1

    // Sensor de meta
    QGraphicsEllipseItem *sensorMeta = new QGraphicsEllipseItem(0, 0, 60, 60);
    sensorMeta->setPos(750, 520);
    sensorMeta->setBrush(Qt::yellow);
    sensorMeta->setOpacity(0.5);
    scene->addItem(sensorMeta);
    sensores.append(sensorMeta);  // index 2
}


void Nivel3::actualizar() {
    if (juegoTerminado) return;
    goku->moverConGravedad(plataformas, gravedadActual);
    verificarSensores();

    // Reiniciar si cae fuera de la escena
    if (goku->y() > scene->height() || goku->y() < -100) {
        goku->setPos(plataformas[0]->x() + 10, plataformas[0]->y() - goku->boundingRect().height());
        goku->reiniciarEstado();
        gravedadActual = 0.5;
        goku->setGravedad(gravedadActual);
        goku->setRotation(0); // ← Reset visual
        enZonaGravedad = false;
    }
}

void Nivel3::cambiarGravedad() {
    if (enZonaGravedad) return; // No cambiar si está en zona especial

    gravedadActual = (gravedadActual == 0.5) ? 0.2 : 0.5; // Alternar entre gravedad normal y baja
    goku->setGravedad(gravedadActual);
}

void Nivel3::verificarSensores() {
    if (sensores.size() < 3) return;

    if (goku->collidesWithItem(sensores[0])) {
        gravedadActual = -0.5;
        goku->setGravedad(gravedadActual);
    } else if (goku->collidesWithItem(sensores[1])) {
        gravedadActual = 0.5;
        goku->setGravedad(gravedadActual);
    } else if (goku->collidesWithItem(sensores[2])) {
        mostrarVictoria();
    } else if (enZonaGravedad) {
        enZonaGravedad = false;
        gravedadActual = 0.5;
        goku->setGravedad(gravedadActual);
    }
}



void Nivel3::mostrarVictoria() {
    if (juegoTerminado) return;
    juegoTerminado = true;

    timer->stop();
    gravityTimer->stop();

    // Mostrar solo la imagen de KamiSama al final
    QString rutaKami = "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/kamisama.png";
    if (QFile::exists(rutaKami)) {
        QPixmap imgKami(rutaKami);
        if (!imgKami.isNull()) {
            kami = new QGraphicsPixmapItem(imgKami.scaled(200, 200));  // Tamaño visible
            kami->setPos(400, 300);  // Centro de la escena
            kami->setZValue(10);     // Por encima del fondo
            scene->addItem(kami);
        } else {
            qDebug() << " Imagen Kami cargada pero pixmap es nulo.";
        }
    } else {
        qDebug() << "Ruta del sprite de KamiSama no encontrada.";
    }

    // Crear mensaje
    QGraphicsTextItem *texto = new QGraphicsTextItem("KamiSama: ¡Has superado la prueba, Goku!");
    texto->setFont(QFont("Arial", 18, QFont::Bold));
    texto->setDefaultTextColor(Qt::black);
    texto->setZValue(11);
    texto->setPos(300, 90);
    scene->addItem(texto);

    // Reproducir sonido
    BOOL reproducido = PlaySound(TEXT("C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/DragonBall.wav"),
                                 NULL, SND_FILENAME | SND_ASYNC);
    if (!reproducido) {
        qDebug() << " No se pudo reproducir el sonido final.";
    }

    QTimer::singleShot(10000, this, [this]() {
        emit volverAlMenu();
    });
}




Nivel3::~Nivel3() {
    if (timer) timer->stop();
    if (gravityTimer) gravityTimer->stop();
    delete timer;
    delete gravityTimer;

    scene->clear();
    delete scene;
}

