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
#include <QInputDialog>

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
    setWindowTitle("RUTAS Y NODOS");

    // Botones para rutas
    btnCrear = new QPushButton("CREAR RUTA");
    btnModificar = new QPushButton("MODIFICAR RUTA");
    btnEliminar = new QPushButton("ELIMINAR RUTA");

    // Nuevos botones para nodos
    btnCrearNodo = new QPushButton("CREAR NODO");
    btnModificarNodo = new QPushButton("MODIFICAR NODO");
    btnEliminarNodo = new QPushButton("ELIMINAR NODO");

    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(800, 600);
    graphicsView->setStyleSheet("border: 2px solid red;");
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setMouseTracking(true);
    graphicsView->viewport()->installEventFilter(this);

    // Conexiones para botones de rutas
    connect(btnCrear, &QPushButton::clicked, this, [this]() {
        modoActual = CrearRuta;
        primerNodoSeleccionado = nullptr;
        segundoNodoSeleccionado = nullptr;
        qDebug() << "Modo: Crear Ruta activado.";
    });
    connect(btnModificar, &QPushButton::clicked, this, [this]() {
        modoActual = ModificarRuta;
        nodoOrigenMod = nullptr;
        nodoDestinoActual = nullptr;
        nodoNuevoDestino = nullptr;
        qDebug() << "Modo: Modificar Ruta activado.";
    });
    connect(btnEliminar, &QPushButton::clicked, this, [this]() {
        modoActual = EliminarRuta;
        nodoEliminar1 = nullptr;
        nodoEliminar2 = nullptr;
        qDebug() << "Modo: Eliminar Ruta activado.";
    });

    // Conexiones para botones de nodos
    connect(btnCrearNodo, &QPushButton::clicked, this, [this]() {
        modoActual = CrearNodo;
        qDebug() << "Modo: Crear Nodo activado.";
        bool ok;
        QString nombre = QInputDialog::getText(this, "Crear Nodo", "Nombre del nodo:", QLineEdit::Normal, "", &ok);
        if (ok && !nombre.isEmpty()) {
            QSqlQuery query;
            query.prepare("INSERT INTO nodos (nombre, x, y) VALUES (:nombre, 0, 0)");
            query.bindValue(":nombre", nombre);
            if (query.exec()) {
                int id = query.lastInsertId().toInt();
                qDebug() << "Nodo creado con ID:" << id;
                cargarNodosDesdeBD();
            } else {
                qDebug() << "Error al crear nodo:" << query.lastError().text();
            }
        }
    });

    connect(btnModificarNodo, &QPushButton::clicked, this, [this]() {
        modoActual = ModificarNodo;
        qDebug() << "Modo: Modificar Nodo activado.";
        // Aquí iría la lógica para modificar un nodo
    });

    connect(btnEliminarNodo, &QPushButton::clicked, this, [this]() {
        modoActual = EliminarNodo;
        qDebug() << "Modo: Eliminar Nodo activado.";
        // Aquí iría la lógica para eliminar un nodo
    });

    // Layout para botones de rutas
    QHBoxLayout *botonRutasLayout = new QHBoxLayout;
    botonRutasLayout->addWidget(btnCrear);
    botonRutasLayout->addWidget(btnModificar);
    botonRutasLayout->addWidget(btnEliminar);

    // Layout para botones de nodos
    QHBoxLayout *botonNodosLayout = new QHBoxLayout;
    botonNodosLayout->addWidget(btnCrearNodo);
    botonNodosLayout->addWidget(btnModificarNodo);
    botonNodosLayout->addWidget(btnEliminarNodo);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(botonRutasLayout);
    mainLayout->addLayout(botonNodosLayout);  // Agregamos el layout de nodos
    mainLayout->addWidget(graphicsView);

    centralWidget->setLayout(mainLayout);
}


