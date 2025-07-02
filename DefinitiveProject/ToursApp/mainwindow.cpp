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
#include <queue>
#include <stack>
#include <functional>
#include <QTimer>
#include <QInputDialog>
#include <algorithm>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), timerAnimacion(new QTimer(this)), indiceAnimacion(0) {
    configurarUI();
    conectarBaseDeDatos();
    cargarNodosDesdeBD();

    connect(timerAnimacion, &QTimer::timeout, this, &MainWindow::avanzarAnimacion);
}

MainWindow::~MainWindow() {
    if (db.isOpen())
        db.close();
    delete timerAnimacion;
}

void MainWindow::configurarUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Sistema de Rutas TurÃ­sticas");

    // ConfiguraciÃ³n de botones
    btnCrearNodo = new QPushButton("Agregar Nodo");
    btnLeerNodos = new QPushButton("Ver Nodos");
    btnActualizarNodo = new QPushButton("Actualizar Nodo");
    btnEliminarNodoDB = new QPushButton("Eliminar Nodo");
    btnCrear = new QPushButton("Crear Ruta");
    btnModificar = new QPushButton("Modificar Ruta");
    btnEliminar = new QPushButton("Eliminar Ruta");
    btnRutaMinima = new QPushButton("Ruta MÃ­nima");
    btnRecorrido = new QPushButton("Recorrido BFS/DFS");

    // ConfiguraciÃ³n del Ã¡rea grÃ¡fica
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
        resetSeleccionModificacion();
        qDebug() << "Modo: Crear Ruta activado.";
    });

    connect(btnModificar, &QPushButton::clicked, this, [this]() {
        modoActual = ModificarRuta;
        resetSeleccionModificacion();
        qDebug() << "Modo: Modificar Ruta activado.";
    });

    connect(btnEliminar, &QPushButton::clicked, this, [this]() {
        modoActual = EliminarRuta;
        resetSeleccionModificacion();
        qDebug() << "Modo: Eliminar Ruta activado.";
    });

    connect(btnRutaMinima, &QPushButton::clicked, this, &MainWindow::mostrarDialogoRutaMinima);
    connect(btnRecorrido, &QPushButton::clicked, this, &MainWindow::mostrarDialogoRecorrido);
    connect(btnCrearNodo, &QPushButton::clicked, this, &MainWindow::mostrarDialogoCrearNodo);
    connect(btnLeerNodos, &QPushButton::clicked, this, &MainWindow::mostrarDialogoLeerNodos);
    connect(btnActualizarNodo, &QPushButton::clicked, this, &MainWindow::mostrarDialogoActualizarNodo);
    connect(btnEliminarNodoDB, &QPushButton::clicked, this, &MainWindow::mostrarDialogoEliminarNodo);


    // DiseÃ±o de la interfaz
    QHBoxLayout *botonLayout = new QHBoxLayout;
    botonLayout->addWidget(btnCrearNodo);
    botonLayout->addWidget(btnLeerNodos);
    botonLayout->addWidget(btnActualizarNodo);
    botonLayout->addWidget(btnEliminarNodoDB);
    botonLayout->addWidget(btnCrear);
    botonLayout->addWidget(btnModificar);
    botonLayout->addWidget(btnEliminar);
    botonLayout->addWidget(btnRutaMinima);
    botonLayout->addWidget(btnRecorrido);

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

        QGraphicsEllipseItem *elipse = new QGraphicsEllipseItem(0, 0, 30, 30);
        elipse->setBrush(Qt::blue);

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

    cargarRutasDesdeBD();  // 🟢 Cargar rutas después de nodos
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

        qDebug() << "Grupo clickeado con ID:" << nodoPorItem[grupo];  // DEBUG

        switch (modoActual) {
        case CrearRuta:
            if (!primerNodoSeleccionado) {
                primerNodoSeleccionado = grupo;
                grupo->setOpacity(0.7);
                qDebug() << "Primer nodo seleccionado";
            } else if (!segundoNodoSeleccionado && grupo != primerNodoSeleccionado) {
                segundoNodoSeleccionado = grupo;
                grupo->setOpacity(0.7);
                qDebug() << "Segundo nodo seleccionado";
                crearRuta(); // Aquí debe llamarse
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

    int origenId = nodoPorItem.value(primerNodoSeleccionado, -1);
    int destinoId = nodoPorItem.value(segundoNodoSeleccionado, -1);

    qDebug() << "IDs: origen" << origenId << "destino" << destinoId;

    if (origenId == -1 || destinoId == -1 || origenId == destinoId) return;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM rutas WHERE (origen_id = :origen AND destino_id = :destino) OR (origen_id = :destino AND destino_id = :origen)");
    checkQuery.bindValue(":origen", origenId);
    checkQuery.bindValue(":destino", destinoId);

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Advertencia", "La ruta ya existe");
        resetSeleccionModificacion();
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO rutas (origen_id, destino_id) VALUES (:origen, :destino)");
    insertQuery.bindValue(":origen", origenId);
    insertQuery.bindValue(":destino", destinoId);

    if (!insertQuery.exec()) {
        QMessageBox::critical(this, "Error", "No se pudo crear la ruta: " + insertQuery.lastError().text());
    }

    resetSeleccionModificacion();
    cargarNodosDesdeBD();
}

void MainWindow::mostrarRutaMinima(const std::vector<int>& ruta, int distancia) {
    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD();

    if (ruta.empty()) {
        QMessageBox::information(this, "Ruta Mínima", "No existe camino entre los nodos seleccionados");
        return;
    }

    QPen penRuta(Qt::red, 3);
    for (size_t i = 0; i < ruta.size() - 1; ++i) {
        int nodo1 = ruta[i];
        int nodo2 = ruta[i+1];

        if (mapaNodos.contains(nodo1) && mapaNodos.contains(nodo2)) {
            QPointF p1 = mapaNodos[nodo1]->sceneBoundingRect().center();
            QPointF p2 = mapaNodos[nodo2]->sceneBoundingRect().center();
            scene->addLine(QLineF(p1, p2), penRuta);

            QPointF midPoint = (p1 + p2) / 2;
            QGraphicsTextItem* text = scene->addText(QString::number(QLineF(p1, p2).length(), 'f', 0));
            text->setPos(midPoint);
            text->setDefaultTextColor(Qt::red);
            text->setZValue(1);
        }
    }

    for (int nodoId : ruta) {
        if (mapaNodos.contains(nodoId)) {
            for (QGraphicsItem* item : mapaNodos[nodoId]->childItems()) {
                if (auto elipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    elipse->setBrush(Qt::yellow);
                }
            }
        }
    }

    QGraphicsTextItem* totalDistText = scene->addText(QString("Distancia total: %1").arg(distancia));
    totalDistText->setDefaultTextColor(Qt::blue);
    totalDistText->setPos(20, 20);
    totalDistText->setZValue(1);
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

std::vector<std::vector<std::pair<int, int>>> MainWindow::construirGrafo() {
    std::vector<std::vector<std::pair<int, int>>> grafo;
    if (!db.isOpen()) return grafo;

    // Obtener el nÃºmero mÃ¡ximo de nodos
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


void MainWindow::mostrarDialogoRutaMinima() {
    QDialog dialog(this);
    dialog.setWindowTitle("Calcular Ruta MÃ­nima");

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

void MainWindow::mostrarDialogoRecorrido() {
    QDialog dialog(this);
    dialog.setWindowTitle("Configurar Recorrido");
    dialog.setFixedSize(400, 200);

    QFormLayout form(&dialog);

    QComboBox *comboNodoInicial = new QComboBox(&dialog);
    QSqlQuery queryNodos("SELECT id, nombre FROM nodos ORDER BY nombre");
    while (queryNodos.next()) {
        comboNodoInicial->addItem(queryNodos.value("nombre").toString(), queryNodos.value("id"));
    }

    QComboBox *comboAlgoritmo = new QComboBox(&dialog);
    comboAlgoritmo->addItem("BFS (Recorrido por amplitud)");
    comboAlgoritmo->addItem("DFS (Recorrido por profundidad)");

    QComboBox *comboVisualizacion = new QComboBox(&dialog);
    comboVisualizacion->addItem("Mostrar distancia");
    comboVisualizacion->addItem("Mostrar tiempo estimado");

    form.addRow("Nodo inicial:", comboNodoInicial);
    form.addRow("Algoritmo:", comboAlgoritmo);
    form.addRow("VisualizaciÃ³n:", comboVisualizacion);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int nodoInicial = comboNodoInicial->currentData().toInt();
        QString algoritmo = comboAlgoritmo->currentText();
        QString visualizacion = comboVisualizacion->currentText();

        limpiarVisualizacion();

        if (algoritmo == "BFS (Recorrido por amplitud)") {
            ejecutarBFS(nodoInicial);
        } else {
            ejecutarDFS(nodoInicial);
        }

        visualizarRecorrido(recorridoActual, visualizacion);
    }
}

void MainWindow::mostrarDialogoCrearNodo() {
    QDialog dialog(this);
    dialog.setWindowTitle("Crear Nodo");
    dialog.setFixedSize(300, 200);

    QFormLayout form(&dialog);

    QLineEdit *nombreEdit = new QLineEdit(&dialog);
    QSpinBox *spinX = new QSpinBox(&dialog);
    QSpinBox *spinY = new QSpinBox(&dialog);
    spinX->setRange(0, 800);
    spinY->setRange(0, 600);

    form.addRow("Nombre:", nombreEdit);
    form.addRow("Coordenada X:", spinX);
    form.addRow("Coordenada Y:", spinY);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString nombre = nombreEdit->text();
        int x = spinX->value();
        int y = spinY->value();

        if (nombre.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Advertencia", "El nombre no puede estar vacío.");
            return;
        }

        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO nodos (nombre, x, y) VALUES (:nombre, :x, :y)");
        insertQuery.bindValue(":nombre", nombre);
        insertQuery.bindValue(":x", x);
        insertQuery.bindValue(":y", y);

        if (!insertQuery.exec()) {
            QMessageBox::critical(this, "Error", "No se pudo crear el nodo: " + insertQuery.lastError().text());
        } else {
            cargarNodosDesdeBD();
        }
    }
}

void MainWindow::mostrarDialogoLeerNodos() {
    QDialog dialog(this);
    dialog.setWindowTitle("Lista de Nodos");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTableWidget *table = new QTableWidget(&dialog);

    QSqlQuery query("SELECT id, nombre, x, y FROM nodos ORDER BY id");
    int row = 0;
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Nombre", "X", "Y"});

    while (query.next()) {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value("nombre").toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value("x").toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value("y").toString()));
        row++;
    }

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(table);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    layout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    dialog.exec();
}

