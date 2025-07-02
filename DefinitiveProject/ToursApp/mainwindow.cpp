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
    connect(btnModificar, &QPushButton::clicked, this, [this]() {
        modoActual = ModificarRuta;
        nodoOrigenMod = nullptr;
        nodoDestinoActual = nullptr;
        nodoNuevoDestino = nullptr;
        qDebug() << "Modo: Modificar Ruta activado.";
    });

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

    QSqlQuery checkQuery;
    checkQuery.prepare(R"(
        SELECT COUNT(*) FROM rutas
        WHERE (origen_id = :o AND destino_id = :d)
           OR (origen_id = :d AND destino_id = :o)
    )");
    checkQuery.bindValue(":o", origenId);
    checkQuery.bindValue(":d", destinoId);

    if (!checkQuery.exec()) {
        qDebug() << "Error al verificar existencia de ruta:" << checkQuery.lastError().text();
        return;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "La ruta entre" << origenId << "y" << destinoId << "ya existe.";
        primerNodoSeleccionado->setOpacity(1.0);
        segundoNodoSeleccionado->setOpacity(1.0);
        primerNodoSeleccionado = nullptr;
        segundoNodoSeleccionado = nullptr;
        return;
    }

    QPointF p1 = primerNodoSeleccionado->pos() + QPointF(200, 200);
    QPointF p2 = segundoNodoSeleccionado->pos() + QPointF(200, 200);


    QGraphicsLineItem *linea = new QGraphicsLineItem(QLineF(p1, p2));
    linea->setPen(QPen(Qt::blue, 3));
    scene->addItem(linea);

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO rutas (origen_id, destino_id) VALUES (:o, :d)");
    insertQuery.bindValue(":o", origenId);
    insertQuery.bindValue(":d", destinoId);

    if (!insertQuery.exec()) {
        qDebug() << "Error al guardar ruta:" << insertQuery.lastError().text();
    } else {
        qDebug() << "Ruta creada entre" << origenId << "y" << destinoId;
    }

    // Limpiar selección
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
        if (!item) return false;

        // Buscar el grupo al que pertenece el item (el nodo)
        QGraphicsItemGroup* grupo = nullptr;

        if (item->type() == QGraphicsItemGroup::Type) {
            grupo = static_cast<QGraphicsItemGroup*>(item);
        } else if (item->parentItem() && item->parentItem()->type() == QGraphicsItemGroup::Type) {
            grupo = static_cast<QGraphicsItemGroup*>(item->parentItem());
        }

        if (!grupo || !nodoPorItem.contains(grupo)) return false;

        // MODO: CREAR RUTA
        if (modoActual == CrearRuta) {
            if (!primerNodoSeleccionado) {
                primerNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Primer nodo seleccionado.";
            } else if (!segundoNodoSeleccionado && grupo != primerNodoSeleccionado) {
                segundoNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Segundo nodo seleccionado.";
                crearRuta();  // llama a la función cuando hay dos nodos
            }
            return true;
        }

        // MODO: MODIFICAR RUTA
        if (modoActual == ModificarRuta) {
            if (!nodoOrigenMod) {
                nodoOrigenMod = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nodo origen seleccionado.";
            } else if (!nodoDestinoActual && grupo != nodoOrigenMod) {
                nodoDestinoActual = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nodo destino actual seleccionado.";
            } else if (!nodoNuevoDestino && grupo != nodoOrigenMod && grupo != nodoDestinoActual) {
                nodoNuevoDestino = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nuevo destino seleccionado.";
                modificarRuta();  // ejecuta el cambio
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::modificarRuta() {
    int origenId = nodoPorItem[nodoOrigenMod];
    int destinoActualId = nodoPorItem[nodoDestinoActual];
    int nuevoDestinoId = nodoPorItem[nodoNuevoDestino];

    // Verificar si la ruta actual existe
    QSqlQuery checkQuery;
    checkQuery.prepare(R"(
        SELECT id FROM rutas
        WHERE (origen_id = :o AND destino_id = :d)
           OR (origen_id = :d AND destino_id = :o)
    )");
    checkQuery.bindValue(":o", origenId);
    checkQuery.bindValue(":d", destinoActualId);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "La ruta original no existe.";
        resetSeleccionModificacion();
        return;
    }

    int rutaId = checkQuery.value(0).toInt();

    // Actualizar la ruta
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE rutas SET destino_id = :nuevo WHERE id = :id");
    updateQuery.bindValue(":nuevo", nuevoDestinoId);
    updateQuery.bindValue(":id", rutaId);

    if (!updateQuery.exec()) {
        qDebug() << "Error al modificar ruta:" << updateQuery.lastError().text();
    } else {
        qDebug() << "Ruta modificada en BD.";
    }

    // Redibujar (borramos todas y recargamos desde BD)
    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD(); // asegúrate de tener esta función

    resetSeleccionModificacion();
}

void MainWindow::resetSeleccionModificacion() {
    if (nodoOrigenMod) nodoOrigenMod->setOpacity(1.0);
    if (nodoDestinoActual) nodoDestinoActual->setOpacity(1.0);
    if (nodoNuevoDestino) nodoNuevoDestino->setOpacity(1.0);

    nodoOrigenMod = nullptr;
    nodoDestinoActual = nullptr;
    nodoNuevoDestino = nullptr;
    modoActual = Ninguno;
}
