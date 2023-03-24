#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"
#include "tables.h"


void write_obj_file(char *filename, machine_w code_image[], long ic, data_w data_image[], long dc);

void write_ent_file(char *filename, s_table *symbol_table);

void write_ext_file(char *filename, s_table *symbol_table, machine_w code_image[], long ic);