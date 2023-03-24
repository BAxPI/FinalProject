#include "globals.h"
#include "tables.h"
#include "lexer.h"


/*
    Processes a line in the first pass of the assembler.
    @param line The line to process
    @param symbol_table The symbol table
    @param ic The instruction counter
    @param dc The data counter
    @param data_image The data image
    @param code_image The code image
    @param lt The lex tree of the line (Abstract Syntax Tree)
    @return TRUE if the line was processed successfully, FALSE otherwise
*/
bool first_pass_line_process(line_info line, s_table *symbol_table, long *ic, long *dc,
                             data_w *data_image, machine_w *code_image, struct lex_tree *lt);