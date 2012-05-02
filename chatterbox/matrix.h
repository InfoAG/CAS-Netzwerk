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
    //QString getMatrixString() const;
    static QString MatrixDlg();

    //bool eventFilter(QObject* object, QEvent* event);
    QSize sizeHint() const;


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);

private:
    QTableView *tblv;
    QStandardItemModel *model;

    QPoint m_dragPosition;
    QStringList m_lines;

    
signals:
    
public slots:
    void expandModel();
    //void calc();
    
};

#endif // MATRIX_H
