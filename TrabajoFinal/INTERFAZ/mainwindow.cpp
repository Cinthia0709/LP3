#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    escena = new QGraphicsScene(this);
    ui->graphicsView->setScene(escena);

    crearNodos();
    crearConexiones();
    dibujarMapa();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::crearNodos() {
    nodos = {
        {"A", QPointF(50, 50)},
        {"B", QPointF(200, 80)},
        {"C", QPointF(120, 200)},
        {"D", QPointF(300, 200)}
    };
}

void MainWindow::crearConexiones() {
    conexiones = {
        {0, 1, 10.5},
        {1, 2, 15.2},
        {2, 0, 12.7},
        {1, 3, 20.3}
    };
}

void MainWindow::dibujarMapa() {
    escena->clear();
    QPen pen(Qt::black);
    pen.setWidth(2);

    // Dibujar conexiones
    for (const auto& c : conexiones) {
        QPointF p1 = nodos[c.desde].posicion;
        QPointF p2 = nodos[c.hasta].posicion;

        escena->addLine(QLineF(p1, p2), pen);

        // Dibujar distancia
        QPointF medio = (p1 + p2) / 2;
        auto* texto = escena->addText(QString::number(c.distancia));
        texto->setPos(medio);
    }

    // Dibujar nodos
    for (const auto& n : nodos) {
        QRectF rect(n.posicion.x() - 10, n.posicion.y() - 10, 20, 20);
        escena->addEllipse(rect, pen, QBrush(Qt::blue));

        // Etiqueta del nodo
        auto* texto = escena->addText(n.nombre);
        texto->setDefaultTextColor(Qt::white);
        texto->setPos(n.posicion.x() - 6, n.posicion.y() - 10);
    }
}

