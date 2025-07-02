#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QVector>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "enemigo.h"

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Jugador();
    bool isOnPlatform(QGraphicsItem *platform) const;
    void boostJump(qreal extraHeight);
    void mover(const QVector<QGraphicsRectItem*>& plataformas);
    void keyPressEvent(QKeyEvent *event) override;
    void setGravedad(qreal nuevaGravedad);
    void reiniciarEstado();
    bool enCaida() const;
    bool estaMirandoDerecha() const { return mirandoDerecha; }

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

#endif