void MainWindow::conectarBaseDeDatos() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\LP3\\LP3\\DefinitiveProject\\ToursApp\\Toursapp.sqlite");

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

    primerNodoSeleccionado->setOpacity(1.0);
    segundoNodoSeleccionado->setOpacity(1.0);
    primerNodoSeleccionado = nullptr;
    segundoNodoSeleccionado = nullptr;
    modoActual = Ninguno;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == graphicsView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPointF scenePos = graphicsView->mapToScene(mouseEvent->pos());

        QGraphicsItem *item = scene->itemAt(scenePos, QTransform());
        if (!item) return false;

        QGraphicsItemGroup* grupo = nullptr;
        if (item->type() == QGraphicsItemGroup::Type) {
            grupo = static_cast<QGraphicsItemGroup*>(item);
        } else if (item->parentItem() && item->parentItem()->type() == QGraphicsItemGroup::Type) {
            grupo = static_cast<QGraphicsItemGroup*>(item->parentItem());
        }

        if (!grupo || !nodoPorItem.contains(grupo)) return false;

        // ====================== MODO CREAR RUTA ======================
        if (modoActual == CrearRuta) {
            if (!primerNodoSeleccionado) {
                primerNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Primer nodo seleccionado para crear.";
            } else if (!segundoNodoSeleccionado && grupo != primerNodoSeleccionado) {
                segundoNodoSeleccionado = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Segundo nodo seleccionado para crear.";
                crearRuta();
            }
            return true;
        }

        // ====================== MODO MODIFICAR RUTA ======================
        if (modoActual == ModificarRuta) {
            if (!nodoOrigenMod) {
                nodoOrigenMod = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nodo origen seleccionado para modificar.";
            } else if (!nodoDestinoActual && grupo != nodoOrigenMod) {
                nodoDestinoActual = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nodo destino actual seleccionado para modificar.";
            } else if (!nodoNuevoDestino && grupo != nodoOrigenMod && grupo != nodoDestinoActual) {
                nodoNuevoDestino = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Nuevo destino seleccionado.";
                modificarRuta();
            }
            return true;
        }

        // ====================== MODO ELIMINAR RUTA ======================
        if (modoActual == EliminarRuta) {
            if (!nodoEliminar1) {
                nodoEliminar1 = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Primer nodo seleccionado para eliminar.";
            } else if (!nodoEliminar2 && grupo != nodoEliminar1) {
                nodoEliminar2 = grupo;
                grupo->setOpacity(0.5);
                qDebug() << "Segundo nodo seleccionado para eliminar.";
                eliminarRuta();
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

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE rutas SET destino_id = :nuevo WHERE id = :id");
    updateQuery.bindValue(":nuevo", nuevoDestinoId);
    updateQuery.bindValue(":id", rutaId);

    if (!updateQuery.exec()) {
        qDebug() << "Error al modificar ruta:" << updateQuery.lastError().text();
    } else {
        qDebug() << "Ruta modificada en BD.";
    }


    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD();

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

void MainWindow::eliminarRuta() {
    int id1 = nodoPorItem[nodoEliminar1];
    int id2 = nodoPorItem[nodoEliminar2];

    // Verificar si existe
    QSqlQuery query;
    query.prepare(R"(
        DELETE FROM rutas
        WHERE (origen_id = :a AND destino_id = :b)
           OR (origen_id = :b AND destino_id = :a)
    )");
    query.bindValue(":a", id1);
    query.bindValue(":b", id2);

    if (!query.exec()) {
        qDebug() << "Error al eliminar ruta:" << query.lastError().text();
    } else if (query.numRowsAffected() == 0) {
        qDebug() << "No se encontró la ruta para eliminar.";
    } else {
        qDebug() << "Ruta eliminada correctamente.";
    }


    nodoEliminar1->setOpacity(1.0);
    nodoEliminar2->setOpacity(1.0);
    nodoEliminar1 = nullptr;
    nodoEliminar2 = nullptr;
    modoActual = Ninguno;

    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD();
}
