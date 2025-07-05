#include "nivel2.h"
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>

const qreal VELOCIDAD_EMPUJE = 3.0;


Nivel2::Nivel2(QWidget *parent) : QGraphicsView(parent), puzzleResuelto(false), modoEmpuje(false), trampolinActivado(false) {
    setupScene();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    retornarPlataforma = false;
    plataformaSubio = false;
}

void Nivel2::setupScene() {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(1000, 700);
    scene->setSceneRect(0, 0, 990, 690);

    // Fondo
    QPixmap fondo("C:/Users/juanm/Downloads/DragonBall/recursos/background2.jpg");
    if(!fondo.isNull()) {
        QGraphicsPixmapItem *background = new QGraphicsPixmapItem(fondo.scaled(990, 690));
        background->setZValue(-1);
        scene->addItem(background);
    } else {
        qDebug() << "Error: No se pudo cargar la imagen de fondo";
        scene->setBackgroundBrush(Qt::darkBlue);
    }

    // Mr. Popo
    QPixmap popoImg("C:/Users/juanm/Downloads/DragonBall/recursos/mrpopo.png");
    if(!popoImg.isNull()) {
        QGraphicsPixmapItem *popo = new QGraphicsPixmapItem(popoImg.scaled(140, 120));
        popo->setPos(800, 500);
        scene->addItem(popo);
    }

    crearPlataformas();
    crearObstaculos();

    // Jugador
    goku = new Jugador();
    scene->addItem(goku);
    goku->setPos(150, plataformas[0]->y() - goku->boundingRect().height());

    // Timers
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel2::actualizar);
    timer->start(16);

    platformTimer = new QTimer(this);
    connect(platformTimer, &QTimer::timeout, this, &Nivel2::moverPlataformas);
    platformTimer->start(30);

    connect(goku, &Jugador::solicitarMenu, this, &Nivel2::volverAlMenu);

}

Nivel2::~Nivel2() {
    if (timer) {
        timer->stop();
        delete timer;
    }

    if (platformTimer) {
        platformTimer->stop();
        delete platformTimer;
    }

    scene->clear();
    delete scene;
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
    platVer->setPos(600, 550);
    platVer->setBrush(Qt::lightGray);
    scene->addItem(platVer);
    plataformas.append(platVer);
    plataformasVelocidades.append(-1.2);
    // Guardar posición inicial
    posInicialPlataformaVer = platVer->y();

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
    target->setPos(790, 80);
    target->setBrush(QColor(0, 255, 0, 100));
    scene->addItem(target);
    obstaculos.append(target);
}

void Nivel2::moverPlataformas() {
    //plataforma horizontal, rango movimiento
    if (plataformas.size() > 2 && plataformasVelocidades.size() > 0) {
        QGraphicsRectItem *platHor = plataformas[2];
        qreal velHor = plataformasVelocidades[0];

        if (platHor->x() <= 200 || platHor->x() >= 600) {
            plataformasVelocidades[0] *= -1;
            velHor = plataformasVelocidades[0];
        }
        platHor->moveBy(velHor, 0);
    }

    // Plataforma vertical
    if (plataformas.size() > 3 && plataformasVelocidades.size() > 1) {
        QGraphicsRectItem *platVer = plataformas[3];
        qreal velVer = plataformasVelocidades[1];

        bool gokuSobre = goku->isOnPlatform(platVer);
        bool objetoSobre = false;
        QList<QGraphicsItem*> objetosSobre;

        // Verificar si algún objeto está encima
        for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
            QGraphicsItem* objeto = obstaculos[i];
            QRectF objetoBase = objeto->boundingRect();
            objetoBase.moveTo(objeto->pos());
            objetoBase.setTop(objetoBase.bottom() - 5);

            QRectF zonaPlataforma(platVer->x(), platVer->y(), platVer->boundingRect().width(), 5);

            if (objetoBase.intersects(zonaPlataforma)) {
                objetoSobre = true;
                objetosSobre.append(objeto);
            }
        }

        // SUBE si hay algo encima
        if ((gokuSobre || objetoSobre) && platVer->y() > 120) {
            plataformaSubio = true;
            platVer->moveBy(0, velVer); // velVer < 0
            for (QGraphicsItem* obj : objetosSobre) obj->moveBy(0, velVer);
            if (gokuSobre) goku->moveBy(0, velVer);
            retornarPlataforma = false;
        }

        // RETORNA si subió antes y ya no hay nada encima
        else if (plataformaSubio && !gokuSobre && !objetoSobre) {
            retornarPlataforma = true;
        }

        if (retornarPlataforma) {
            qreal distancia = posInicialPlataformaVer - platVer->y();
            if (qAbs(distancia) > 1) {
                qreal direccion = (distancia > 0) ? 1 : -1;
                platVer->moveBy(0, direccion * 1.2);
            } else {
                retornarPlataforma = false;
                plataformaSubio = false;
                platVer->setY(posInicialPlataformaVer); // Corrige posición final
            }
        }
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

        QRectF rectObjScene = objeto->sceneBoundingRect();

        for (QGraphicsItem *plat : plataformas) {
            QRectF rectPlatScene = plat->sceneBoundingRect();

            // Verifica si el objeto está justo sobre la plataforma
            if (rectObjScene.intersects(rectPlatScene)) {
                if (rectObjScene.bottom() >= rectPlatScene.top() - 5 &&
                    rectObjScene.bottom() <= rectPlatScene.top() + 10 &&
                    velY > 0) {

                    objeto->setY(plat->y() - objeto->boundingRect().height());
                    velY = 0;
                    enAire = false;
                    break; // Salir del bucle si ya está sobre una plataforma
                }
            }
        }

        objeto->setData(1, velY);
        objeto->setData(2, !enAire);
    }
}

