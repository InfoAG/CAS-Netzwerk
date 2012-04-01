#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document):QSyntaxHighlighter(document)
{
    format.setFontWeight(QFont::Bold);
    format.setBackground(Qt::cyan);
    QFont hlFont = format.font();
    format.setFont(hlFont);
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    enum {NormalState = -1, InsideBracket};

    int state = previousBlockState();
    int start = 0;

    for (int i = 0; i < text.length(); ++i)
    {
        if (state == InsideBracket)
        {
            if (text.mid(i, 1) == "]")
            {
                state = NormalState;
                setFormat(start, i - start + 1, format);
            }
        } else
        {
            if (text.mid(i, 1) == "[")
            {
                        start = i;
                        state = InsideBracket;
            }
        }
    }

    if (state == InsideBracket)
        setFormat(start, text.length() - start, format);

    setCurrentBlockState(state);
}
