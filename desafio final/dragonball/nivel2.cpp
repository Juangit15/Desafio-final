#include "nivel2.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QRandomGenerator>

Nivel2::Nivel2(QWidget *parent) : QGraphicsView(parent), tiempo(0), puzzleResuelto(false), modoEmpuje(false) {
    std::srand(std::time(0));
    setupScene();
    connect(goku, &Jugador::solicitarMenu, this, &Nivel2::volverAlMenu);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void Nivel2::setupScene() {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(1000, 700);
    scene->setSceneRect(0, 0, 1000, 700);

    // Fondo
    QPixmap fondo("C:/Users/juanm/Downloads/DragonBall/recursos/background2.jpg");
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(fondo.scaled(1000, 700));
    background->setZValue(-1);
    scene->addItem(background);

    // Mr. Popo
    QGraphicsPixmapItem *popo = new QGraphicsPixmapItem(QPixmap("C:/Users/juanm/Downloads/DragonBall/recursos/mrpopo.png").scaled(120, 120));
    popo->setPos(800, 500);
    scene->addItem(popo);

    crearPlataformas();

    // Jugador
    goku = new Jugador();
    scene->addItem(goku);
    goku->setPos(150, plataformas[0]->y() - goku->boundingRect().height());

    crearObstaculos();

    // Timers
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);
    timer->start(16);

    platformTimer = new QTimer(this);
    connect(platformTimer, &QTimer::timeout, this, &Nivel2::moverPlataformas);
    platformTimer->start(30);
}

void Nivel2::crearPlataformas() {
    // Plataforma base
    QGraphicsRectItem *base = new QGraphicsRectItem(0, 0, 300, 30);
    base->setPos(50, 600);
    base->setBrush(QColor(139, 69, 19));
    scene->addItem(base);
    plataformas.append(base);

    // Trampolín
    QGraphicsRectItem *trampolin = new QGraphicsRectItem(0, 0, 100, 20);
    trampolin->setPos(base->x() + base->rect().width(), base->y() + 10);
    trampolin->setBrush(Qt::red);
    scene->addItem(trampolin);
    plataformas.append(trampolin);

    // Plataforma móvil horizontal
    QGraphicsRectItem *platHor = new QGraphicsRectItem(0, 0, 200, 30);
    platHor->setPos(300, 400);
    platHor->setBrush(Qt::lightGray);
    scene->addItem(platHor);
    plataformas.append(platHor);
    plataformasVelocidades.append(1.5);

    // Plataforma móvil vertical
    QGraphicsRectItem *platVer = new QGraphicsRectItem(0, 0, 150, 25);
    platVer->setPos(600, 300);
    platVer->setBrush(Qt::lightGray);
    scene->addItem(platVer);
    plataformas.append(platVer);
    plataformasVelocidades.append(-1.2);

    // Plataforma final
    QGraphicsRectItem *final = new QGraphicsRectItem(0, 0, 200, 30);
    final->setPos(700, 150);
    final->setBrush(Qt::gray);
    scene->addItem(final);
    plataformas.append(final);
}

void Nivel2::crearObstaculos() {
    QPixmap bloqueTex("C:/Users/juanm/Downloads/DragonBall/recursos/objetomr.png");
    qreal yBase = plataformas[0]->y() - 40;

    for (int i = 0; i < 3; i++) {
        QGraphicsPixmapItem *bloque = new QGraphicsPixmapItem(bloqueTex.scaled(40, 40));
        bloque->setPos(120 + i * 80, yBase);
        bloque->setData(0, "empujable");
        bloque->setData(1, 0.0); // Velocidad Y
        bloque->setData(2, false); // En suelo
        scene->addItem(bloque);
        obstaculos.append(bloque);
    }

    // Zona objetivo
    QGraphicsEllipseItem *target = new QGraphicsEllipseItem(0, 0, 120, 120);
    target->setPos(650, 100);
    target->setBrush(QColor(0, 255, 0, 100));
    scene->addItem(target);
    obstaculos.append(target);
}

void Nivel2::moverPlataformas() {
    if (plataformas.size() > 2 && plataformasVelocidades.size() > 0) {
        QGraphicsRectItem *platHor = plataformas[2];
        qreal velHor = plataformasVelocidades[0];

        if (platHor->x() <= 200 || platHor->x() >= 600) {
            plataformasVelocidades[0] *= -1;
            velHor = plataformasVelocidades[0];
        }
        platHor->moveBy(velHor, 0);
    }

    if (plataformas.size() > 3 && plataformasVelocidades.size() > 1) {
        QGraphicsRectItem *platVer = plataformas[3];
        qreal velVer = plataformasVelocidades[1];

        if (platVer->y() <= 200 || platVer->y() >= 400) {
            plataformasVelocidades[1] *= -1;
            velVer = plataformasVelocidades[1];
        }
        platVer->moveBy(0, velVer);
    }
}

