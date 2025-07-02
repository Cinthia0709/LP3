#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    configurarUI();
    conectarBaseDeDatos();
    cargarNodosDesdeBD();
}

MainWindow::~MainWindow() {
    if (db.isOpen()) db.close();
}

void MainWindow::configurarUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Rutas Qt");

    btnCrearNodo = new QPushButton("CREAR NODO");
    btnModificarNodo = new QPushButton("MODIFICAR NODO");
    btnEliminarNodo = new QPushButton("ELIMINAR NODO");
    btnCrearRuta = new QPushButton("CREAR RUTA");
    btnModificarRuta = new QPushButton("MODIFICAR RUTA");
    btnEliminarRuta = new QPushButton("ELIMINAR RUTA");

    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(800, 600);
    graphicsView->setStyleSheet("border: 2px solid red;");
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setMouseTracking(true);
    graphicsView->setInteractive(true);
    graphicsView->viewport()->installEventFilter(this);

    // Botones
    connect(btnCrearNodo, &QPushButton::clicked, this, [=] {
        modoActual = CrearNodo; resetSeleccionNodos(); qDebug() << "Crear nodo";
    });
    connect(btnModificarNodo, &QPushButton::clicked, this, [=] {
        modoActual = ModificarNodo; resetSeleccionNodos(); qDebug() << "Modificar nodo";
    });
    connect(btnEliminarNodo, &QPushButton::clicked, this, [=] {
        modoActual = EliminarNodo; resetSeleccionNodos(); qDebug() << "Eliminar nodo";
    });
    connect(btnCrearRuta, &QPushButton::clicked, this, [=] {
        modoActual = CrearRuta; resetSeleccionNodos(); qDebug() << "Crear ruta";
    });
    connect(btnModificarRuta, &QPushButton::clicked, this, [=] {
        modoActual = ModificarRuta; resetSeleccionNodos(); qDebug() << "Modificar ruta";
    });
    connect(btnEliminarRuta, &QPushButton::clicked, this, [=] {
        modoActual = EliminarRuta; resetSeleccionNodos(); qDebug() << "Eliminar ruta";
    });

    QHBoxLayout *layNodos = new QHBoxLayout;
    layNodos->addWidget(btnCrearNodo);
    layNodos->addWidget(btnModificarNodo);
    layNodos->addWidget(btnEliminarNodo);

    QHBoxLayout *layRutas = new QHBoxLayout;
    layRutas->addWidget(btnCrearRuta);
    layRutas->addWidget(btnModificarRuta);
    layRutas->addWidget(btnEliminarRuta);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layNodos);
    mainLayout->addLayout(layRutas);
    mainLayout->addWidget(graphicsView);

    centralWidget->setLayout(mainLayout);
}

void MainWindow::conectarBaseDeDatos() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\eicek\\Documents\\CRUDnodos\\Toursapp.sqlite");  // Cambia la ruta si quieres

    if (!db.open())
        qDebug() << "Error al abrir la BD:" << db.lastError().text();
    else
        qDebug() << "Base de datos abierta correctamente.";
}

void MainWindow::cargarNodosDesdeBD() {
    QMap<int, QPointF> posiciones;

    QSqlQuery nodos("SELECT id, nombre, x, y FROM nodos");
    while (nodos.next()) {
        int id = nodos.value(0).toInt();
        QString nombre = nodos.value(1).toString();
        float x = nodos.value(2).toFloat();
        float y = nodos.value(3).toFloat();

        auto *circ = scene->addEllipse(x - 10, y - 10, 20, 20, QPen(Qt::black), QBrush(Qt::blue));
        auto *text = scene->addText(nombre);
        text->setPos(x + 10, y - 10);
        auto *grupo = scene->createItemGroup({circ, text});
        nodoPorItem[grupo] = id;
        posiciones[id] = QPointF(x, y);
    }

    QSqlQuery rutas("SELECT origen_id, destino_id FROM rutas");
    while (rutas.next()) {
        int o = rutas.value(0).toInt();
        int d = rutas.value(1).toInt();
        if (posiciones.contains(o) && posiciones.contains(d)) {
            scene->addLine(QLineF(posiciones[o], posiciones[d]), QPen(Qt::darkRed, 2));
        }
    }
}

void MainWindow::crearNodo(const QPointF &pos) {
    bool ok;
    QString nombre = QInputDialog::getText(this, "Nuevo Nodo", "Nombre:", QLineEdit::Normal, "", &ok);
    if (!ok || nombre.isEmpty()) return;

    QSqlQuery q;
    q.prepare("INSERT INTO nodos (nombre, x, y) VALUES (?, ?, ?)");
    q.addBindValue(nombre);
    q.addBindValue(pos.x());
    q.addBindValue(pos.y());
    if (!q.exec()) {
        qDebug() << "Error insertando nodo:" << q.lastError().text();
        return;
    }

    scene->clear();
    cargarNodosDesdeBD();
}

void MainWindow::modificarNodo() {
    if (!nodoSeleccionado) return;
    int id = nodoPorItem[nodoSeleccionado];

    QSqlQuery q;
    q.prepare("SELECT nombre FROM nodos WHERE id = ?");
    q.addBindValue(id);
    if (!q.exec() || !q.next()) return;

    bool ok;
    QString nuevo = QInputDialog::getText(this, "Modificar Nodo", "Nombre:", QLineEdit::Normal, q.value(0).toString(), &ok);
    if (!ok || nuevo.isEmpty()) return;

    QSqlQuery uq;
    uq.prepare("UPDATE nodos SET nombre = ? WHERE id = ?");
    uq.addBindValue(nuevo);
    uq.addBindValue(id);
    uq.exec();

    scene->clear();
    cargarNodosDesdeBD();
}

