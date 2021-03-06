#include "matrix.h"

matrix::matrix(QWidget *parent) :
    QDialog(parent)
{
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->currentSize = 1;

    //input-parameter
    QSpinBox* spin1 = new QSpinBox;
    spin1->setMinimum(1);
    spin1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    spin1->setKeyboardTracking(false);

    QLabel* label1 = new QLabel;
    label1->setText("Gleichungen mit");
    label1->setAlignment(Qt::AlignCenter);

    QSpinBox* spin2 = new QSpinBox;
    spin2->setMinimum(1);
    spin2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QLabel* label2 = new QLabel;
    label2->setText("Unbekannten");
    label2->setAlignment(Qt::AlignCenter);

    QHBoxLayout *input_param = new QHBoxLayout;
    input_param->addWidget(spin1);
    input_param->addWidget(label1);
    input_param->addWidget(spin2);
    input_param->addWidget(label2);


    //initial Equation
    QDoubleSpinBox* initial_value = new QDoubleSpinBox;
    initial_value->setSingleStep(0.1);
    initial_value->setLocale(QLocale::C);
    QLabel* initial_letter = new QLabel;
    QLabel* eq_sign = new QLabel;
    initial_letter->setText("a");
    initial_letter->setAlignment(Qt::AlignLeft);
    eq_sign->setText("=");
    eq_sign->setAlignment(Qt::AlignCenter);
    QDoubleSpinBox* initial_result = new QDoubleSpinBox;
    initial_result->setSingleStep(0.1);
    initial_result->setLocale(QLocale::C);
    eq_coef_layout = new QGridLayout;
    eq_sign_layout = new QVBoxLayout;
    res_layout = new QVBoxLayout;
    eq_coef_layout->addWidget(initial_value,0,0);
    eq_coef_layout->addWidget(initial_letter,0,1);
    eq_sign_layout->addWidget(eq_sign);
    res_layout->addWidget(initial_result);
    eq_layout = new QHBoxLayout;
    eq_layout->addLayout(eq_coef_layout);
    eq_layout->addLayout(eq_sign_layout);
    eq_layout->addLayout(res_layout);

    //control Buttons
    QPushButton *quitButton = new QPushButton("Abbrechen");
    QPushButton *Ok = new QPushButton("Ok");
    Ok->setDefault(true);
    QHBoxLayout *button_layout = new QHBoxLayout;
    button_layout->addWidget(Ok);
    button_layout->addWidget(quitButton);


    //Master-Layout
    QVBoxLayout *all_layout = new QVBoxLayout;
    all_layout->addLayout(input_param);
    all_layout->addLayout(eq_layout);
    all_layout->addLayout(button_layout);
    setLayout(all_layout);
    setWindowTitle("Matrix");

    /*setTabOrder(spin1,spin2);
    setTabOrder(spin2,initial_value);
    setTabOrder(initial_value,initial_result);
    setTabOrder(initial_result,Ok);
    setTabOrder(Ok,quitButton);*/

    connect(spin1,SIGNAL(valueChanged(int)),spin2, SLOT(setValue(int)));
    connect(spin1,SIGNAL(valueChanged(int)),this, SLOT(changeContent(int)));
    connect(spin2,SIGNAL(valueChanged(int)),spin1,SLOT(setValue(int)));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(reject()));
    connect(Ok,SIGNAL(clicked()),this,SLOT(accept()));
}



