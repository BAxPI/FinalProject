#include "tables.h"
#include "globals.h"
#include "lexer.h"


bool spass_line_process(struct lex_tree lt, s_table *table, line_info line);

void spass_label_encoding(s_table *symbol_table, machine_w *code_image, long ic);