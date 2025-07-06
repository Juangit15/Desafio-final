#ifndef NIVEL3_H
#define NIVEL3_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include "jugador.h"

class Nivel3 : public QGraphicsView {
    Q_OBJECT

public:
    Nivel3(QWidget *parent = nullptr);
    ~Nivel3();

signals:
    void volverAlMenu();

private slots:
    void actualizar();
    void cambiarGravedad();

private:
    QGraphicsScene *scene;
    Jugador *goku;
    QTimer *timer;
    QTimer *gravityTimer;
    QVector<QGraphicsRectItem*> plataformas;
    QVector<QGraphicsItem*> sensores;
    qreal gravedadActual;
    bool enZonaGravedad;

    void reproducirAudioFinal();

    void setupScene();
    void crearPlataformas();
    void crearSensores();
    void verificarSensores();
};

#endif
