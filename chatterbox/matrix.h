#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>
#include <QtGui>
#include <QMessageBox>
#include <QPropertyAnimation>
#include "onelinetextedit.h"

class matrix : public QDialog
{
    Q_OBJECT
public:
    matrix(QWidget *parent = 0, QPoint initial_pos =0);
    //QString getMatrixString() const;
    static QString MatrixDlg(QWidget *parent, QPoint initial_pos);

    //bool eventFilter(QObject* object, QEvent* event);


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *e);

private:
    QVBoxLayout *mainLayout;
    QTableView *tblv;
    QStandardItemModel *model;
    QPoint m_dragPosition;
    QStringList m_lines;
    //OneLineTextEdit *texedit;
    QPoint current_pos;
    QTextEdit* textedit;



    
signals:
    
public slots:
    void expandModel();
    void moveAnimation();
    //void calc();
    
};

#endif // MATRIX_H
