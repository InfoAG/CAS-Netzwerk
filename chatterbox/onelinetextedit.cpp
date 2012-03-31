#include "onelinetextedit.h"
#include "syntaxhighlighter.h"
#include <QKeyEvent>
#include <QApplication>
#include <QPushButton>
#include <QObject>

OneLineTextEdit::OneLineTextEdit(QWidget* parent) : QTextEdit(parent),derp(parent)
{
    setWordWrapMode (QTextOption::NoWrap);
    setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setTabChangesFocus (true);
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    SyntaxHighlighter* syntax = new SyntaxHighlighter(this->document());
}

QSize OneLineTextEdit::sizeHint () const
{
    QFontMetrics fm(font());
    int h = fm.height();
    int w = fm.width(QLatin1Char('x'));

    return QSize(w, h);
}

void OneLineTextEdit::keyPressEvent (QKeyEvent *e)
{
    if ((e->key () == Qt::Key_Enter) || (e->key () == Qt::Key_Return))
    {
        QPushButton *button = this->derp->findChild<QPushButton *>("sayButton");
        button->animateClick();
    }

    else
        QTextEdit::keyPressEvent (e);
}



//void OneLineTextEdit::SyntaxLight()
//{
//    QApplication::quit();
//    if (this->toPlainText().right(1) == "[")
//    {
//        QApplication::quit();
//        QTextCursor highlightCursor = this->document()->find("[");
//        if (highlightCursor.isNull() == false)
//        {
//         QTextCharFormat highlightFormat;
//         highlightFormat.setUnderlineColor(Qt::darkGreen);
//         highlightFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
//         highlightCursor.mergeCharFormat(highlightFormat);
//        }

//    }

//}
