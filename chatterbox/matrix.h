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

private:
    QHBoxLayout* input_param; //params

    QGridLayout* eq_coef_layout; //coeffs of equations
    QVBoxLayout* eq_sign_layout; //eq sign layout
    QVBoxLayout* res_layout; //equation result layout
    QHBoxLayout* eq_layout; //Equation Layout
    QHBoxLayout* button_layout; //Ok and quit Button
    QVBoxLayout* all_layout; //Master-Layout
    QPushButton* quitButton;
    QPushButton* Ok;
    QSpinBox* spin1; //eq number
    QSpinBox* spin2; //var number
    QLabel* label1; //param label
    QLabel* label2; // ""
    int previousValue; //holds the previous value of the spin1/spin2

    QWidget* first; //global ptr for taborder definition

    //Variables for solve
    int n;
    double** a;
    double* b;
    double* x;

    
signals:
    
public slots:
    void changeContent(int val);
    void calc();
    
};

#endif // MATRIX_H
