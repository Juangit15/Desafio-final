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
    void keyPressEvent(QKeyEvent *event) override;
    void setGravedad(qreal nuevaGravedad);
    bool estaMirandoDerecha() const;
    bool isOnPlatform(QGraphicsItem *platform) const;
    void setOnMovingPlatform(bool onPlatform, qreal platformVel = 0);
    qreal getPlatformVelocity() const { return platformVelocity; }
    qreal getVelocidadY() const { return velocidadY; }
    bool estaEmpujando() const { return empujando; }
    void setEmpujando(bool empujar) { empujando = empujar; actualizarSprite(); }
    void reiniciarEstado();


signals:
    void solicitarMenu();
    void enemigoEliminado(QGraphicsPixmapItem* enemigo);

private:
    bool mirandoDerecha;
    qreal velocidadY;
    qreal gravedad;
    bool enSalto;
    qreal platformVelocity;
    bool onMovingPlatform;
    void actualizarSprite();
    bool empujando;
};

#endif
