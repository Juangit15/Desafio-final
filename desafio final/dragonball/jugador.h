#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QKeyEvent>
#include <QVector>
#include <QGraphicsRectItem>

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Jugador();
    void mover(const QVector<QGraphicsRectItem*>& plataformas);
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void solicitarMenu();

private:
    qreal velocidadY;
    qreal gravedad;
    bool enSalto;
};

#endif
