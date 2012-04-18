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
    new SyntaxHighlighter(this->document());
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
    if ((e->key() == Qt::Key_Enter) || (e->key() == Qt::Key_Return))
    {
        QPushButton *button = this->derp->findChild<QPushButton *>("sayButton");
        button->animateClick();
    } else if (e->key() == Qt::Key_BracketLeft) {
        QTextEdit::keyPressEvent(e);
        QString matrixstr = matrix::MatrixDlg();
        if (matrixstr.isEmpty()) this->textCursor().deletePreviousChar();
        else {
            this->insertPlainText(matrixstr.left(matrixstr.indexOf(":")) + "]");
            expandedText += matrixstr;
        }
    } else if (e->key() == Qt::Key_Backspace) {
        expandedText.remove(expandedText.length() - 1, 1);
        QTextEdit::keyPressEvent(e);
    } else
        expandedText += e->text();
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
