#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT  // Necesario para señales/slots

public:
    Jugador();
    void mover();
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void volverAMenu();  // Señal para volver al menú al presionar ESC

private:
    qreal velocidadX, velocidadY;
    qreal gravedad;
    bool enSalto;
};

#endif
