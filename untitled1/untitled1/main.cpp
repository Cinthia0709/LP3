/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación de carrera de caballos en Qt.
 */

#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Función principal.
 * @param argc Número de argumentos.
 * @param argv Lista de argumentos.
 * @return Código de salida.
 */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
