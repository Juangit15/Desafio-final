#include "nivel1.h"
#include <QGraphicsPixmapItem>
#include <QDebug>

Nivel1::Nivel1(QWidget *parent) : QGraphicsView(parent), menuMostrado(false) {
    QPixmap fondoPixmap("C:/Users/juanm/Downloads/DragonBall/recursos/torre.png");
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, fondoPixmap.width(), fondoPixmap.height());
    setScene(scene);
    setFixedSize(fondoPixmap.size());

    // Configurar vista para que coincida con el tamaño del fondo
    setFixedSize(fondoPixmap.size());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Agregar fondo
    QGraphicsPixmapItem *fondo = new QGraphicsPixmapItem(fondoPixmap);
    fondo->setPos(0, 0);
    fondo->setZValue(-10);
    scene->addItem(fondo);

    // Precargar textura de nube
    QPixmap nubeTexture("C:/Users/juanm/Downloads/DragonBall/recursos/nube.png");
    if (nubeTexture.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de nube";
        nubeTexture = QPixmap(200, 20); // Crear una nube por defecto si falla la carga
        nubeTexture.fill(Qt::lightGray);
    }

    // Crear plataformas con apariencia de nubes
    for (int i = 0; i < plataformasTotales; ++i) {
        int x = (i % 2 == 0) ? 100 : fondoPixmap.width() - 300;
        int y = fondoPixmap.height() - 150 - i * 80;

        // Crear plataforma física (invisible)
        QGraphicsRectItem *platFisica = new QGraphicsRectItem(0, 0, 200, 20);
        platFisica->setPos(x, y);
        platFisica->setBrush(Qt::NoBrush);
        platFisica->setPen(Qt::NoPen);
        scene->addItem(platFisica);
        plataformas.append(platFisica);

        // Crear nube visual
        QGraphicsPixmapItem *nube = new QGraphicsPixmapItem(nubeTexture.scaled(200, 40));
        nube->setPos(x, y - 10); // Ajustar posición para que la nube cubra la plataforma
        nube->setZValue(1); // Encima del fondo pero debajo de personajes
        scene->addItem(nube);
    }

    // Configurar a Goku
    goku = new Jugador();
    goku->setZValue(2);
    scene->addItem(goku);
    // Posicionar a Goku sobre la primera plataforma física
    goku->setPos(plataformas[0]->x() + 20, plataformas[0]->y() - goku->boundingRect().height());
    connect(goku, &Jugador::solicitarMenu, this, &Nivel1::volverAlMenu);

    // Configurar enemigo
    enemigo = new Enemigo();
    enemigo->setZValue(2);
    scene->addItem(enemigo);
    enemigo->setPos(fondoPixmap.width() - 200, plataformas[4]->y() - enemigo->boundingRect().height());

    // Configurar timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Nivel1::actualizar);
    timer->start(30);

    // Conexión para eliminar enemigos
    connect(goku, &Jugador::enemigoEliminado, this, [this](Enemigo* enemigoEliminar) {
        if (this->enemigo == enemigoEliminar) {
            this->scene->removeItem(enemigoEliminar);
            delete enemigoEliminar;
            this->enemigo = nullptr;
        }
    });
}

void Nivel1::actualizar() {
    goku->mover(plataformas);

    // Mover el escenario hacia arriba si Goku está saltando alto
    if (goku->y() < plataformas.last()->y() - 200) {
        for (QGraphicsItem *item : scene->items()) {
            if (item != goku) item->moveBy(0, 2);
        }
    }

    // Verificar colisión con enemigo
    if (enemigo && goku->collidesWithItem(enemigo)) {
        if (goku->y() + goku->boundingRect().height() <
            enemigo->y() + enemigo->boundingRect().height() / 2) {
            // El enemigo será eliminado por la señal enemigoEliminado
        } else {
            volverAlMenu();
            return;
        }
    }

    // Verificar si Goku cayó fuera del escenario
    if (goku->y() > scene->height()) {
        volverAlMenu();
        return;
    }

    // Verificar si llegó a la última plataforma
    if (goku->y() + goku->boundingRect().height() < plataformas.last()->y() &&
        goku->x() + goku->boundingRect().width() > plataformas.last()->x() &&
        goku->x() < plataformas.last()->x() + plataformas.last()->boundingRect().width()) {
        timer->stop();
        emit nivelCompletado(); // Asegúrate de emitir la señal
    }
}

void Nivel1::volverAlMenu() {
    if (!menuMostrado) {
        menuMostrado = true;
        emit solicitarMenu();
    }
}

Nivel1::~Nivel1() {
    delete timer;
    delete scene;
}
