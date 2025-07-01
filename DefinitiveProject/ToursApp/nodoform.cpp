#include "nodoform.h"
#include "ui_nodoform.h"

NodoForm::NodoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NodoForm)
{
    ui->setupUi(this);

    modelo = new QSqlTableModel(this);
    modelo->setTable("nodos");
    modelo->select();
    ui->tablaNodos->setModel(modelo);

    connect(ui->btnGuardar, &QPushButton::clicked, this, &NodoForm::guardarNodo);
    connect(ui->btnEliminar, &QPushButton::clicked, this, &NodoForm::eliminarNodo);
    connect(ui->tablaNodos, &QTableView::clicked, this, &NodoForm::seleccionarNodo);
}

NodoForm::~NodoForm()
{
    delete ui;
}

void NodoForm::cargarNodos()
{
    modelo->select();
}

void NodoForm::guardarNodo()
{
    QSqlQuery query;

    if (idSeleccionado == -1) {
        query.prepare("INSERT INTO nodos (nombre, descripcion, tipo, x, y, precio, popular) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)");
    } else {
        query.prepare("UPDATE nodos SET nombre=?, descripcion=?, tipo=?, x=?, y=?, precio=?, popular=? "
                      "WHERE id=?");
    }

    query.addBindValue(ui->lineNombre->text());
    query.addBindValue(ui->textDescripcion->toPlainText());
    query.addBindValue(ui->comboTipo->currentText());
    query.addBindValue(ui->spinX->value());
    query.addBindValue(ui->spinY->value());
    query.addBindValue(ui->spinPrecio->value());
    query.addBindValue(ui->checkPopular->isChecked() ? 1 : 0);

    if (idSeleccionado != -1)
        query.addBindValue(idSeleccionado);

    if (!query.exec())
        qDebug() << "Error al guardar nodo:" << query.lastError();

    idSeleccionado = -1;
    cargarNodos();
}

void NodoForm::eliminarNodo()
{
    if (idSeleccionado == -1) return;

    QSqlQuery query;
    query.prepare("DELETE FROM nodos WHERE id=?");
    query.addBindValue(idSeleccionado);

    if (!query.exec())
        qDebug() << "Error al eliminar nodo:" << query.lastError();

    idSeleccionado = -1;
    cargarNodos();
}

void NodoForm::seleccionarNodo(const QModelIndex &index)
{
    int row = index.row();
    idSeleccionado = modelo->data(modelo->index(row, 0)).toInt();
    ui->lineNombre->setText(modelo->data(modelo->index(row, 1)).toString());
    ui->textDescripcion->setText(modelo->data(modelo->index(row, 2)).toString());
    ui->comboTipo->setCurrentText(modelo->data(modelo->index(row, 3)).toString());
    ui->spinX->setValue(modelo->data(modelo->index(row, 4)).toInt());
    ui->spinY->setValue(modelo->data(modelo->index(row, 5)).toInt());
    ui->spinPrecio->setValue(modelo->data(modelo->index(row, 6)).toDouble());
    ui->checkPopular->setChecked(modelo->data(modelo->index(row, 7)).toInt() == 1);
}
