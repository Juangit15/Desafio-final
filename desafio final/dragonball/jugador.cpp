#include "jugador.h"
#include <QPixmap>
#include <QTransform>

Jugador::Jugador() : mirandoDerecha(true),
    velocidadY(0),
    gravedad(0.5),
    enSalto(false),
    empujando(false)
{
    actualizarSprite();
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

bool Jugador::isOnPlatform(QGraphicsItem *platform) const {
    QRectF feetRect = QRectF(
        x() + boundingRect().width() * 0.2,
        y() + boundingRect().height() - 1,
        boundingRect().width() * 0.6,
        2
        );

    QRectF platformRect = platform->sceneBoundingRect();
    QRectF platformTop = QRectF(
        platformRect.x(),
        platformRect.y(),
        platformRect.width(),
        5
        );

    return feetRect.intersects(platformTop) && velocidadY >= 0;
}

void Jugador::actualizarSprite() {
    QString ruta;

    if (enSalto) {
        ruta = mirandoDerecha ? "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/salto2g.png"
                              : "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/salto2gL.png";
    }
    else if (empujando) {
        ruta = mirandoDerecha ? "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/gokuempuje.png"
                              : "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/gokuempujeL.png";
    }
    else {
        ruta = mirandoDerecha ? "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/camina1g.png"
                              : "C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/camina1gL.png";
    }

    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(60, 60));
    } else {
        qDebug() << "Error: No se pudo cargar" << ruta;
    }
}

void Jugador::setEmpujando(bool estado) {
    empujando = estado;
    actualizarSprite();
}

void Jugador::mover(const QVector<QGraphicsRectItem*>& plataformas) {
    velocidadY += gravedad;
    moveBy(0, velocidadY);

    bool sobrePlataforma = false;

    QRectF gokuFeet = sceneBoundingRect();
    gokuFeet.setRect(
        gokuFeet.x() + gokuFeet.width() * 0.2,
        gokuFeet.y() + gokuFeet.height() - 5,
        gokuFeet.width() * 0.6,
        5
        );

    for (QGraphicsRectItem* plataforma : plataformas) {
        QRectF platTop = plataforma->sceneBoundingRect();
        platTop.setHeight(8);

        if (gokuFeet.intersects(platTop) && velocidadY >= 0) {
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

//  NUEVA FUNCIÓN SOLO PARA NIVEL 3
void Jugador::moverConGravedad(const QVector<QGraphicsRectItem*>& plataformas, qreal gravedadLocal) {
    // Aplicar gravedad
    velocidadY += gravedadLocal;
    moveBy(0, velocidadY);

    bool sobrePlataforma = false;
    QRectF playerRect = sceneBoundingRect();

    for (QGraphicsRectItem* plataforma : plataformas) {
        QRectF platformRect = plataforma->sceneBoundingRect();

        if (gravedadLocal > 0) { // Gravedad normal
            // pies goku
            QRectF feetArea(
                playerRect.left() + playerRect.width() * 0.2,
                playerRect.bottom() - 5,  // Pequeña área en los pies
                playerRect.width() * 0.6,
                10
                );

            // Parte superior de la plataforma
            QRectF platformTop(
                platformRect.left(),
                platformRect.top(),
                platformRect.width(),
                10  // Pequeña área en la parte superior
                );

            if (feetArea.intersects(platformTop) && velocidadY >= 0) {
                setY(platformRect.top() - boundingRect().height());
                velocidadY = 0;
                enSalto = false;
                sobrePlataforma = true;
                break;
            }
        }
        else { // Gravedad invertida
            // Área de cabeza (parte superior del jugador)
            QRectF headArea(
                playerRect.left() + playerRect.width() * 0.2,
                playerRect.top(),
                playerRect.width() * 0.6,
                10
                );

            // Parte inferior de la plataforma
            QRectF platformBottom(
                platformRect.left(),
                platformRect.bottom() - 10,
                platformRect.width(),
                10
                );

            if (headArea.intersects(platformBottom) && velocidadY <= 0) {
                setY(platformRect.bottom());
                velocidadY = 0;
                enSalto = false;
                sobrePlataforma = true;
                break;
            }
        }
    }

    if (!sobrePlataforma) {
        enSalto = true;
    }

    // Rotación visual cuando está en gravedad invertida
    if (gravedadLocal < 0) {
        setTransformOriginPoint(boundingRect().center());
        setRotation(180);
    } else {
        setRotation(0);
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
            if (gravedad < 0) {
                velocidadY = +12; // salto "hacia abajo" en gravedad invertida
            } else {
                velocidadY = -12; // salto normal hacia arriba
            }
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
    if (velocidadY >= 0) {
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
