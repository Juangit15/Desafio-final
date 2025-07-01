#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QVector>
#include <QGraphicsRectItem>
#include <QGraphicsScene> // Añadir esta línea
#include "enemigo.h"

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Jugador();
    void mover(const QVector<QGraphicsRectItem*>& plataformas);
    void keyPressEvent(QKeyEvent *event) override;
    void setGravedad(qreal nuevaGravedad);

signals:
    void solicitarMenu();
    void enemigoEliminado(Enemigo* enemigo); // Nueva señal

private:
    qreal velocidadY;
    qreal gravedad;
    bool enSalto;
    bool mirandoDerecha;
    void actualizarSprite();
};

#endif // JUGADOR_H
