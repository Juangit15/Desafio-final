#include "jugador.h"
#include <QPixmap>
#include <QKeyEvent>

Jugador::Jugador() {
    setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/camina1g.png").scaled(60, 60));
    velocidadX = 0;
    velocidadY = 0;
    gravedad = 0.5;
    enSalto = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void Jugador::mover() {
    if (enSalto) {
        velocidadY += gravedad;
        moveBy(velocidadX, velocidadY);
        if (y() > 400) {
            enSalto = false;
            setY(400);
            velocidadY = 0;
            setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/camina1g.png").scaled(60, 60));
        }
    }
}

void Jugador::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_A)
        moveBy(-10, 0);
    else if (event->key() == Qt::Key_D)
        moveBy(10, 0);
    else if (event->key() == Qt::Key_Space && !enSalto) {
        velocidadY = -10;
        enSalto = true;
        setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/salto2g.png").scaled(60, 60));
    }
    else if (event->key() == Qt::Key_Escape) {
        emit volverAMenu();
    }
}
