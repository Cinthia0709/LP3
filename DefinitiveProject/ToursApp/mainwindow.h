#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSqlDatabase>
#include <QMap>
#include <QGraphicsItemGroup>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QPushButton *btnCrear;
    QPushButton *btnModificar;
    QPushButton *btnEliminar;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QSqlDatabase db;

    QMap<int, QGraphicsItemGroup*> mapaNodos;
    QGraphicsItemGroup* primerNodoSeleccionado = nullptr;
    QGraphicsItemGroup* segundoNodoSeleccionado = nullptr;
    QGraphicsItemGroup* nodoOrigenMod = nullptr;
    QGraphicsItemGroup* nodoDestinoActual = nullptr;
    QGraphicsItemGroup* nodoNuevoDestino = nullptr;
    QMap<QGraphicsItemGroup*, int> nodoPorItem;
    QGraphicsItemGroup* nodoEliminar1 = nullptr;
    QGraphicsItemGroup* nodoEliminar2 = nullptr;

    void configurarUI();
    void conectarBaseDeDatos();
    void cargarNodosDesdeBD();
    void cargarRutasDesdeBD();
    void crearRuta();
    void eliminarRuta();
    void modificarRuta();
    void resetSeleccionModificacion();
    bool eventFilter(QObject *obj, QEvent *event) override;
    enum ModoSeleccion { Ninguno, CrearRuta, ModificarRuta, EliminarRuta };
    ModoSeleccion modoActual = Ninguno;

};

#endif // MAINWINDOW_H
