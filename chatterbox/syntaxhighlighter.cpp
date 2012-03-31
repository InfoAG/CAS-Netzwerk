#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* document):QSyntaxHighlighter(document)
{

}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    for(int i = 0; i < text.length(); i++)
    {
        if(text.mid(i,1) == "[")
        {
            setFormat(i,1, Qt::darkCyan);
        }
    }

}