void matrix::changeContent(int val)
{
    if(val > currentSize)
    {
        //new line
        QDoubleSpinBox* newInput = new QDoubleSpinBox;
        newInput->setSingleStep(0.1);
        newInput->setLocale(QLocale::C);
        this->eq_coef_layout->addWidget(newInput,val-1,0);
        QLabel* newA = new QLabel;
        newA->setText("a");
        newA->setAlignment(Qt::AlignCenter);
        this->eq_coef_layout->addWidget(newA,val-1,1);

        int pos;
        for(int i = 2; i < val; i++) //i = column
        {
            pos = 3 * i - 4;


            QDoubleSpinBox* newSpin = new QDoubleSpinBox;
            newSpin->setSingleStep(0.1);
            newSpin->setLocale(QLocale::C);

            QLabel* newMathSign = new QLabel;
            newMathSign->setText("+");
            newMathSign->setAlignment(Qt::AlignCenter);

            this->eq_coef_layout->addWidget(newMathSign,val-1,pos);
            this->eq_coef_layout->addWidget(newSpin,val-1,pos+1);
            this->eq_coef_layout->addWidget(new QLabel((QString)(96+i)),val-1,pos+2);
        }


        //new stuff next to the new coeff
        QLabel* newLineEq = new QLabel; //new eqSign for new Equation
        newLineEq->setText("=");
        newLineEq->setAlignment(Qt::AlignCenter);
        this->eq_sign_layout->addWidget(newLineEq);
        QDoubleSpinBox* newRes = new QDoubleSpinBox;
        newRes->setSingleStep(0.1);
        newRes->setLocale(QLocale::C);
        this->res_layout->addWidget(newRes);


        int eq_pos = 3 * val - 4;

        for(int i = 0; i < val; i++) //i = row
        {
            QDoubleSpinBox* newSpin = new QDoubleSpinBox;
            newSpin->setSingleStep(0.1);
            newSpin->setLocale(QLocale::C);

            QLabel* newMathSign = new QLabel;
            newMathSign->setText("+");
            newMathSign->setAlignment(Qt::AlignCenter);

            QLabel* newLetter = new QLabel;
            QString temp = "";
            temp.append((char)97+(val-1));
            newLetter->setText(temp);
            temp.clear();

            this->eq_coef_layout->addWidget(newMathSign,i,eq_pos);
            this->eq_coef_layout->addWidget(newSpin,i,eq_pos+1);
            this->eq_coef_layout->addWidget(newLetter,i,eq_pos+2);
        }


        //new setTabOrder defs: LARS GUCK HIER
        /*QString current_className;
        QWidget* current_widget;
        for(int i = 0; i < val; i++)
        {
            for(int j = 0,len = 0; j < val; j++)
            {
                current_widget = eq_coef_layout->itemAtPosition(i,len)->widget();
                current_className = current_widget->metaObject()->className();
                if(current_className == "QDoubleSpinBox")
                {
                    setTabOrder(first,current_widget);
                    first = current_widget;
                    len+=3;
                }
            }
            current_widget = res_layout->itemAt(i)->widget();
            setTabOrder(first,current_widget);
            first = current_widget;
        }*/






    }


    else //if increase == false
    {
        //delete last line
        int length = (val+1)*2+(val)+2;
        QWidget *tmp_widget;
        for(int i = 0; i < length; i++)
        {
            if(i < length-2)
            {
                tmp_widget = eq_coef_layout->itemAtPosition(val,i)->widget();
                eq_coef_layout->removeWidget(tmp_widget);
                delete tmp_widget;
            }
            else if(i == length-2)
            {
                tmp_widget = eq_sign_layout->itemAt(val)->widget();
                eq_sign_layout->removeWidget(tmp_widget);
                delete tmp_widget;
            }
            else
            {
                tmp_widget = res_layout->itemAt(val)->widget();
                res_layout->removeWidget(tmp_widget);
                delete tmp_widget;
            }

        }

        //delete last elements of equation
        int eq_pos = ((val+1)*2+(val))-1;
        for(int i = 0; i < val; i++) //i = row; last row has been deleted
        {
            //if((eq_coef_layout->itemAtPosition(i,eq_pos-2) != 0) && (eq_coef_layout->itemAtPosition(i,eq_pos-1) != 0) && (eq_coef_layout->itemAtPosition(i,eq_pos) != 0))
            //{
                tmp_widget = eq_coef_layout->itemAtPosition(i,eq_pos-2)->widget();
                eq_coef_layout->removeWidget(tmp_widget);
                delete tmp_widget;
                tmp_widget = eq_coef_layout->itemAtPosition(i,eq_pos-1)->widget();
                eq_coef_layout->removeWidget(tmp_widget);
                delete tmp_widget;
                tmp_widget = eq_coef_layout->itemAtPosition(i,eq_pos)->widget();
                eq_coef_layout->removeWidget(tmp_widget);
                delete tmp_widget;
            //}
        }

        this->adjustSize();
    }
    this->currentSize = val;
}

