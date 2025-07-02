#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QMap>
#include <QSqlDatabase>

enum Modo { Ninguno, CrearNodo, ModificarNodo, EliminarNodo,
            CrearRuta, ModificarRuta, EliminarRuta };

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QPushButton *btnCrearNodo, *btnModificarNodo, *btnEliminarNodo;
    QPushButton *btnCrearRuta, *btnModificarRuta, *btnEliminarRuta;

    QGraphicsItemGroup *nodoSeleccionado = nullptr;
    QGraphicsItemGroup *primerNodoSeleccionado = nullptr;
    QGraphicsItemGroup *segundoNodoSeleccionado = nullptr;
    QGraphicsItemGroup *nodoOrigenMod = nullptr;
    QGraphicsItemGroup *nodoDestinoActual = nullptr;
    QGraphicsItemGroup *nodoNuevoDestino = nullptr;
    QGraphicsItemGroup *nodoEliminar1 = nullptr;
    QGraphicsItemGroup *nodoEliminar2 = nullptr;

    QMap<QGraphicsItemGroup*, int> nodoPorItem;

    QSqlDatabase db;
    Modo modoActual = Ninguno;

    void configurarUI();
    void conectarBaseDeDatos();
    void cargarNodosDesdeBD();
    void crearNodo(const QPointF &pos);
    void modificarNodo();
    void eliminarNodo();
    void crearRuta();
    void modificarRuta();
    void eliminarRuta();
    void resetSeleccionNodos();
};

#endif // MAINWINDOW_H
