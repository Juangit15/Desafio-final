#include "enemigo.h"
#include <QPixmap>

Enemigo::Enemigo() {
    setPixmap(QPixmap("C:/Users/juanm/Documents/GitHub/Desafio-final/desafio final/dragonball/recursos/enemi.png").scaled(50, 50));
    direccion = 1;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemigo::mover);
    timer->start(100);
}

void Enemigo::mover() {
    moveBy(direccion * 5, 0);
    if (x() < 50 || x() > 400) {
        direccion *= -1;
    }
}
