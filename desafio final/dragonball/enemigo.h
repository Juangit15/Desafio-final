#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Enemigo : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Enemigo();

private slots:
    void mover();

private:
    QTimer *timer;
    int direccion;
};

#endif
