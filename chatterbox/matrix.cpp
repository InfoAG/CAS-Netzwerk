#include "matrix.h"

matrix::matrix(QWidget *parent, QPoint initial_pos) :
    QDialog(0, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    this->current_pos = initial_pos;
    //this->setModal(false);
    //this->move(this->mapFromGlobal(initial_pos).x(), this->mapFromGlobal(initial_pos).y());
    this->move(initial_pos.x()-80,initial_pos.y()-330);
    qDebug() << this->height();
    QPushButton* accepted_button = new QPushButton;
    accepted_button->setText(tr("Ok"));
    accepted_button->setDefault(true);
    QPushButton* quit_button = new QPushButton;
    quit_button->setText(tr("Quit"));
    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->addWidget(accepted_button);
    button_layout->addWidget(quit_button);

    //achtung unschoener programmierstil
    //textedit = new QTextEdit;
    textedit  =static_cast<QTextEdit*>(parent);
    connect(textedit,SIGNAL(textChanged()),this,SLOT(moveAnimation()));

    mainLayout = new QVBoxLayout;
    tblv = new QTableView();
    //installEventFilter(this);

    mainLayout->addWidget(tblv);
    mainLayout->addLayout(button_layout);
    mainLayout->addSpacerItem(new QSpacerItem(0, 33, QSizePolicy::Maximum, QSizePolicy::Maximum));
    setLayout(mainLayout);

    model = new QStandardItemModel(1,1,this);

    model->setItem(0, 0, new QStandardItem(QString("asgsdga")));

    tblv->setModel(model);

    connect(quit_button,SIGNAL(clicked()),this,SLOT(reject()));


    /*QPixmap pixmap(this->sizeHint());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundRect(pixmap.rect());
    setMask(pixmap.createMaskFromColor(Qt::white));*/

//    tblv->setVisible(false);
//    tblv->resizeColumnsToContents();
//    tblv->resizeRowsToContents();
//    tblv->setVisible(true);


    //this->adjustSize();

    //this->setStyleSheet("QWidget{margin-bottom: 40px;}");

}

void matrix::moveAnimation()
{
    qDebug() << this->textedit->mapToGlobal(this->textedit->cursorRect().bottomLeft()).x();
    qDebug() << this->textedit->mapToGlobal(this->textedit->cursorRect().bottomLeft()).y();

    QPropertyAnimation animation(this, "geometry");

    /*animation.setDuration(10000);
    //animation.setEasingCurve(QEasingCurve::OutCubic);
    animation.setStartValue(QRect(this->current_pos.x()-80, this->current_pos.y()-330, width(), height()));

    //this->current_pos = this->textedit->mapToGlobal(this->textedit->cursorRect().bottomLeft());

    //animation.setEndValue(QRect(this->current_pos.x()-80, this->current_pos.y()-330, width(), height()));
    animation.setEndValue(QRect(0, 0, width(), height()));


    animation.start();*/


    animation.setDuration(10000);
     animation.setStartValue(QRect(0, 0, 100, 30));
     animation.setEndValue(QRect(250, 250, 100, 30));

     animation.start();

}




void matrix::mouseMoveEvent(QMouseEvent *event)
{
    /*if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }*/
    event->ignore();
}

void matrix::mousePressEvent(QMouseEvent *event)
{
    /*if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }*/
    event->ignore();
}

void matrix::paintEvent(QPaintEvent*)
{

    const QPoint start(width()/2, 0);
    const QPoint finalStop(width()/2, height());
    QLinearGradient gradient(start, finalStop);
    const QColor qtGreen(102, 176, 54);

    gradient.setColorAt(0, qtGreen.dark());
    gradient.setColorAt(0.5, qtGreen);
    gradient.setColorAt(1, qtGreen.dark());

    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), gradient);

    QFont headerFont("Sans Serif", 12, QFont::Bold);
    QFont normalFont("Sans Serif", 9, QFont::Normal);

    //shadoweffect: draw 2 times
    painter.setFont(headerFont);
    QRect headerRect(1, 1, width(), 25);
    headerRect.moveTopLeft(QPoint(0, 0)); //go to topleft position 0/0
    painter.setPen(Qt::white);
    painter.drawText(headerRect, Qt::AlignCenter, "Matrix"); //and write text in the center
    painter.setFont(normalFont);

}

void matrix::resizeEvent(QResizeEvent*)
{
    const QColor qtGreen(102, 176, 54);

    static const QPointF points[3] = {
             QPointF(100.0, height()- 40),
             QPointF(150.0, height()- 40),
            QPointF(80.0, height())
    };

    QBitmap maskBitmap(width(), height());
    maskBitmap.clear();

    QPainter p(&maskBitmap);
    p.setBrush(Qt::black);
    p.drawRoundRect(0, 0, width(), height() - 40, 20, 30);
    p.drawPolygon(points, 3);
    p.end();

    setMask(maskBitmap);
}




void matrix::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Down)
    {
        qDebug() << "Du hast einen Pfeil bedient";

        this->model->insertRow(1);
        this->adjustSize();
    }

    if(e->key() == Qt::Key_Up)
    {
        qDebug() << "Du hast einen anderen Pfeil bedient";

        this->model->insertColumn(1);
        this->adjustSize();
    }

}


/*bool matrix::eventFilter(QObject* object, QEvent* event)
{
    if (event->type()==QEvent::KeyPress)
    {
        QKeyEvent* pKeyEvent=static_cast<QKeyEvent*>(event);
        if(pKeyEvent->key() == Qt::Key_Right)
        {
            if (tblv->hasFocus())
            {
                qDebug() << "Event filter: RightArrow";
            }
            return TRUE;
        }
        else if(pKeyEvent->key() == Qt::Key_Down)
        {
            if (tblv->hasFocus())
            {
                qDebug() << "Event filter: DownArrow";
            }
            return TRUE;
        }
    }

    return QWidget::eventFilter(object, event);
}*/



void matrix::expandModel()
{
//insertcolumns()
//

}




/*QString matrix::getMatrixString() const {
    QString matrixstr;
    for (int i = 0; i < this->currentSize; i++) {
        for (int j = 0; j < this->currentSize; j++) {
            matrixstr += qobject_cast<QDoubleSpinBox *>(this->eq_coef_layout->itemAtPosition(i, j * 3)->widget())->text() + ",";
        }
        matrixstr += qobject_cast<QDoubleSpinBox *>(this->res_layout->itemAt(i)->widget())->text();
        if (i < this->currentSize - 1) matrixstr += "|";
    }
    return matrixstr;
}*/

QString matrix::MatrixDlg(QWidget *parent, QPoint initial_pos) {
    matrix* MDlg = new matrix(parent, initial_pos);
    /*if (MDlg.exec() == QDialog::Accepted) {
        return "herp"; //MDlg.getMatrixString();
    }
    else return QString();*/

    MDlg->show();

    return "";
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
            int ret = QMessageBox::critical(this, "Fehler", "Das eingebene lineare Gleichungssystem ist nicht eindeutig lösbar!", QMessageBox::Ok, QMessageBox::Close);
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