QString matrix::getMatrixString() const {
    QString matrixstr;
    for (int i = 0; i < this->currentSize; i++) {
        for (int j = 0; j < this->currentSize; j++) {
            matrixstr += qobject_cast<QDoubleSpinBox *>(this->eq_coef_layout->itemAtPosition(i, j * 3)->widget())->text() + ",";
        }
        matrixstr += qobject_cast<QDoubleSpinBox *>(this->res_layout->itemAt(i)->widget())->text();
        if (i < this->currentSize - 1) matrixstr += "|";
    }
    return matrixstr;
}

QString matrix::MatrixDlg() {
    matrix MDlg;
    if (MDlg.exec() == QDialog::Accepted) {
        return MDlg.getMatrixString();
    }
    else return QString();
}




/*void matrix::calc()
{
    this->n = this->currentSize;
    this->x = new double[this->n];
    for(int i = 0; i < this->n; i ++)
    {
        this->x[i] = 0;
    }

    this->a = new double*[this->n];
    for(int i = 0; i < this->n; i++)
    {
        this->a[i] = new double[this->n];
    }

    this->b = new double[this->n];


    QWidget* current_widget;
    QDoubleSpinBox* current_spinbox;
    QString current_className;
    for(int i = 0; i < this->n; i++)
    {
        for(int j = 0,len = 0; j < this->n; j++)
        {
            current_widget = eq_coef_layout->itemAtPosition(i,len)->widget();
            current_className = current_widget->metaObject()->className();
            if(current_className == "QDoubleSpinBox")
            {
                current_spinbox = qobject_cast<QDoubleSpinBox *>(current_widget);
                this->a[i][j] = current_spinbox->value();
                len+=3;
            }
        }
        current_widget = res_layout->itemAt(i)->widget();
        current_spinbox = qobject_cast<QDoubleSpinBox *>(current_widget);
        this->b[i] = current_spinbox->value();
    }

    double f;

    //Dreiecksmatrix bestimmen
    for(int i = 0; i < this->n; i++)
    {
        for(int j = i+1; j < this->n; j++)
        {
            f = this->a[j][i]/this->a[i][i];

            for(int k = 0; k < this->n; k++)
            {
                this->a[j][k] = this->a[j][k] - f*this->a[i][k];
            }

            this->b[j] = this->b[j] - f*this->b[i];
        }
    }

    //Rueckwaertseinsetzen
    for(int i = this->n-1; i >= 0; i--)
    {
        double z = this->b[i]; //z=letztes Spaltenskalar

        for(int j = i+1; j < this->n; j++)
        {
            z = z - (this->x[j]*this->a[i][j]); //Subtrahieren von vorhanden x-Werten * sein Koeffizient
        }

        if(this->a[i][i] == 0)
        {
            int ret = QMessageBox::critical(this, "Fehler", "Das eingebene lineare Gleichungssystem ist nicht eindeutig l�sbar!", QMessageBox::Ok, QMessageBox::Close);
            if (ret == QMessageBox::Ok)
            {
                return;
            }
            else
            {
                QApplication::quit();
            }
        }
        this->x[i] = z/this->a[i][i];
    }


    QString result = "";

    for(int i = 0; i < this->n; i++)
    {
        result.append((char)(i+97));
        result.append(" = ");
        QString temp;
        temp.setNum(this->x[i]);
        result += temp;
        result.append("\n");
    }


    QMessageBox::information(this,"Ergebnisse",result);

}*/
