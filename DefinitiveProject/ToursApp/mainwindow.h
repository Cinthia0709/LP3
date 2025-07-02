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
    QMap<QGraphicsItemGroup*, int> nodoPorItem;

    void configurarUI();
    void conectarBaseDeDatos();
    void cargarNodosDesdeBD();
    void cargarRutasDesdeBD();
    void crearRuta();
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // MAINWINDOW_H
