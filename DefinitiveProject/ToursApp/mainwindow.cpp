#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <limits>

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
    setWindowTitle("Sistema de Rutas Turísticas");

    // Configuración de botones
    btnCrear = new QPushButton("Crear Ruta");
    btnModificar = new QPushButton("Modificar Ruta");
    btnEliminar = new QPushButton("Eliminar Ruta");
    btnRutaMinima = new QPushButton("Ruta Mínima");

    // Configuración del área gráfica
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(800, 600);
    graphicsView->setStyleSheet("border: 1px solid #ccc;");
    scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setMouseTracking(true);
    graphicsView->viewport()->installEventFilter(this);

    // Conexiones de botones
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

    connect(btnRutaMinima, &QPushButton::clicked, this, &MainWindow::mostrarDialogoRutaMinima);

    // Diseño de la interfaz
    QHBoxLayout *botonLayout = new QHBoxLayout;
    botonLayout->addWidget(btnCrear);
    botonLayout->addWidget(btnModificar);
    botonLayout->addWidget(btnEliminar);
    botonLayout->addWidget(btnRutaMinima);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(botonLayout);
    mainLayout->addWidget(graphicsView);

    centralWidget->setLayout(mainLayout);
}

void MainWindow::conectarBaseDeDatos() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\USER\\LP3\\DefinitiveProject\\ToursApp\\Toursapp.sqlite");

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "No se pudo abrir la base de datos: " + db.lastError().text());
    } else {
        qDebug() << "Base de datos conectada correctamente";
    }
}

void MainWindow::cargarNodosDesdeBD() {
    if (!db.isOpen()) return;

    QSqlQuery query("SELECT id, nombre, x, y FROM nodos");
    scene->clear();
    mapaNodos.clear();
    nodoPorItem.clear();

    while (query.next()) {
        int id = query.value("id").toInt();
        QString nombre = query.value("nombre").toString();
        int x = query.value("x").toInt();
        int y = query.value("y").toInt();

        // Crear elemento visual del nodo
        QGraphicsEllipseItem *elipse = new QGraphicsEllipseItem(0, 0, 30, 30);
        elipse->setBrush(Qt::blue);
        elipse->setPen(QPen(Qt::black));

        QGraphicsTextItem *texto = new QGraphicsTextItem(nombre);
        texto->setDefaultTextColor(Qt::white);
        texto->setPos(5, 5);

        QGraphicsItemGroup *nodo = new QGraphicsItemGroup();
        nodo->addToGroup(elipse);
        nodo->addToGroup(texto);
        nodo->setPos(x, y);

        mapaNodos[id] = nodo;
        nodoPorItem[nodo] = id;
        scene->addItem(nodo);
    }

    cargarRutasDesdeBD();
}

void MainWindow::cargarRutasDesdeBD() {
    if (!db.isOpen()) return;

    QSqlQuery query("SELECT origen_id, destino_id FROM rutas");
    QPen penNormal(Qt::darkGray, 2);

    while (query.next()) {
        int origenId = query.value("origen_id").toInt();
        int destinoId = query.value("destino_id").toInt();

        if (mapaNodos.contains(origenId) && mapaNodos.contains(destinoId)) {
            QPointF p1 = mapaNodos[origenId]->sceneBoundingRect().center();
            QPointF p2 = mapaNodos[destinoId]->sceneBoundingRect().center();

            QGraphicsLineItem *linea = scene->addLine(QLineF(p1, p2), penNormal);
            linea->setZValue(-1);
        }
    }
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

        switch (modoActual) {
        case CrearRuta:
            if (!primerNodoSeleccionado) {
                primerNodoSeleccionado = grupo;
                grupo->setOpacity(0.7);
            } else if (!segundoNodoSeleccionado && grupo != primerNodoSeleccionado) {
                segundoNodoSeleccionado = grupo;
                grupo->setOpacity(0.7);
                crearRuta();
            }
            return true;

        case ModificarRuta:
            if (!nodoOrigenMod) {
                nodoOrigenMod = grupo;
                grupo->setOpacity(0.7);
            } else if (!nodoDestinoActual && grupo != nodoOrigenMod) {
                nodoDestinoActual = grupo;
                grupo->setOpacity(0.7);
            } else if (!nodoNuevoDestino && grupo != nodoOrigenMod && grupo != nodoDestinoActual) {
                nodoNuevoDestino = grupo;
                grupo->setOpacity(0.7);
                modificarRuta();
            }
            return true;

        case EliminarRuta:
            if (!nodoEliminar1) {
                nodoEliminar1 = grupo;
                grupo->setOpacity(0.7);
            } else if (!nodoEliminar2 && grupo != nodoEliminar1) {
                nodoEliminar2 = grupo;
                grupo->setOpacity(0.7);
                eliminarRuta();
            }
            return true;

        default:
            return false;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::crearRuta() {
    if (!primerNodoSeleccionado || !segundoNodoSeleccionado) return;

    int origenId = nodoPorItem[primerNodoSeleccionado];
    int destinoId = nodoPorItem[segundoNodoSeleccionado];

    // Verificar si la ruta ya existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM rutas WHERE "
                       "(origen_id = :origen AND destino_id = :destino) OR "
                       "(origen_id = :destino AND destino_id = :origen)");
    checkQuery.bindValue(":origen", origenId);
    checkQuery.bindValue(":destino", destinoId);

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Advertencia", "La ruta ya existe");
        resetSeleccionModificacion();
        return;
    }

    // Crear la ruta en la base de datos
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO rutas (origen_id, destino_id) VALUES (:origen, :destino)");
    insertQuery.bindValue(":origen", origenId);
    insertQuery.bindValue(":destino", destinoId);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Error", "No se pudo crear la ruta: " + insertQuery.lastError().text());
    }

    // Actualizar vista
    resetSeleccionModificacion();
    cargarNodosDesdeBD();
}

