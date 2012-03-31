#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document):QSyntaxHighlighter(document)
{
    format.setFontWeight(QFont::Bold);
    format.setBackground(Qt::yellow);
    QFont hlFont = format.font();
    hlFont.setPointSize(8.5);
    format.setFont(hlFont);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for(int i = 0; i < text.length(); i++)
    {
        if(text.mid(i,1) == "[")
        {
            setFormat(i,1, format);
            /*QTextCursor imageCursor(currentBlock());
            imageCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, i);
            QImage bracketImage(":/cas.png");
            QPropertyAnimation animation(&bracketImage, "scale");
            animation.setDuration(10000);
            animation.setStartValue(1);
            animation.setStartValue(10);
            imageCursor.insertImage(bracketImage);
            animation.start();*/
        }
    }
}
