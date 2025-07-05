#include "mainmenu.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QResizeEvent> // Necesario para el redimensionamiento

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void MainMenu::setupUI() {
    // Configurar layout principal
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0); // Eliminar márgenes

    // Crear label para el fondo
    background = new QLabel(this);
    background->setAlignment(Qt::AlignCenter);
    background->setScaledContents(true);
    background->lower(); // Asegurar que está detrás de todo

    // Configurar widgets del menú
    QWidget *menuWidget = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel("Desafío Final: Dragon Ball");
    title->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #000080; }");
    title->setAlignment(Qt::AlignCenter);
    menuLayout->addWidget(title);

    QPushButton *nivel1Btn = new QPushButton("Nivel 1: Ascenso al Templo");
    QPushButton *nivel2Btn = new QPushButton("Nivel 2: Entrenamiento con Mr. Popo");
    QPushButton *nivel3Btn = new QPushButton("Nivel 3: Encuentro con KamiSama");

    QString buttonStyle = "QPushButton { background: solid #19b72d; border: 2px solid #C0C0C0; padding: 10px; font-weight: bold; color: white; }";
    nivel1Btn->setStyleSheet(buttonStyle);
    nivel2Btn->setStyleSheet(buttonStyle);
    nivel3Btn->setStyleSheet(buttonStyle);

    menuLayout->addWidget(nivel1Btn);
    menuLayout->addWidget(nivel2Btn);
    menuLayout->addWidget(nivel3Btn);

    // Cargar imagen de fondo
    QPixmap fondo("C:/Users/juanm/Downloads/DragonBall/recursos/backmenu.png"); // Usar sistema de recursos
    background->setPixmap(fondo);

    // Añadir el widget del menú al layout principal
    layout->addWidget(menuWidget);

    // Conexiones de los botones
    connect(nivel1Btn, &QPushButton::clicked, this, &MainMenu::nivel1Requested);
    connect(nivel2Btn, &QPushButton::clicked, this, &MainMenu::nivel2Requested);
    connect(nivel3Btn, &QPushButton::clicked, this, &MainMenu::nivel3Requested);
}

void MainMenu::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Tamaño fijo deseado para el fondo
    const QSize fixedSize(1500, 500);

    if (background) {
        // Obtener la imagen original
        QPixmap originalPixmap = background->pixmap(Qt::ReturnByValue);
        if (!originalPixmap.isNull()) {
            // Escalar manteniendo relación de aspecto
            QPixmap scaledPixmap = originalPixmap.scaled(fixedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Crear una nueva pixmap del tamaño exacto 900x500
            QPixmap finalPixmap(fixedSize);
            finalPixmap.fill(Qt::transparent);

            // Pintar la imagen escalada centrada
            QPainter painter(&finalPixmap);
            painter.drawPixmap((fixedSize.width() - scaledPixmap.width()) / 2,
                               (fixedSize.height() - scaledPixmap.height()) / 2,
                               scaledPixmap);
            painter.end();

            // Aplicar al fondo
            background->setPixmap(finalPixmap);
            background->setGeometry((width() - fixedSize.width()) / 2,
                                    (height() - fixedSize.height()) / 2,
                                    fixedSize.width(),
                                    fixedSize.height());
        }
    }
}
