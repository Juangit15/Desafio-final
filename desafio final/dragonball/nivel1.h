#ifndef NIVEL1_H
#define NIVEL1_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include <QGraphicsRectItem>
#include "jugador.h"
#include "enemigo.h"

class Nivel1 : public QGraphicsView {
    Q_OBJECT
public:
    Nivel1(QWidget *parent = nullptr);

private slots:
    void actualizar();
    void volverAlMenu();

private:
    QGraphicsScene *scene;
    Jugador *goku;
    QTimer *timer;
    Enemigo *enemigo;
    QVector<QGraphicsRectItem*> plataformas;
    int plataformasTotales = 8;
    bool menuMostrado;
};

#endif
