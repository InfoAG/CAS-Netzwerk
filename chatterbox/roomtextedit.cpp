#include "roomtextedit.h"

roomTextEdit::roomTextEdit()
{
    SyntaxHighlighter* syntax = new SyntaxHighlighter(this->document());
}
