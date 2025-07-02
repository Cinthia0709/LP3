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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    enum ModoSeleccion {
        Ninguno,
        CrearRuta,
        ModificarRuta,
        EliminarRuta,
        CrearNodo,
        ModificarNodo,
        EliminarNodo
    };
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    ModoSeleccion modoActual;
    QWidget *centralWidget;
    QPushButton *btnCrear;
    QPushButton *btnModificar;
    QPushButton *btnEliminar;
    QPushButton *btnRutaMinima;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QSqlDatabase db;
    QPushButton *btnCrearNodo;
    QPushButton *btnModificarNodo;
    QPushButton *btnEliminarNodo;



    QMap<int, QGraphicsItemGroup*> mapaNodos;
    QMap<QGraphicsItemGroup*, int> nodoPorItem;


    QGraphicsItemGroup* primerNodoSeleccionado = nullptr;
    QGraphicsItemGroup* segundoNodoSeleccionado = nullptr;
    QGraphicsItemGroup* nodoOrigenMod = nullptr;
    QGraphicsItemGroup* nodoDestinoActual = nullptr;
    QGraphicsItemGroup* nodoNuevoDestino = nullptr;
    QGraphicsItemGroup* nodoEliminar1 = nullptr;
    QGraphicsItemGroup* nodoEliminar2 = nullptr;




    void configurarUI();
    void conectarBaseDeDatos();
    void cargarNodosDesdeBD();
    void cargarRutasDesdeBD();
    void crearNodo();
    void modificarNodo();
    void eliminarNodo();


    void crearRuta();
    void eliminarRuta();
    void modificarRuta();
    void resetSeleccionModificacion();

    std::vector<std::vector<std::pair<int, int>>> construirGrafo();
    std::pair<std::vector<int>, int> calcularRutaMinima(int origen, int destino);
    void mostrarRutaMinima(const std::vector<int>& ruta, int distancia);
    void mostrarDialogoRutaMinima();

    QPushButton *btnRecorrido;
    QString visualizacionActual;
    void mostrarDialogoRecorrido();
    void ejecutarBFS(int nodoInicial);
    void ejecutarDFS(int nodoInicial);
    void visualizarRecorrido(const std::vector<int>& recorrido, const QString& tipo);
    void limpiarVisualizacion();
    void avanzarAnimacion();

    QTimer* timerAnimacion;
    int indiceAnimacion;
    std::vector<int> recorridoActual;
    QList<QGraphicsItem*> itemsAnimacion;

};

#endif // MAINWINDOW_H
