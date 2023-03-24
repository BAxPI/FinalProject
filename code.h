#include <stdlib.h>
#include "lexer.h"
#include "tables.h"
#include "globals.h"


/*
    this function encodes an instruction line into the code image.
    @param line The line to encode
    @param lt The lex tree of the line (Abstract Syntax Tree)
    @param code_image The code image to encode the line into
    @param ic The instruction counter
    @param symbol_table The symbol table.
    @return TRUE if the line was encoded successfully, FALSE otherwise
*/
bool encode_instruction(line_info *line, struct lex_tree *lt, machine_w *code_image, long *ic, s_table *symbol_table);