void MainWindow::mostrarDialogoActualizarNodo() {
    QDialog dialog(this);
    dialog.setWindowTitle("Actualizar Nodo");

    QFormLayout form(&dialog);
    QComboBox *comboNodos = new QComboBox(&dialog);

    QSqlQuery query("SELECT id, nombre FROM nodos ORDER BY nombre");
    while (query.next()) {
        comboNodos->addItem(query.value("nombre").toString(), query.value("id"));
    }

    QLineEdit *editNombre = new QLineEdit(&dialog);
    QSpinBox *spinX = new QSpinBox(&dialog);
    QSpinBox *spinY = new QSpinBox(&dialog);
    spinX->setRange(0, 1000);
    spinY->setRange(0, 1000);

    form.addRow("Nodo a actualizar:", comboNodos);
    form.addRow("Nuevo nombre:", editNombre);
    form.addRow("Nueva posición X:", spinX);
    form.addRow("Nueva posición Y:", spinY);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int id = comboNodos->currentData().toInt();
        QString nuevoNombre = editNombre->text().trimmed();
        int x = spinX->value();
        int y = spinY->value();

        if (nuevoNombre.isEmpty()) {
            QMessageBox::warning(this, "Advertencia", "El nombre no puede estar vacío.");
            return;
        }

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE nodos SET nombre = :nombre, x = :x, y = :y WHERE id = :id");
        updateQuery.bindValue(":nombre", nuevoNombre);
        updateQuery.bindValue(":x", x);
        updateQuery.bindValue(":y", y);
        updateQuery.bindValue(":id", id);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Error", "No se pudo actualizar el nodo: " + updateQuery.lastError().text());
            return;
        }

        cargarNodosDesdeBD();
    }
}

