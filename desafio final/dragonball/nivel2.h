#ifndef NIVEL2_H
#define NIVEL2_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include "jugador.h"

class Nivel2 : public QGraphicsView {
    Q_OBJECT

public:
    Nivel2(QWidget *parent = nullptr);
    ~Nivel2();

signals:
    void solicitarMenu();
    void nivelCompletado();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void actualizar();
    void moverPlataformas();

private:
    void setupScene();
    void crearPlataformas();
    void crearObstaculos();
    void aplicarGravedadObjetos();
    void verificarPuzzle();
    void resetLevel();
    void volverAlMenu();

    QGraphicsScene *scene;
    Jugador *goku;
    QTimer *timer;
    QTimer *platformTimer;
    qreal tiempo;
    bool puzzleResuelto;
    bool modoEmpuje;

    QVector<QGraphicsRectItem*> plataformas;
    QVector<qreal> plataformasVelocidades;
    QVector<QGraphicsItem*> obstaculos;
};

#endif
