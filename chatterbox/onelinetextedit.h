#ifndef ONELINETEXTEDIT_H
#define ONELINETEXTEDIT_H

#include <QKeyEvent>
#include <QApplication>
#include <QPushButton>
#include <QObject>
#include <QTextEdit>
#include "syntaxhighlighter.h"
#include "simpsongui.h"

class OneLineTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit OneLineTextEdit(QWidget *parent = 0);

    QSize sizeHint() const;

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    QWidget* derp; //?!?! :D

private slots:
    void textChanged();

signals:
    void cHistRequested(QKeyEvent*);

};

#endif // ONELINETEXTEDIT_H
