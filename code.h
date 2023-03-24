#include <stdlib.h>
#include "lexer.h"
#include "tables.h"
#include "globals.h"


/*
    this function encodes an instruction line into the code image.
    @param line - the line to encode
    @param lt - the lex tree of the line (Abstract Syntax Tree)
    @param code_image - the code image to encode the line into
    @param ic - the instruction counter
    @param symbol_table - the symbol table.
    @return - TRUE if the line was encoded successfully, FALSE otherwise
*/
bool encode_instruction(line_info *line, struct lex_tree *lt, machine_w *code_image, long *ic, s_table *symbol_table);



