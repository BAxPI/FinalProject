
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "globals.h"
#include "tables.h"
#include "utils.h"

/*
    This function acts as the preprocessor of the assembler.
    It reads the file and removes all comments and empty lines, and replaces all macros with their content.
    @param filename The name of the file to preprocess
    @return TRUE if the file was preprocessed successfully and FALSE otherwise.
*/
bool preprocess_file(const char *filename);