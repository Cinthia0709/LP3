#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Nodo {
    QString nombre;
    QPointF posicion;
};

struct Conexion {
    int desde;
    int hasta;
    double distancia;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* escena;

    QVector<Nodo> nodos;
    QVector<Conexion> conexiones;

    void crearNodos();
    void crearConexiones();
    void dibujarMapa();
};
#endif // MAINWINDOW_H
