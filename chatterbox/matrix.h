#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>
#include <QtGui>
#include <QMessageBox>

class matrix : public QDialog
{
    Q_OBJECT
public:
    matrix(QWidget *parent = 0);
    QString getMatrixString() const;

    static QString MatrixDlg();

private:
    QHBoxLayout* input_param; //params
    QGridLayout* eq_coef_layout; //coeffs of equations
    QVBoxLayout* eq_sign_layout; //eq sign layout
    QVBoxLayout* res_layout; //equation result layout
    QHBoxLayout* eq_layout; //Equation Layout

    QLineEdit *nameEdit;

    int currentSize; //holds the current value of the spin1/spin2
    
signals:
    
public slots:
    void changeContent(int val);
    //void calc();
    
};

#endif // MATRIX_H