void MainWindow::modificarRuta() {
    if (!nodoOrigenMod || !nodoDestinoActual || !nodoNuevoDestino) return;

    int origenId = nodoPorItem[nodoOrigenMod];
    int destinoActualId = nodoPorItem[nodoDestinoActual];
    int nuevoDestinoId = nodoPorItem[nodoNuevoDestino];

    // Verificar y actualizar la ruta
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE rutas SET destino_id = :nuevo WHERE "
                        "(origen_id = :origen AND destino_id = :destino) OR "
                        "(origen_id = :destino AND destino_id = :origen)");
    updateQuery.bindValue(":nuevo", nuevoDestinoId);
    updateQuery.bindValue(":origen", origenId);
    updateQuery.bindValue(":destino", destinoActualId);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Error", "No se pudo modificar la ruta");
    }

    resetSeleccionModificacion();
    cargarNodosDesdeBD();
}

void MainWindow::eliminarRuta() {
    if (!nodoEliminar1 || !nodoEliminar2) return;

    int id1 = nodoPorItem[nodoEliminar1];
    int id2 = nodoPorItem[nodoEliminar2];

    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM rutas WHERE "
                        "(origen_id = :id1 AND destino_id = :id2) OR "
                        "(origen_id = :id2 AND destino_id = :id1)");
    deleteQuery.bindValue(":id1", id1);
    deleteQuery.bindValue(":id2", id2);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(this, "Error", "No se pudo eliminar la ruta");
    }

    resetSeleccionModificacion();
    cargarNodosDesdeBD();
}

void MainWindow::resetSeleccionModificacion() {
    if (primerNodoSeleccionado) primerNodoSeleccionado->setOpacity(1.0);
    if (segundoNodoSeleccionado) segundoNodoSeleccionado->setOpacity(1.0);
    if (nodoOrigenMod) nodoOrigenMod->setOpacity(1.0);
    if (nodoDestinoActual) nodoDestinoActual->setOpacity(1.0);
    if (nodoNuevoDestino) nodoNuevoDestino->setOpacity(1.0);
    if (nodoEliminar1) nodoEliminar1->setOpacity(1.0);
    if (nodoEliminar2) nodoEliminar2->setOpacity(1.0);

    primerNodoSeleccionado = segundoNodoSeleccionado = nullptr;
    nodoOrigenMod = nodoDestinoActual = nodoNuevoDestino = nullptr;
    nodoEliminar1 = nodoEliminar2 = nullptr;
    modoActual = Ninguno;
}

