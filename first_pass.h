#include "globals.h"
#include "tables.h"
#include "lexer.h"



bool first_pass_line_process(line_info line, s_table *symbol_table, long *ic, long *dc,
                             data_w *data_image, machine_w *code_image, struct lex_tree *lt);