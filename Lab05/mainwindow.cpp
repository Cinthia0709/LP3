#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QInputDialog>

bool carreraEnCurso = false;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timer(new QTimer(this)) {

    QWidget *centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    btnIniciar = new QPushButton("Iniciar Carrera");
    layout->addWidget(btnIniciar);


    setCentralWidget(centralWidget);

    connect(btnIniciar, &QPushButton::clicked, this, &MainWindow::iniciarCarrera);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarCarrera);
}

MainWindow::~MainWindow() {
    for (auto t : std::as_const(tiempos)) delete t;
}

void MainWindow::extracted() {
    for (QLabel *label : std::as_const(caballos)) {
        layout->removeWidget(label);
        delete label;
    }
}
void MainWindow::crearCaballos(int cantidad) {
    // Eliminar caballos anteriores
    extracted();
    caballos.clear();
    tiempos.clear();
    llegados.clear();

    // Crear nuevos caballos
    for (int i = 0; i < cantidad; ++i) {
        QLabel *caballo = new QLabel(QString("Caballo %1 🐎").arg(i + 1));
        layout->insertWidget(i, caballo);
        caballos.push_back(caballo);

        QElapsedTimer *timer = new QElapsedTimer();
        tiempos.push_back(timer);
        llegados.push_back(false);
    }
}

void MainWindow::iniciarCarrera() {
    if (carreraEnCurso) return;

    cantidadCaballos = QInputDialog::getInt(this, "Cantidad de Caballos", "¿Cuántos caballos participarán?", 5, 1, 20);

    crearCaballos(cantidadCaballos);

    for (int i = 0; i < cantidadCaballos; ++i) {
        caballos[i]->move(10, caballos[i]->y());
        tiempos[i]->restart();
        caballos[i]->setStyleSheet("");
        llegados[i] = false;
    }

    carreraEnCurso = true;
    btnIniciar->setEnabled(false);
    timer->start(100);
}


void MainWindow::actualizarCarrera() {
    bool todosLlegaron = true;

    for (int i = 0; i < cantidadCaballos; ++i) {
        if (!llegados[i]) {
            int avance = QRandomGenerator::global()->bounded(11);
            int nuevaX = caballos[i]->x() + avance;

            if (nuevaX >= meta) {
                caballos[i]->move(meta, caballos[i]->y());
                llegados[i] = true;

                qint64 ms = tiempos[i]->elapsed();
                double seg = ms / 1000.0;
                caballos[i]->setText(QString("Caballo %1 🐎 - Llegó en %2 segundos").arg(i + 1).arg(seg, 0, 'f', 2));
                caballos[i]->setStyleSheet("color: green; font-weight: bold;");
            } else {
                caballos[i]->move(nuevaX, caballos[i]->y());
                todosLlegaron = false;
            }


        }
    }

    if (todosLlegaron) {
        timer->stop();
        carreraEnCurso = false;
        btnIniciar->setEnabled(true);
    }
}