// Implementación de Dijkstra para ruta mínima
std::vector<std::vector<std::pair<int, int>>> MainWindow::construirGrafo() {
    std::vector<std::vector<std::pair<int, int>>> grafo;
    if (!db.isOpen()) return grafo;

    // Obtener el número máximo de nodos
    QSqlQuery maxQuery("SELECT MAX(id) FROM nodos");
    int maxId = maxQuery.next() ? maxQuery.value(0).toInt() : 0;
    grafo.resize(maxId + 1);

    // Construir el grafo con las conexiones existentes
    QSqlQuery query("SELECT origen_id, destino_id FROM rutas");
    while (query.next()) {
        int origen = query.value("origen_id").toInt();
        int destino = query.value("destino_id").toInt();

        if (mapaNodos.contains(origen) && mapaNodos.contains(destino)) {
            QPointF p1 = mapaNodos[origen]->sceneBoundingRect().center();
            QPointF p2 = mapaNodos[destino]->sceneBoundingRect().center();
            int distancia = QLineF(p1, p2).length();

            grafo[origen].emplace_back(destino, distancia);
            grafo[destino].emplace_back(origen, distancia); // Grafo no dirigido
        }
    }

    return grafo;
}

std::pair<std::vector<int>, int> MainWindow::calcularRutaMinima(int origen, int destino) {
    auto grafo = construirGrafo();
    if (grafo.empty() || origen >= static_cast<int>(grafo.size()) || destino >= static_cast<int>(grafo.size())) {
        return {{}, -1};
    }

    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;
    std::vector<int> dist(grafo.size(), std::numeric_limits<int>::max());
    std::vector<int> prev(grafo.size(), -1);

    pq.push({0, origen});
    dist[origen] = 0;

    while (!pq.empty()) {
        auto [current_dist, u] = pq.top();
        pq.pop();

        if (u == destino) break;
        if (current_dist > dist[u]) continue;

        for (const auto& [v, weight] : grafo[u]) {
            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destino] == std::numeric_limits<int>::max()) {
        return {{}, -1};
    }

    std::vector<int> ruta;
    for (int at = destino; at != -1; at = prev[at]) {
        ruta.push_back(at);
    }
    std::reverse(ruta.begin(), ruta.end());

    return {ruta, dist[destino]};
}

void MainWindow::mostrarRutaMinima(const std::vector<int>& ruta, int distancia) {
    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD();

    if (ruta.empty()) {
        QMessageBox::information(this, "Ruta Mínima", "No existe camino entre los nodos seleccionados");
        return;
    }

    // Resaltar la ruta
    QPen penRuta(Qt::red, 3);
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        int nodo1 = ruta[i];
        int nodo2 = ruta[i+1];

        if (mapaNodos.contains(nodo1) && mapaNodos.contains(nodo2)) {
            QPointF p1 = mapaNodos[nodo1]->sceneBoundingRect().center();
            QPointF p2 = mapaNodos[nodo2]->sceneBoundingRect().center();
            QGraphicsLineItem* line = scene->addLine(QLineF(p1, p2), penRuta);

            // Agregar texto con la distancia del segmento
            QPointF midPoint = (p1 + p2) / 2;
            QGraphicsTextItem* text = scene->addText(QString::number(QLineF(p1, p2).length(), 'f', 0));
            text->setPos(midPoint);
            text->setDefaultTextColor(Qt::red);
            text->setZValue(1);
        }
    }

    // Resaltar los nodos
    for (int nodoId : ruta) {
        if (mapaNodos.contains(nodoId)) {
            for (QGraphicsItem* item : mapaNodos[nodoId]->childItems()) {
                if (auto elipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    elipse->setBrush(Qt::yellow);
                }
            }
        }
    }

    // Mostrar distancia total
    QGraphicsTextItem* totalDistText = scene->addText(QString("Distancia total: %1").arg(distancia));
    totalDistText->setDefaultTextColor(Qt::blue);
    totalDistText->setPos(20, 20);
    totalDistText->setZValue(1);
}

void MainWindow::mostrarDialogoRutaMinima() {
    QDialog dialog(this);
    dialog.setWindowTitle("Calcular Ruta Mínima");

    QFormLayout form(&dialog);
    QComboBox *comboOrigen = new QComboBox(&dialog);
    QComboBox *comboDestino = new QComboBox(&dialog);

    QSqlQuery query("SELECT id, nombre FROM nodos ORDER BY nombre");
    while (query.next()) {
        comboOrigen->addItem(query.value("nombre").toString(), query.value("id"));
        comboDestino->addItem(query.value("nombre").toString(), query.value("id"));
    }

    form.addRow("Punto de origen:", comboOrigen);
    form.addRow("Punto de destino:", comboDestino);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int origen = comboOrigen->currentData().toInt();
        int destino = comboDestino->currentData().toInt();
        auto [ruta, distancia] = calcularRutaMinima(origen, destino);
        mostrarRutaMinima(ruta, distancia);
    }
}
