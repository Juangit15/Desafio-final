QT += widgets gui
LIBS += -lwinmm

TARGET = DragonBallGame
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainmenu.cpp \
    jugador.cpp \
    enemigo.cpp \
    nivel1.cpp \
    nivel2.cpp \
    nivel3.cpp

HEADERS += \
    mainwindow.h \
    mainmenu.h \
    jugador.h \
    enemigo.h \
    nivel1.h \
    nivel2.h \
    nivel3.h

RESOURCES += recursos.qrc