void Nivel2::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        volverAlMenu();
        return;
    }

    if (event->key() == Qt::Key_S) {
        if (!modoEmpuje) {
            modoEmpuje = true;
            goku->setEmpujando(true);
        }
        return;
    }

    if (modoEmpuje && (event->key() == Qt::Key_A || event->key() == Qt::Key_D)) {
        bool mirandoDerecha = (event->key() == Qt::Key_D);
        if (mirandoDerecha != goku->estaMirandoDerecha()) {
            // Cambia orientación
            goku->keyPressEvent(event); // Para actualizar dirección
        }

        for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
            QGraphicsItem* item = obstaculos[i];
            if (item->data(0).toString() == "empujable" && goku->collidesWithItem(item)) {
                qreal dx = mirandoDerecha ? VELOCIDAD_EMPUJE : -VELOCIDAD_EMPUJE;
                item->moveBy(dx, 0);
            }
        }
        return;
    }

    // Si no está en modo empuje, se mueve normal
    if (goku) {
        goku->keyPressEvent(event);
    }

    QGraphicsView::keyPressEvent(event);
}


void Nivel2::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_S) {
        modoEmpuje = false;
        goku->setEmpujando(false);
    }

    QGraphicsView::keyReleaseEvent(event);
}


void Nivel2::actualizar() {
    // Mover Goku
    goku->mover(plataformas);

    for (QGraphicsRectItem* plataforma : plataformas) {
        if (goku->collidesWithItem(plataforma)) {
            QRectF gokuRect = goku->sceneBoundingRect();
            QRectF platRect = plataforma->sceneBoundingRect();
            if (gokuRect.bottom() >= platRect.top() &&
                gokuRect.bottom() <= platRect.top() + 10 &&
                goku->getVelocidadY() >= 0) {
                goku->setY(plataforma->y() - goku->boundingRect().height());
                goku->reiniciarEstado();
                break;
            }
        }
    }

    QGraphicsRectItem* trampolin = plataformas[1];

    // Crear zona de activación del trampolín (más ancha que el trampolín visual)
    QRectF trampolinZone = trampolin->sceneBoundingRect();
    trampolinZone.adjust(-5, 0, 5, 0); // Extender ligeramente los lados
    trampolinZone.setTop(trampolinZone.top());
    trampolinZone.setHeight(15); // Zona de activación más gruesa

    // TRAMPOLÍN PARA GOKU
    QRectF gokuFeet = QRectF(
        goku->x() + goku->boundingRect().width() * 0.2,
        goku->y() + goku->boundingRect().height() - 10, // Ajustar posición pies
        goku->boundingRect().width() * 0.6,
        10
        );

    // Activar salto si Goku está cayendo y sus pies intersectan la zona del trampolín
    if (gokuFeet.intersects(trampolinZone)) {
        if (goku->getVelocidadY() > -1) {
            goku->boostJump(10.0); // Fuerza de salto más fuerte
        }
    }

    // TRAMPOLÍN PARA OBJETOS
    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        QGraphicsItem* objeto = obstaculos[i];
        QRectF objFeet = objeto->boundingRect();
        objFeet.moveTo(objeto->pos());
        objFeet.setTop(objFeet.bottom() - 10); // Parte inferior del objeto

        if (objFeet.intersects(trampolinZone)) {
            qreal velY = objeto->data(1).toReal();
            if (velY > -1) {
                objeto->setData(1, -15.0); // Impulso más fuerte hacia arriba
            }
        }
    }


    // --- Empuje de bloques ---
    if (modoEmpuje) {
        for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
            QGraphicsItem* item = obstaculos[i];
            if (item->data(0).toString() == "empujable" && goku->collidesWithItem(item)) {
                qreal dx = goku->estaMirandoDerecha() ? VELOCIDAD_EMPUJE : -VELOCIDAD_EMPUJE;
                item->moveBy(dx, 0);
            }
        }
    }

    aplicarGravedadObjetos();
    verificarPuzzle();

    if (goku->y() > scene->height()) {
        resetLevel();
    }
}


void Nivel2::empujarObjetos() {
    foreach(QGraphicsItem* item, obstaculos) {
        if (item->data(0).toString() == "empujable" &&
            goku->collidesWithItem(item)) {

            qreal pushForce = goku->estaMirandoDerecha() ? VELOCIDAD_EMPUJE : -VELOCIDAD_EMPUJE;

            // Verificamos que Goku esté en el lado correcto
            QRectF gokuRect = goku->sceneBoundingRect();
            QRectF objRect = item->sceneBoundingRect();

            bool puedeEmpujar =
                (goku->estaMirandoDerecha() && gokuRect.right() <= objRect.left() + 5) ||
                (!goku->estaMirandoDerecha() && gokuRect.left() >= objRect.right() - 5);

            if (puedeEmpujar) {
                item->setX(item->x() + pushForce);
                goku->moveBy(pushForce, 0); // Goku se mueve junto al objeto
            }
        }
    }
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

    modoEmpuje = false;
    QString spriteNormal = goku->estaMirandoDerecha() ?
                               "C:/Users/juanm/Downloads/DragonBall/recursos/camina1g.png" :
                               "C:/Users/juanm/Downloads/DragonBall/recursos/camina1gL.png";
    goku->setPixmap(QPixmap(spriteNormal).scaled(60, 60));

    for (int i = 0; i < 3 && i < obstaculos.size(); i++) {
        obstaculos[i]->setPos(120 + i * 80, plataformas[0]->y() - 40);
        obstaculos[i]->setData(1, 0.0);
        obstaculos[i]->setData(2, false);
    }
}

void Nivel2::volverAlMenu() {
    emit solicitarMenu();
}
