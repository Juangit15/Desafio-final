#include "jugador.h"
#include <QPixmap>
#include <QTransform>

Jugador::Jugador() : mirandoDerecha(true) {
    actualizarSprite();
    velocidadY = 0;
    gravedad = 0.5;
    enSalto = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Jugador::actualizarSprite() {
    QString ruta;
    if (enSalto) {
        ruta = mirandoDerecha ? "C:/Users/juanm/Downloads/DragonBall/recursos/salto2g.png"
                              : "C:/Users/juanm/Downloads/DragonBall/recursos/salto2gL.png";
    } else {
        ruta = mirandoDerecha ? "C:/Users/juanm/Downloads/DragonBall/recursos/camina1g.png"
                              : "C:/Users/juanm/Downloads/DragonBall/recursos/camina1gL.png";
    }

    QPixmap pixmap(ruta);
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(60, 60));
    }
}

void Jugador::setGravedad(qreal nuevaGravedad) {
    gravedad = nuevaGravedad;
}

void Jugador::mover(const QVector<QGraphicsRectItem*>& plataformas) {
    velocidadY += gravedad;
    moveBy(0, velocidadY);

    bool sobrePlataforma = false;

    for (QGraphicsRectItem* plataforma : plataformas) {
        QRectF rectGoku = this->sceneBoundingRect();
        QRectF rectPlataforma = plataforma->sceneBoundingRect();

        bool sobreX = rectGoku.center().x() >= rectPlataforma.left() &&
                      rectGoku.center().x() <= rectPlataforma.right();

        bool tocaY = rectGoku.bottom() <= rectPlataforma.top() + 5 &&
                     rectGoku.bottom() >= rectPlataforma.top() - 5;

        if (sobreX && tocaY && velocidadY >= 0) {
            setY(plataforma->y() - boundingRect().height());
            velocidadY = 0;
            enSalto = false;
            actualizarSprite();
            sobrePlataforma = true;
            break;
        }
    }

    if (!sobrePlataforma) {
        enSalto = true;
        actualizarSprite();
    }

    // Verificar colisión con enemigos (solo si está saltando y cayendo)
    if (enSalto && velocidadY > 0) {
        QList<QGraphicsItem*> itemsColisionados = collidingItems();
        for (QGraphicsItem* item : itemsColisionados) {
            Enemigo* enemigo = dynamic_cast<Enemigo*>(item);
            if (enemigo) {
                // Emitir señal en lugar de eliminar directamente
                emit enemigoEliminado(enemigo);
                velocidadY = -8;
                break;
            }
        }
    }

    if (y() > 1600) {
        emit solicitarMenu();
    }
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
    }
}
