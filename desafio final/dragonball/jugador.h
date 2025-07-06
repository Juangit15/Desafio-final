#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QVector>
#include <QGraphicsRectItem>

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Jugador();
    void boostJump(qreal extraHeight);
    void mover(const QVector<QGraphicsRectItem*>& plataformas);
    void moverConGravedad(const QVector<QGraphicsRectItem*>& plataformas, qreal gravedadLocal);
    void keyPressEvent(QKeyEvent *event) override;
    void setGravedad(qreal nuevaGravedad);
    bool estaMirandoDerecha() const;
    bool isOnPlatform(QGraphicsItem *platform) const;
    qreal getVelocidadY() const { return velocidadY; }
    bool estaEmpujando() const { return empujando; }
    void setEmpujando(bool empujar);
    void reiniciarEstado();


signals:
    void solicitarMenu();
    void enemigoEliminado(QGraphicsPixmapItem* enemigo);

private:
    bool mirandoDerecha;
    qreal velocidadY;
    qreal gravedad;
    bool enSalto;
    void actualizarSprite();
    bool empujando = false;
};

#endif