void MainWindow::mostrarDialogoEliminarNodo() {
    QDialog dialog(this);
    dialog.setWindowTitle("Eliminar Nodo");

    QFormLayout form(&dialog);
    QComboBox *comboNodos = new QComboBox(&dialog);

    QSqlQuery query("SELECT id, nombre FROM nodos ORDER BY nombre");
    while (query.next()) {
        comboNodos->addItem(query.value("nombre").toString(), query.value("id"));
    }

    form.addRow("Nodo a eliminar:", comboNodos);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int id = comboNodos->currentData().toInt();
        QString nombre = comboNodos->currentText();

        QMessageBox::StandardButton confirm = QMessageBox::question(
            this, "Confirmar Eliminación",
            QString("¿Estás seguro de que deseas eliminar el nodo '%1'? Se eliminarán también sus rutas.").arg(nombre),
            QMessageBox::Yes | QMessageBox::No
            );

        if (confirm == QMessageBox::Yes) {
            QSqlQuery deleteRutas;
            deleteRutas.prepare("DELETE FROM rutas WHERE origen_id = :id OR destino_id = :id");
            deleteRutas.bindValue(":id", id);
            deleteRutas.exec(); // Se eliminan rutas primero

            QSqlQuery deleteNodo;
            deleteNodo.prepare("DELETE FROM nodos WHERE id = :id");
            deleteNodo.bindValue(":id", id);

            if (!deleteNodo.exec()) {
                QMessageBox::critical(this, "Error", "No se pudo eliminar el nodo: " + deleteNodo.lastError().text());
                return;
            }

            cargarNodosDesdeBD();
        }
    }
}