void MainWindow::eliminarNodo() {
    if (!nodoSeleccionado) return;
    int id = nodoPorItem[nodoSeleccionado];

    QSqlQuery qr;
    qr.prepare("DELETE FROM rutas WHERE origen_id = ? OR destino_id = ?");
    qr.addBindValue(id);
    qr.addBindValue(id);
    qr.exec();

    QSqlQuery q;
    q.prepare("DELETE FROM nodos WHERE id = ?");
    q.addBindValue(id);
    q.exec();

    scene->clear();
    cargarNodosDesdeBD();
}

void MainWindow::crearRuta() {
    if (!primerNodoSeleccionado || !segundoNodoSeleccionado) return;
    int a = nodoPorItem[primerNodoSeleccionado];
    int b = nodoPorItem[segundoNodoSeleccionado];

    QSqlQuery q;
    q.prepare("INSERT INTO rutas (origen_id, destino_id) VALUES (?, ?)");
    q.addBindValue(a);
    q.addBindValue(b);
    q.exec();

    scene->clear();
    cargarNodosDesdeBD();
    resetSeleccionNodos();
}

void MainWindow::modificarRuta() {
    if (!nodoOrigenMod || !nodoDestinoActual || !nodoNuevoDestino) return;

    int o = nodoPorItem[nodoOrigenMod];
    int d = nodoPorItem[nodoDestinoActual];
    int nuevo = nodoPorItem[nodoNuevoDestino];

    QSqlQuery q;
    q.prepare("UPDATE rutas SET destino_id = ? WHERE origen_id = ? AND destino_id = ?");
    q.addBindValue(nuevo);
    q.addBindValue(o);
    q.addBindValue(d);
    q.exec();

    scene->clear();
    cargarNodosDesdeBD();
    resetSeleccionNodos();
}

void MainWindow::eliminarRuta() {
    if (!nodoEliminar1 || !nodoEliminar2) return;

    int a = nodoPorItem[nodoEliminar1];
    int b = nodoPorItem[nodoEliminar2];

    QSqlQuery q;
    q.prepare("DELETE FROM rutas WHERE (origen_id = ? AND destino_id = ?) OR (origen_id = ? AND destino_id = ?)");
    q.addBindValue(a);
    q.addBindValue(b);
    q.addBindValue(b);
    q.addBindValue(a);
    q.exec();

    scene->clear();
    cargarNodosDesdeBD();
    resetSeleccionNodos();
}

void MainWindow::resetSeleccionNodos() {
    if (nodoSeleccionado) nodoSeleccionado->setOpacity(1.0);
    if (primerNodoSeleccionado) primerNodoSeleccionado->setOpacity(1.0);
    if (segundoNodoSeleccionado) segundoNodoSeleccionado->setOpacity(1.0);
    if (nodoOrigenMod) nodoOrigenMod->setOpacity(1.0);
    if (nodoDestinoActual) nodoDestinoActual->setOpacity(1.0);
    if (nodoNuevoDestino) nodoNuevoDestino->setOpacity(1.0);
    if (nodoEliminar1) nodoEliminar1->setOpacity(1.0);
    if (nodoEliminar2) nodoEliminar2->setOpacity(1.0);

    nodoSeleccionado = primerNodoSeleccionado = segundoNodoSeleccionado = nullptr;
    nodoOrigenMod = nodoDestinoActual = nodoNuevoDestino = nullptr;
    nodoEliminar1 = nodoEliminar2 = nullptr;
    modoActual = Ninguno;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    qDebug() << "eventFilter llamado, tipo:" << event->type();
    if (obj == graphicsView->viewport() && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPointF pos = graphicsView->mapToScene(me->pos());
        QGraphicsItem *item = scene->itemAt(pos, QTransform());

        if (!item && modoActual == CrearNodo) {
            crearNodo(pos);
            return true;
        }

        if (!item) return false;

        QGraphicsItemGroup *grupo = nullptr;
        if (item->type() == QGraphicsItemGroup::Type) grupo = static_cast<QGraphicsItemGroup*>(item);
        else if (item->parentItem() && item->parentItem()->type() == QGraphicsItemGroup::Type)
            grupo = static_cast<QGraphicsItemGroup*>(item->parentItem());

        if (!grupo || !nodoPorItem.contains(grupo)) return false;

        switch (modoActual) {
        case ModificarNodo:
            nodoSeleccionado = grupo; grupo->setOpacity(0.5); modificarNodo(); break;
        case EliminarNodo:
            nodoSeleccionado = grupo; grupo->setOpacity(0.5); eliminarNodo(); break;
        case CrearRuta:
            if (!primerNodoSeleccionado) primerNodoSeleccionado = grupo;
            else segundoNodoSeleccionado = grupo;
            grupo->setOpacity(0.5);
            if (primerNodoSeleccionado && segundoNodoSeleccionado) crearRuta();
            break;
        case ModificarRuta:
            if (!nodoOrigenMod) nodoOrigenMod = grupo;
            else if (!nodoDestinoActual) nodoDestinoActual = grupo;
            else nodoNuevoDestino = grupo;
            grupo->setOpacity(0.5);
            if (nodoOrigenMod && nodoDestinoActual && nodoNuevoDestino) modificarRuta();
            break;
        case EliminarRuta:
            if (!nodoEliminar1) nodoEliminar1 = grupo;
            else nodoEliminar2 = grupo;
            grupo->setOpacity(0.5);
            if (nodoEliminar1 && nodoEliminar2) eliminarRuta();
            break;
        default: break;
        }
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}
