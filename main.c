#include <stdio.h>
#include "lexer.h"




int main() {
    struct lex_tree  lt = lexer_get_lex_tree_from_line("MAIN:  mov   r4,LENGTH");
}