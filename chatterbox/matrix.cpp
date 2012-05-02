#include "matrix.h"

matrix::matrix(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint)
{
    QPushButton* accepted_button = new QPushButton;
    accepted_button->setText(tr("Ok"));
    accepted_button->setDefault(true);
    QPushButton* quit_button = new QPushButton;
    quit_button->setText(tr("Quit"));
    QHBoxLayout* button_layout = new QHBoxLayout;
    button_layout->addWidget(accepted_button);
    button_layout->addWidget(quit_button);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    tblv = new QTableView();
    installEventFilter(this);

    mainLayout->addWidget(tblv);
    mainLayout->addLayout(button_layout);
    setLayout(mainLayout);

    model = new QStandardItemModel(2, 2, this);
    //model->setHorizontalHeaderItem(0, new QStandardItem(QString("1")));
    //model->setHorizontalHeaderItem(1,new QStandardItem(QString("2")));
    QStandardItem* item = new QStandardItem(QString("[Arrow Right]"));
    item->setSelectable(false);
    model->setItem(0, 0,new QStandardItem());
    model->setItem(0, 1, item);
    model->setItem(1, 0, new QStandardItem(QString("[Arrow Down]")));

    tblv->setModel(model);

    //setAttribute( Qt::WA_TranslucentBackground, true);
    connect(quit_button,SIGNAL(clicked()),this,SLOT(reject()));

    //setStyleSheet("matrix {background-color: black; border-radius: 5px; border-color: black; border-width: 2px; border-style: outset; padding-top: 2px; padding-bottom: 5px; padding-left: 5px; padding-right: 5px}");

    //setStyleSheet("matrix {border-width: 5px; border-style: inset; border-color: grey; border-radius: 9px; background: black; color: blue; selection-background-color: ltblue;}");

    //setStyleSheet("matrix {background-color: black; border-style: outset; border-width: 10px; border-radius: 20px; border-color: white; font: bold 14px; min-width: 10em; padding: 6px}");


    //this->setMask(roundedRect(tblv->rect(), 10));


    QPixmap pixmap(this->sizeHint());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::white);
    painter.setBrush(Qt::black);
    painter.drawRoundRect(pixmap.rect());
    setMask(pixmap.createMaskFromColor(Qt::white));
}

QSize matrix::sizeHint() const
{
    return QSize(300, 200);
}

void matrix::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void matrix::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
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

    QPainter p(this);
    p.fillRect(0, 0, width(), height(), gradient);

    QFont headerFont("Sans Serif", 12, QFont::Bold);
    QFont normalFont("Sans Serif", 9, QFont::Normal);

    // draw it twice for shadow effect
    p.setFont(headerFont);
    QRect headerRect(1, 1, width(), 25);
    p.setPen(Qt::black);
    //p.drawText(headerRect, Qt::AlignCenter, m_station.name());

    headerRect.moveTopLeft(QPoint(0, 0));
    p.setPen(Qt::white);
    //p.drawText(headerRect, Qt::AlignCenter, m_station.name());

    p.setFont(normalFont);
}

void matrix::resizeEvent(QResizeEvent*)
{
    QBitmap maskBitmap(width(), height());
    maskBitmap.clear();

    QPainter p(&maskBitmap);
    p.setBrush(Qt::black);
    p.drawRoundRect(0, 0, width(), height(), 20, 30);
    p.end();

    setMask(maskBitmap);
}




void matrix::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Right)
    {
        qDebug() << "derp";

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

QString matrix::MatrixDlg() {
    matrix MDlg;
    if (MDlg.exec() == QDialog::Accepted) {
        return "herp"; //MDlg.getMatrixString();
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
