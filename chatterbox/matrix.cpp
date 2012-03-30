#include "matrix.h"
#include "ui_matrix.h"

Matrix::Matrix(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Matrix)
{
    ui->setupUi(this);
}

Matrix::~Matrix()
{
    delete ui;
}

void Matrix::on_n1_valueChanged(int arg1)
{
    ui->n2->setValue(arg1);
}

void Matrix::on_n2_valueChanged(int arg1)
{
    ui->n1->setValue(arg1);
}
