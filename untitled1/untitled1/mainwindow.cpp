#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QRandomGenerator>

/**
 * @brief Constructor de MainWindow. Inicializa la interfaz y configura los caballos.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), caballo1(new QLabel("Caballo 1 🐎")),
    caballo2(new QLabel("Caballo 2 🐎")), caballo3(new QLabel("Caballo 3 🐎")),
    caballo4(new QLabel("Caballo 4 🐎")), timer(new QTimer(this)) {

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    layout->addWidget(caballo1);
    layout->addWidget(caballo2);
    layout->addWidget(caballo3);
    layout->addWidget(caballo4);

    QPushButton *btnIniciar = new QPushButton("Iniciar Carrera");
    layout->addWidget(btnIniciar);

    setCentralWidget(centralWidget);

    connect(btnIniciar, &QPushButton::clicked, this, &MainWindow::iniciarCarrera);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarCarrera);
}

/**
 * @brief Destructor de MainWindow.
 */
MainWindow::~MainWindow() {}

void MainWindow::iniciarCarrera() {
    // Reinicia la carrera
    caballo1->move(10, caballo1->y());
    caballo2->move(10, caballo2->y());
    caballo3->move(10, caballo3->y());
    caballo4->move(10, caballo4->y());
    timer->start(100);
}

void MainWindow::actualizarCarrera() {
    // Movimiento aleatorio de cada caballo
    caballo1->move(caballo1->x() + QRandomGenerator::global()->bounded(5), caballo1->y());
    caballo2->move(caballo2->x() + QRandomGenerator::global()->bounded(5), caballo2->y());
    caballo3->move(caballo3->x() + QRandomGenerator::global()->bounded(5), caballo3->y());
    caballo4->move(caballo4->x() + QRandomGenerator::global()->bounded(5), caballo4->y());

    // Verifica si alguno llegó al final
    if (caballo1->x() > 400 || caballo2->x() > 400 || caballo3->x() > 400 || caballo4->x() > 400) {
        timer->stop();
    }
}
