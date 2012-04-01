#ifndef ONELINETEXTEDIT_H
#define ONELINETEXTEDIT_H

#include <QTextEdit>

class OneLineTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit OneLineTextEdit(QWidget *parent = 0);

    QSize sizeHint() const;

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    QWidget* derp;

//public slots:
//    void SyntaxLight();

//signals:4ss4
//    void textChanged();

};

#endif // ONELINETEXTEDIT_H
