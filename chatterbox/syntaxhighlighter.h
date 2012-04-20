#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCursor>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include "onelinetextedit.h"
#include "matrix.h"

class SyntaxHighlighter: public QSyntaxHighlighter
{

  public:
    SyntaxHighlighter(QTextDocument* document);

    void highlightBlock(const QString &text);
    QTextCharFormat format;
};

#endif // SYNTAXHIGHLIGHTER_H
