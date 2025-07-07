#include "nivel1.h"
#include <QGraphicsPixmapItem>
#include <QDebug>


Nivel1::Nivel1(QWidget *parent) : QGraphicsView(parent), menuMostrado(false) {
    QPixmap fondoPixmap("C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/torre.png");
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());
    setScene(scene);
    setFixedSize(fondoPixmap.size());

    QGraphicsPixmapItem *fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setPos(0, 0);
    fondo->setZValue(-10);
    scene->addItem(fondo);

    QPixmap nubeTexture("C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/nube.png");
    if (nubeTexture.isNull()) {
        nubeTexture = QPixmap(200, 20);
        nubeTexture.fill(Qt::lightGray);
    }

    for (int i = 0; i < 7; ++i) {
        int x = (i % 2 == 0) ? 100 : fondoPixmap.width() - 300;
        int y = fondoPixmap.height() - 150 - i * 80;

        QGraphicsRectItem *platFisica = new QGraphicsRectItem(0, 0, 200, 20);
        platFisica->setPos(x, y);
        platFisica->setBrush(Qt::NoBrush);
        platFisica->setPen(Qt::NoPen);
        scene->addItem(platFisica);
        plataformas.append(platFisica);

        QGraphicsPixmapItem *nube = new QGraphicsPixmapItem(nubeTexture.scaled(200, 40));
        nube->setPos(x, y - 10);
        nube->setZValue(1);
        scene->addItem(nube);
    }

    goku = new Jugador();
    goku->setZValue(2);
    scene->addItem(goku);
    goku->setPos(plataformas[0]->x() + 20, plataformas[0]->y() - goku->boundingRect().height());
    connect(goku, &Jugador::solicitarMenu, this, &Nivel1::volverAlMenu);

    enemigo = new Enemigo();
    enemigo->setZValue(2);
    scene->addItem(enemigo);
    enemigo->setPos(fondoPixmap.width() - 200, plataformas[4]->y() - enemigo->boundingRect().height());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);
    timer->start(30);
}

Nivel1::~Nivel1() {
    // Limpiar memoria
    if (timer) {
        timer->stop();
        delete timer;
    }

    if (enemigo) {
        scene->removeItem(enemigo);
        delete enemigo;
    }

    if (goku) {
        scene->removeItem(goku);
        delete goku;
    }

    // Limpiar plataformas
    qDeleteAll(plataformas);
    plataformas.clear();

    // eliminar la escena
    delete scene;
}

void Nivel1::actualizar() {
    goku->mover(plataformas);

    if (goku->y() < plataformas.last()->y() - 200) {
        for (QGraphicsItem *item : scene->items()) {
            if (item != goku) item->moveBy(0, 2);
        }
    }

    if (enemigo && goku->collidesWithItem(enemigo)) {
        if (goku->y() + goku->boundingRect().height() < enemigo->y() + enemigo->boundingRect().height()) {
            scene->removeItem(enemigo);
            delete enemigo;
            enemigo = nullptr;
        } else {
            volverAlMenu();
            return;
        }
    }

    if (goku->y() > scene->height()) {
        volverAlMenu();
        return;
    }

    if (goku->y() + goku->boundingRect().height() < plataformas.last()->y() &&
        goku->x() + goku->boundingRect().width() > plataformas.last()->x() &&
        goku->x() < plataformas.last()->x() + plataformas.last()->boundingRect().width()) {
        timer->stop();
        emit nivelCompletado();
    }
}

void Nivel1::volverAlMenu() {
    if (!menuMostrado) {
        menuMostrado = true;
        emit solicitarMenu();
    }
}
