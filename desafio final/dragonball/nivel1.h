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
    ~Nivel1();

signals:
    void solicitarMenu();
    void nivelCompletado();

private slots:
    void actualizar();

private:
    void volverAlMenu();

    QGraphicsScene *scene;
    Jugador *goku;
    QTimer *timer;
    Enemigo *enemigo = nullptr; // Solo una declaración, inicializado como nullptr
    QVector<QGraphicsRectItem*> plataformas;
    int plataformasTotales = 7;
    bool menuMostrado;
};

#endif // NIVEL1_H
