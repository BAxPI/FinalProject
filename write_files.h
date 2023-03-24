#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.h"
#include "tables.h"

/*
    This function writes the object file.
    @param filename: The name of the file to write to
    @param code_image The code image to write
    @param ic The instruction counter
    @param data_image The data image to write
    @param dc The data counter
*/
void write_obj_file(char *filename, machine_w code_image[], long ic, data_w data_image[], long dc);

/*
    This function writes the entry file.
    @param filename The name of the file to write to
    @param symbol_table The symbol table
*/
void write_ent_file(char *filename, s_table *symbol_table);

/*
    This function writes the external file.
    @param filename The name of the file to write to
    @param symbol_table The symbol table
    @param code_image The code image
    @param ic The instruction counter
*/
void write_ext_file(char *filename, s_table *symbol_table, machine_w code_image[], long ic);