void MainWindow::ejecutarBFS(int nodoInicial) {
    recorridoActual.clear();
    if (!mapaNodos.contains(nodoInicial)) return;

    std::queue<int> cola;
    QSet<int> visitados;

    cola.push(nodoInicial);
    visitados.insert(nodoInicial);

    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        recorridoActual.push_back(actual);

        QSqlQuery query;
        query.prepare("SELECT destino_id FROM rutas WHERE origen_id = :id");
        query.bindValue(":id", actual);

        if (query.exec()) {
            while (query.next()) {
                int vecino = query.value("destino_id").toInt();
                if (!visitados.contains(vecino)) {
                    visitados.insert(vecino);
                    cola.push(vecino);
                }
            }
        }
    }
}

void MainWindow::ejecutarDFS(int nodoInicial) {
    recorridoActual.clear();
    if (!mapaNodos.contains(nodoInicial)) return;

    QSet<int> visitados;
    std::stack<int> pila;

    pila.push(nodoInicial);
    visitados.insert(nodoInicial);

    while (!pila.empty()) {
        int actual = pila.top();
        pila.pop();
        recorridoActual.push_back(actual);

        QSqlQuery query;
        query.prepare("SELECT destino_id FROM rutas WHERE origen_id = :id ORDER BY destino_id DESC");
        query.bindValue(":id", actual);

        if (query.exec()) {
            while (query.next()) {
                int vecino = query.value("destino_id").toInt();
                if (!visitados.contains(vecino)) {
                    visitados.insert(vecino);
                    pila.push(vecino);
                }
            }
        }
    }
}

void MainWindow::visualizarRecorrido(const std::vector<int>& recorrido, const QString& tipo) {
    if (recorrido.empty()) return;

    scene->clear();
    cargarNodosDesdeBD();
    cargarRutasDesdeBD();

    visualizacionActual = tipo;

    QColor colorNodo(144, 238, 144); // Verde claro
    QColor colorLinea(255, 0, 255);  // Magenta

    for (int nodoId : recorrido) {
        if (mapaNodos.contains(nodoId)) {
            for (QGraphicsItem* item : mapaNodos[nodoId]->childItems()) {
                if (auto elipse = dynamic_cast<QGraphicsEllipseItem*>(item)) {
                    elipse->setBrush(colorNodo);
                }
            }
        }
    }

    indiceAnimacion = 0;
    recorridoActual = recorrido;
    visualizacionActual = tipo;

    QGraphicsTextItem* textoInfo = scene->addText(
        QString("Recorrido %1\n%2 nodos visitados")
            .arg(tipo)
            .arg(recorridoActual.size())
        );
    textoInfo->setPos(20, 20);
    textoInfo->setDefaultTextColor(Qt::darkBlue);
    textoInfo->setZValue(3);
    itemsAnimacion.append(textoInfo);

    timerAnimacion->start(1000);
}

void MainWindow::avanzarAnimacion() {
    if (indiceAnimacion >= static_cast<int>(recorridoActual.size()) - 1) {
        timerAnimacion->stop();
        return;
    }

    int actual = recorridoActual[indiceAnimacion];
    int siguiente = recorridoActual[indiceAnimacion + 1];

    if (mapaNodos.contains(actual) && mapaNodos.contains(siguiente)) {
        QPointF p1 = mapaNodos[actual]->sceneBoundingRect().center();
        QPointF p2 = mapaNodos[siguiente]->sceneBoundingRect().center();

        QPen penAnimacion(QColor(255, 0, 255), 3);
        QGraphicsLineItem* linea = scene->addLine(QLineF(p1, p2), penAnimacion);
        linea->setZValue(1);
        itemsAnimacion.append(linea);

        double distancia = QLineF(p1, p2).length();
        double tiempo = distancia / 50.0;

        QString info = (visualizacionActual == "Mostrar distancia")
                           ? QString("Dist: %1 u").arg(distancia, 0, 'f', 1)
                           : QString("Tiempo: %1 min").arg(tiempo, 0, 'f', 1);

        QPointF midPoint = (p1 + p2) / 2;  // Definir midPoint aquÃ­
        QGraphicsTextItem* texto = scene->addText(info);
        texto->setPos(midPoint.x() - 30, midPoint.y() - 20);
        texto->setDefaultTextColor(Qt::blue);
        texto->setZValue(2);
        itemsAnimacion.append(texto);
    }

    indiceAnimacion++;
}

void MainWindow::limpiarVisualizacion() {
    timerAnimacion->stop();
    for (QGraphicsItem* item : itemsAnimacion) {
        scene->removeItem(item);
        delete item;
    }
    itemsAnimacion.clear();
    recorridoActual.clear();
    indiceAnimacion = 0;
}
