#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <qcoreevent.h>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    configurarUI();
    conectarBaseDeDatos();
    cargarNodosDesdeBD();
}

MainWindow::~MainWindow() {
    if (db.isOpen())
        db.close();
}

void MainWindow::configurarUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("RUTAS");

    btnCrear = new QPushButton("CREAR");
    btnModificar = new QPushButton("MODIFICAR");
    btnEliminar = new QPushButton("ELIMINAR");

    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(800, 600);
    graphicsView->setStyleSheet("border: 2px solid red;");
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setMouseTracking(true);
    graphicsView->viewport()->installEventFilter(this);
    connect(btnCrear, &QPushButton::clicked, this, &MainWindow::crearRuta);

    QHBoxLayout *botonLayout = new QHBoxLayout;
    botonLayout->addWidget(btnCrear);
    botonLayout->addWidget(btnModificar);
    botonLayout->addWidget(btnEliminar);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(botonLayout);
    mainLayout->addWidget(graphicsView);

    centralWidget->setLayout(mainLayout);
}

void MainWindow::conectarBaseDeDatos() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\eicek\\Documents\\ToursApp\\Toursapp.sqlite");

    if (!db.open()) {
        qDebug() << "Error al abrir la base de datos:" << db.lastError().text();
    } else {
        qDebug() << "Base de datos abierta correctamente.";
    }
    if (!db.open()) {
        qDebug() << "Error al conectar con SQLite:" << db.lastError().text();
    } else {
        qDebug() << "Conectado a Toursapp.sqlite";
    }
}

void MainWindow::cargarNodosDesdeBD() {

    if (!db.isOpen()) return;

    QSqlQuery query("SELECT id, nombre, x, y FROM nodos");
    scene->clear();
    mapaNodos.clear();

    while (query.next()) {
        int id = query.value("id").toInt();
        QString nombre = query.value("nombre").toString();
        int x = query.value("x").toInt();
        int y = query.value("y").toInt();


        QGraphicsEllipseItem *elipse = new QGraphicsEllipseItem(0, 0, 400, 400);
        elipse->setBrush(Qt::cyan);
        elipse->setPen(QPen(Qt::black));

        QGraphicsTextItem *texto = new QGraphicsTextItem(nombre);
        texto->setDefaultTextColor(Qt::black);
        QFont font = texto->font();
        font.setPointSize(100);
        texto->setFont(font);

        QRectF bounds = texto->boundingRect();
        texto->setPos((400 - bounds.width()) / 2, (400 - bounds.height()) / 2);

        QGraphicsItemGroup *nodo = new QGraphicsItemGroup();
        nodo->addToGroup(elipse);
        nodo->addToGroup(texto);
        nodo->setPos(x, y);
        mapaNodos[id] = nodo;
        nodoPorItem[nodo] = id;


        scene->addItem(nodo);
        scene->setSceneRect(scene->itemsBoundingRect());
        graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        mapaNodos[id] = nodo;
    }
    cargarRutasDesdeBD();

}

void MainWindow::cargarRutasDesdeBD() {

    if (!db.isOpen()) return;

    QSqlQuery query("SELECT origen_id, destino_id FROM rutas");

    while (query.next()) {
        int origenId = query.value("origen_id").toInt();
        int destinoId = query.value("destino_id").toInt();

        if (mapaNodos.contains(origenId) && mapaNodos.contains(destinoId)) {
            QPointF p1 = mapaNodos[origenId]->sceneBoundingRect().center();
            QPointF p2 = mapaNodos[destinoId]->sceneBoundingRect().center();

            QGraphicsLineItem *linea = scene->addLine(QLineF(p1, p2), QPen(Qt::green, 2));
            linea->setZValue(-1);
        }
    }
}

void MainWindow::crearRuta() {
    if (!primerNodoSeleccionado || !segundoNodoSeleccionado) {
        qDebug() << "Selecciona dos nodos antes de crear la ruta.";
        return;
    }

    int origenId = nodoPorItem[primerNodoSeleccionado];
    int destinoId = nodoPorItem[segundoNodoSeleccionado];

    QPointF p1 = primerNodoSeleccionado->pos() + QPointF(40, 40);  // centro del nodo
    QPointF p2 = segundoNodoSeleccionado->pos() + QPointF(40, 40);

    QGraphicsLineItem *linea = new QGraphicsLineItem(QLineF(p1, p2));
    linea->setPen(QPen(Qt::darkBlue, 3));
    scene->addItem(linea);

    QSqlQuery query;
    query.prepare("INSERT INTO rutas (origen_id, destino_id) VALUES (:o, :d)");
    query.bindValue(":o", origenId);
    query.bindValue(":d", destinoId);

    if (!query.exec()) {
        qDebug() << "Error al guardar ruta:" << query.lastError().text();
    } else {
        qDebug() << "Ruta guardada.";
    }

    // Restablecer selección
    primerNodoSeleccionado->setOpacity(1.0);
    segundoNodoSeleccionado->setOpacity(1.0);
    primerNodoSeleccionado = nullptr;
    segundoNodoSeleccionado = nullptr;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == graphicsView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF scenePos = graphicsView->mapToScene(mouseEvent->pos());

        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
        if (item) {
            QGraphicsItemGroup* grupo = dynamic_cast<QGraphicsItemGroup*>(item->group());
            if (!grupo) return false;

            if (!primerNodoSeleccionado) {
                primerNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Primer nodo seleccionado.";
            } else if (!segundoNodoSeleccionado && grupo != primerNodoSeleccionado) {
                segundoNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Segundo nodo seleccionado.";
            }
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}