void Nivel2::aplicarGravedadObjetos() {
    const qreal GRAVEDAD_OBJETOS = 0.3;

    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        QGraphicsItem *objeto = obstaculos[i];
        qreal velY = objeto->data(1).toReal();
        bool enAire = true;

        velY += GRAVEDAD_OBJETOS;
        objeto->setY(objeto->y() + velY);

        for (QGraphicsItem *plat : plataformas) {
            if (objeto->collidesWithItem(plat)) {
                QRectF rectObj = objeto->boundingRect();

                if (objeto->y() + rectObj.height() <= plat->y() + 5 && velY > 0) {
                    objeto->setY(plat->y() - rectObj.height());
                    velY = -velY * 0;
                    if (qAbs(velY) < 0.5) velY = 0;
                    enAire = false;
                }
            }
        }

        objeto->setData(1, velY);
        objeto->setData(2, enAire);
    }
}

void Nivel2::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        modoEmpuje = true;
        QString empujeSprite = goku->estaMirandoDerecha() ?
                                   "C:/Users/juanm/Downloads/DragonBall/recursos/gokuempuje.png" :
                                   "C:/Users/juanm/Downloads/DragonBall/recursos/gokuempujeL.png";
        goku->setPixmap(QPixmap(empujeSprite).scaled(60, 60));
        return;
    }

    if (modoEmpuje && (event->key() == Qt::Key_A || event->key() == Qt::Key_D)) {
        for (auto item : goku->collidingItems()) {
            if (item->data(0) == "empujable" && !item->data(2).toBool()) {
                qreal dx = (event->key() == Qt::Key_D) ? 3.0 : -3.0;
                item->moveBy(dx, 0);

                if (QRandomGenerator::global()->bounded(100) < 30) {
                    item->setData(1, -2.0);
                    item->setData(2, true);
                }
            }
        }
    }

    QGraphicsView::keyPressEvent(event);
}

void Nivel2::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        modoEmpuje = false;
        QString spriteNormal = goku->estaMirandoDerecha() ?
                                   "C:/Users/juanm/Downloads/DragonBall/recursos/camina1g.png" :
                                   "C:/Users/juanm/Downloads/DragonBall/recursos/camina1gL.png";
        goku->setPixmap(QPixmap(spriteNormal).scaled(60, 60));
    }
}

void Nivel2::actualizar() {
    tiempo += 0.1;
    goku->mover(plataformas);
    qreal velocidadPlataforma = plataformasVelocidades[0];

    // Trampolín
    if (goku->collidesWithItem(plataformas[1]) && goku->isOnPlatform(plataformas[1])) {
        goku->boostJump(8.0);   // salto goku
    }

    // Objetos en trampolín
    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        if (obstaculos[i]->collidesWithItem(plataformas[1])) {
            obstaculos[i]->setData(1, -20.0);
        }
    }

    verificarPuzzle();

    if (goku->y() > scene->height()) {
        resetLevel();
    }

    // Mover Goku con la plataforma si está sobre ella
    if (goku->isOnPlatform(plataformas[2])) {
        goku->setX(goku->x() + velocidadPlataforma);
    }

    // Mover objetos con la plataforma si están sobre ella
    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        QGraphicsItem *objeto = obstaculos[i];
        if (objeto->collidesWithItem(plataformas[2]) && !objeto->data(2).toBool()) {
            objeto->setX(objeto->x() + velocidadPlataforma);
        }
    }

    aplicarGravedadObjetos();  // Gravedad para objetos (sin rebote)
    verificarPuzzle();
}

void Nivel2::verificarPuzzle() {
    if (puzzleResuelto) return;

    int bloquesEnZona = 0;
    QGraphicsItem *target = obstaculos.last();

    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        if (obstaculos[i]->collidesWithItem(target)) {
            bloquesEnZona++;
        }
    }

    if (bloquesEnZona >= 2) {
        puzzleResuelto = true;
        plataformas.last()->setBrush(Qt::green);
        QMessageBox::information(this, "¡Puzzle completado!", "Mr. Popo: ¡Bien hecho, Goku!");
        emit nivelCompletado();
    }
}

void Nivel2::resetLevel() {
    goku->setPos(150, plataformas[0]->y() - goku->boundingRect().height());
    goku->reiniciarEstado();

    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        obstaculos[i]->setPos(120 + i * 80, plataformas[0]->y() - 40);
        obstaculos[i]->setData(1, 0.0);
        obstaculos[i]->setData(2, false);
    }
}

void Nivel2::volverAlMenu() {
    emit solicitarMenu();
}

Nivel2::~Nivel2() {
    delete timer;
    delete platformTimer;
    delete scene;
}
