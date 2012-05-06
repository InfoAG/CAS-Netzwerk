#include "onelinetextedit.h"

OneLineTextEdit::OneLineTextEdit(QWidget* parent) : QTextEdit(parent),derp(parent)
{
    setWordWrapMode (QTextOption::NoWrap);
    setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setTabChangesFocus (true);
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    new SyntaxHighlighter(this->document());
    connect(this, SIGNAL(textChanged()), this, SLOT(textChanged()));
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
    } else if ((e->key() == Qt::Key_Up) || (e->key() == Qt::Key_Down)) {
        emit cHistRequested(e);
    } else if (e->text() == "[") {
        QTextEdit::keyPressEvent(e);
        QString matrixstr = matrix::MatrixDlg(this,this->mapToGlobal(this->cursorRect().bottomLeft()));
        //if (matrixstr.isEmpty()) this->textCursor().deletePreviousChar();
        //else this->insertPlainText(matrixstr + "]");
    } else
        QTextEdit::keyPressEvent(e);
}

void OneLineTextEdit::textChanged()
{
    if (this->toPlainText().endsWith("integrate("))
    {
        QString simpsonstr = SimpsonGui::SimpsonDlg();
        if (simpsonstr.isEmpty()) this->setText(this->toPlainText().left(this->toPlainText().length() - 10));
        else this->insertPlainText(simpsonstr + ")");
    }
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
