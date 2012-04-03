#include "roomtextedit.h"

roomTextEdit::roomTextEdit()
{
    new SyntaxHighlighter(this->document());
}
