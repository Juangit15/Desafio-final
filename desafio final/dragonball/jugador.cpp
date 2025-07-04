#include "jugador.h"
#include <QPixmap>
#include <QTransform>

Jugador::Jugador() : mirandoDerecha(true),
    velocidadY(0),
    gravedad(0.5),
    enSalto(false),
    platformVelocity(0),
    onMovingPlatform(false)
{
    actualizarSprite();
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Jugador::setOnMovingPlatform(bool onPlatform, qreal platformVel) {
    onMovingPlatform = onPlatform;
    platformVelocity = platformVel;
}

bool Jugador::isOnPlatform(QGraphicsItem *platform) const {
    QRectF playerFeet = QRectF(
        x() + boundingRect().width() * 0.2,
        y() + boundingRect().height() - 5,
        boundingRect().width() * 0.6,
        5
        );

    QRectF platformTop = QRectF(
        platform->x(),
        platform->y(),
        platform->boundingRect().width(),
        10
        );

    return playerFeet.intersects(platformTop) && velocidadY >= 0;
}

void Jugador::actualizarSprite() {
    QString ruta;

    if (enSalto) {
        ruta = mirandoDerecha ? "C:/Users/juanm/Downloads/DragonBall/recursos/salto2g.png"
                              : "C:/Users/juanm/Downloads/DragonBall/recursos/salto2gL.png";
    }
    else if (empujando) {
        ruta = mirandoDerecha ? "C:/Users/juanm/Downloads/DragonBall/recursos/gokuempuje.png"
                              : "C:/Users/juanm/Downloads/DragonBall/recursos/gokuempujeL.png";
    }
    else {
        ruta = mirandoDerecha ? "C:/Users/juanm/Downloads/DragonBall/recursos/camina1g.png"
                              : "C:/Users/juanm/Downloads/DragonBall/recursos/camina1gL.png";
    }

    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(60, 60));
    } else {
        qDebug() << "Error: No se pudo cargar" << ruta;
    }
}

void Jugador::mover(const QVector<QGraphicsRectItem*>& plataformas) {
    // Aplicar movimiento de plataforma si corresponde
    if (onMovingPlatform && platformVelocity != 0) {
        moveBy(platformVelocity, 0);
    }

    // Resto de la lógica de movimiento...
    velocidadY += gravedad;
    moveBy(0, velocidadY);

    bool sobrePlataforma = false;
    for (QGraphicsRectItem* plataforma : plataformas) {
        if (isOnPlatform(plataforma)) {
            setY(plataforma->y() - boundingRect().height());
            velocidadY = 0;
            enSalto = false;
            sobrePlataforma = true;
            break;
        }
    }

    if (!sobrePlataforma) {
        enSalto = true;
    }
    actualizarSprite();
}

void Jugador::reiniciarEstado() {
    velocidadY = 0;
    enSalto = false;
    actualizarSprite();
}

void Jugador::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_A:
        moveBy(-10, 0);
        if (mirandoDerecha) {
            mirandoDerecha = false;
            actualizarSprite();
        }
        break;

    case Qt::Key_D:
        moveBy(10, 0);
        if (!mirandoDerecha) {
            mirandoDerecha = true;
            actualizarSprite();
        }
        break;

    case Qt::Key_Space:
        if (!enSalto) {
            velocidadY = -12;
            enSalto = true;
            actualizarSprite();
        }
        break;

    case Qt::Key_Escape:
        emit solicitarMenu();
        break;

    case Qt::Key_S:
        break;
    }
}

void Jugador::boostJump(qreal extraForce) {
    // Aplicar impulso adicional independientemente de si está saltando o no
    if (velocidadY >= 0) { // Solo si está cayendo o en el suelo
        velocidadY = -12.0 - extraForce;
        enSalto = true;
        actualizarSprite();
    }
}

void Jugador::setGravedad(qreal nuevaGravedad) {
    gravedad = nuevaGravedad;
}


bool Jugador::estaMirandoDerecha() const {
    return mirandoDerecha;
}

