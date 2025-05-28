#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>

/**
 * @brief La clase MainWindow representa la ventana principal de la carrera de caballos.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase MainWindow.
     * @param parent El widget padre (por defecto es nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /// Destructor de la clase MainWindow.
    ~MainWindow();

private slots:
    /// Inicia la carrera de caballos.
    void iniciarCarrera();

    /// Actualiza la posición de los caballos durante la carrera.
    void actualizarCarrera();

private:
    QLabel *caballo1;
    QLabel *caballo2;
    QLabel *caballo3;
    QLabel *caballo4;
    QTimer *timer;
};

#endif // MAINWINDOW_H
