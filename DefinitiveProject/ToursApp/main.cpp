#include <QApplication>
#include <QWidget>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Ventana principal
    QWidget window;
    window.setWindowTitle("Interfaz Rutas");

    // Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout();

    // Parte izquierda: Grupo de botones con título
    QGroupBox *groupBox = new QGroupBox("RUTAS");
    QVBoxLayout *buttonLayout = new QVBoxLayout();

    QPushButton *btnCrear = new QPushButton("CREAR");
    QPushButton *btnModificar = new QPushButton("MODIFICAR");
    QPushButton *btnEliminar = new QPushButton("ELIMINAR");

    buttonLayout->addWidget(btnCrear);
    buttonLayout->addWidget(btnModificar);
    buttonLayout->addWidget(btnEliminar);
    buttonLayout->addStretch();

    groupBox->setLayout(buttonLayout);

    // Parte derecha: Graphics View
    QGraphicsView *graphicsView = new QGraphicsView();
    graphicsView->setMinimumSize(400, 300);

    // Añadir al layout principal
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(graphicsView);

    // Asignar layout a la ventana
    window.setLayout(mainLayout);
    window.resize(600, 400);
    window.show();

    return app.exec();
}
