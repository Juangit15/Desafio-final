#include "nivel1.h"
#include "mainmenu.h"
#include "enemigo.h"
#include "nivel2.h"
#include <QGraphicsPixmapItem>



Nivel1::Nivel1(QWidget *parent) : QGraphicsView(parent), menuMostrado(false) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(610, 960);
    scene->setSceneRect(0, 0, 610, 960);

    QPixmap fondoPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/torre.png");
    QGraphicsPixmapItem *fondo = new QGraphicsPixmapItem(fondoPixmap.scaled(600, 951));
    fondo->setZValue(-10);
    scene->addItem(fondo);

    for (int i = 0; i < plataformasTotales; ++i) {
        int x = 100 + (i % 3) * 120; // más variado: 100, 220, 340
        int y = 800 - i * 100;       // más separadas verticalmente
        QGraphicsRectItem *plat = new QGraphicsRectItem(x, y, 150, 20);
        plat->setPos(x, y);
        plat->setBrush(Qt::darkGray);
        scene->addItem(plat);
        plataformas.append(plat);
    }

    goku = new Jugador();
    goku->setZValue(2);
    scene->addItem(goku);
    goku->setPos(plataformas[0]->x() + 20, plataformas[0]->y() - goku->boundingRect().height());
    connect(goku, &Jugador::solicitarMenu, this, &Nivel1::volverAlMenu);

    enemigo = new Enemigo();
    enemigo->setZValue(2);
    scene->addItem(enemigo);
    enemigo->setPos(plataformas[3]->x() + 60, plataformas[3]->y() - enemigo->boundingRect().height());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);
    timer->start(30);
}

void Nivel1::actualizar() {
    goku->mover(plataformas);

    if (goku->y() < plataformas.last()->y() - 200) {
        for (QGraphicsItem *item : scene->items()) {
            if (item != goku) item->moveBy(0, 2);
        }
    }

    for (QGraphicsItem* item : goku->collidingItems()) {
        if (item == enemigo) {
            volverAlMenu();
            return;
        }
    }

    if (goku->y() < plataformas.last()->y() - 40) {
        timer->stop();
        Nivel2 *nivel2 = new Nivel2();
        nivel2->show();
        this->close();
    }
}

void Nivel1::volverAlMenu() {
    if (!menuMostrado) {
        menuMostrado = true;
        MainMenu *menu = new MainMenu();
        menu->show();
        this->close();
    }
}
