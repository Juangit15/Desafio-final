#ifndef NIVEL1_H
#define NIVEL1_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "jugador.h"

class Nivel1 : public QGraphicsView {
    Q_OBJECT
public:
    Nivel1(QWidget *parent = nullptr);
private:
    QGraphicsScene *scene;
    Jugador *goku;
    QTimer *timer;
private slots:
    void actualizar();
};

#endif // NIVEL1_H
