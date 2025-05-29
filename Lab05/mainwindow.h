#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QVector>
#include <QVBoxLayout>
#include <QElapsedTimer>
#include <qpushbutton.h>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void iniciarCarrera();
    void actualizarCarrera();

private:
    QVector<QLabel*> caballos;
    QVector<QElapsedTimer*> tiempos;
    QVector<bool> llegados;
    QTimer *timer;
    QVBoxLayout *layout;
    int cantidadCaballos;
    const int meta = 200;

    QPushButton *btnIniciar;
    bool carreraEnCurso = false;

    void extracted();
    void crearCaballos(int cantidad);
};

#endif // MAINWINDOW_H

