#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSqlDatabase>
#include <QMap>
#include <QGraphicsItemGroup>
#include <vector>
#include <queue>
#include <utility> // Para std::pair
#include <QDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSpinBox> // NECESARIO para crear nodos
#include <QDebug>   // Opcional: útil para depuración

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    // Elementos de la UI
    QWidget *centralWidget;
    QPushButton *btnCrearNodo;
    QPushButton *btnLeerNodos;
    QPushButton *btnActualizarNodo;
    QPushButton *btnEliminarNodoDB;
    QPushButton *btnCrear;
    QPushButton *btnModificar;
    QPushButton *btnEliminar;
    QPushButton *btnRutaMinima;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QSqlDatabase db;

    // Mapeo de nodos
    QMap<int, QGraphicsItemGroup*> mapaNodos;
    QMap<QGraphicsItemGroup*, int> nodoPorItem;

    // Variables de estado para selecciÃ³n
    QGraphicsItemGroup* primerNodoSeleccionado = nullptr;
    QGraphicsItemGroup* segundoNodoSeleccionado = nullptr;
    QGraphicsItemGroup* nodoOrigenMod = nullptr;
    QGraphicsItemGroup* nodoDestinoActual = nullptr;
    QGraphicsItemGroup* nodoNuevoDestino = nullptr;
    QGraphicsItemGroup* nodoEliminar1 = nullptr;
    QGraphicsItemGroup* nodoEliminar2 = nullptr;

    enum ModoSeleccion { Ninguno, CrearRuta, ModificarRuta, EliminarRuta };
    ModoSeleccion modoActual = Ninguno;

    // Funciones de configuraciÃ³n
    void mostrarDialogoCrearNodo();
    void mostrarDialogoLeerNodos();
    void mostrarDialogoActualizarNodo();
    void mostrarDialogoEliminarNodo();
    void configurarUI();
    void conectarBaseDeDatos();
    void cargarNodosDesdeBD();
    void cargarRutasDesdeBD();

    // Funciones de operaciones con rutas
    void crearRuta();
    void eliminarRuta();
    void modificarRuta();
    void resetSeleccionModificacion();

    // Funciones para el algoritmo de ruta mÃ­nima
    std::vector<std::vector<std::pair<int, int>>> construirGrafo();
    std::pair<std::vector<int>, int> calcularRutaMinima(int origen, int destino);
    void mostrarRutaMinima(const std::vector<int>& ruta, int distancia);
    void mostrarDialogoRutaMinima();

    // Para BFS/DFS
    QPushButton *btnRecorrido;  // DeclaraciÃ³n faltante
    QString visualizacionActual; // Variable para almacenar el tipo de visualizaciÃ³n
    void mostrarDialogoRecorrido();
    void ejecutarBFS(int nodoInicial);
    void ejecutarDFS(int nodoInicial);
    void visualizarRecorrido(const std::vector<int>& recorrido, const QString& tipo);
    void limpiarVisualizacion();
    void avanzarAnimacion();    // DeclaraciÃ³n faltante

    // Variables para animaciÃ³n
    QTimer* timerAnimacion;
    int indiceAnimacion;
    std::vector<int> recorridoActual;
    QList<QGraphicsItem*> itemsAnimacion;

};

#endif // MAINWINDOW_H
