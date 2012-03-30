#ifndef MATRIX_H
#define MATRIX_H

#include <QDialog>

namespace Ui {
class Matrix;
}

class Matrix : public QDialog
{
    Q_OBJECT
    
public:
    explicit Matrix(QWidget *parent = 0);
    ~Matrix();
    
private slots:
    void on_n1_valueChanged(int arg1);

    void on_n2_valueChanged(int arg1);

private:
    Ui::Matrix *ui;
};

#endif // MATRIX_H
