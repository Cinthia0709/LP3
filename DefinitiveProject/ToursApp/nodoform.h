#ifndef NODOFORM_H
#define NODOFORM_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class NodoForm;
}

class NodoForm : public QWidget
{
    Q_OBJECT

public:
    explicit NodoForm(QWidget *parent = nullptr);
    ~NodoForm();

private slots:
    void cargarNodos();
    void guardarNodo();
    void eliminarNodo();
    void seleccionarNodo(const QModelIndex &index);

private:
    Ui::NodoForm *ui;
    QSqlTableModel *modelo;
    int idSeleccionado = -1;
};

#endif // NODOFORM_H
