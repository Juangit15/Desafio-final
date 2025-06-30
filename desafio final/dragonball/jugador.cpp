#include "jugador.h"
#include <QPixmap>

Jugador::Jugador() {
    setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/camina1g.png").scaled(60, 60));
    velocidadY = 0;
    gravedad = 0.5;
    enSalto = false;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
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
            setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/camina1g.png").scaled(60, 60));
            sobrePlataforma = true;
            break;
        }
    }

    if (!sobrePlataforma) enSalto = true;

    if (y() > 1600) {
        emit solicitarMenu();
    }
}

void Jugador::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_A)
        moveBy(-10, 0);
    else if (event->key() == Qt::Key_D)
        moveBy(10, 0);
    else if (event->key() == Qt::Key_Space && !enSalto) {
        velocidadY = -12;
        enSalto = true;
        setPixmap(QPixmap("C:/Users/juanm/Downloads/Desafio Final, Dragon Ball/recursos/salto2g.png").scaled(60, 60));
    }
    else if (event->key() == Qt::Key_Escape) {
        emit solicitarMenu();
    }
}
