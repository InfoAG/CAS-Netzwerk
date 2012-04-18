#ifndef ONELINETEXTEDIT_H
#define ONELINETEXTEDIT_H

#include <QTextEdit>

class OneLineTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit OneLineTextEdit(QWidget *parent = 0);

    QSize sizeHint() const;

    QString getExpandedText() const { return expandedText; };

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    QWidget* derp; //?!?! :D
    QString expandedText;

//public slots:
//    void SyntaxLight();

//signals:4ss4
//    void textChanged();

};

#endif // ONELINETEXTEDIT_H
