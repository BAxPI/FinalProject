#include <stdlib.h> 
#include <stdio.h> 
#include "preprocessor.h"
#include "first_pass.h"
#include "second_pass.h"
#include "write_files.h"
#include "tables.h"
#include "globals.h"
#include "utils.h"
#include "lexer.h"

/*
    This function recieves a file name and process it to machine code.
    @param filename The file to be processed.
    @return TRUE if the file process succeeded and FALSE otherwise.
*/
bool process_file(char *filename); 

/* 
    Final Project of the course System Programming Lab in the Open University of Israel. 
    The goal of the project is to build an assembler for a 16-instruction assembly language,
    for a 14-bit CPU with memory size of 256 words. The author of the project is @Bar Amber.
*/
int main(int argc, char *argv[])
{
    int i;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 file3 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > MAX_FILE_NAME) {
            fprintf(stderr, "Filename %s is too long (max %d characters)\n",
                    argv[i], MAX_FILE_NAME);
            continue;
        }
        process_file(argv[i]);
    }

    return 0;
}


bool process_file(char *filename){
    long ic = 0, dc = 0;
    data_w data_image[MEMORY_SIZE] = {0}; /* Contains the data image of the file */
    machine_w code_image[MEMORY_SIZE] = {0}; /* Contains the code image of the file */
    char *am_filename = NULL;
    char tmp_line[MAX_LINE_LENGTH + 2];
    line_info current_line;
    s_table *symbol_table = NULL;
    FILE *file_desc = NULL;
    bool success = TRUE, extern_sym = FALSE, entry_sym = FALSE;
    char temp_c;
    struct lex_tree line_structure;



    /* Spread the macros from the origin file */
    if(!preprocess_file(filename)){
        printf("Couldn't spread the macros correctly into %s.as\n", filename);
        return FALSE;
    }

    /* Open the file created by the preprocessor */
    am_filename = str_allocate_cat(filename, ".am");
    file_desc = fopen(am_filename, "r");
    if(file_desc == NULL){
        printf("Couldn't open the file %s.\n", am_filename);
        free(am_filename);
        return FALSE;
    }

    /* Start first pass */
    symbol_table = create_s_table();
    current_line.filename = am_filename;
    current_line.content = tmp_line;
    for(current_line.line_number=1; fgets(tmp_line, MAX_LINE_LENGTH + 2, file_desc) != NULL; current_line.line_number++){
        /* Check if the line is too long */
        if (strchr(tmp_line, '\n') == NULL && !feof(file_desc)){
            print_error_msg(&current_line, "The line is too long, maximum size of an input line is 80 charachters.");
            success = FALSE;
            /* Skip garbage charachters */
            do {
				temp_c = fgetc(file_desc);
			} while (temp_c != '\n' && temp_c != EOF);
        }
        else {
                line_structure = lexer_get_lex_tree_from_line(tmp_line);
                if(line_structure.lex_union_option == lex_union_lex_error){
                    print_error_msg(&current_line, line_structure.lex_error);
                    success = FALSE;
                }
                else{
                    if(line_structure.lex_union_option == lex_union_asm_dir){
                        if(line_structure.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry) entry_sym = TRUE;
                        else if(line_structure.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_extern) extern_sym = TRUE;
                    }
                    success = first_pass_line_process(current_line, symbol_table, &ic, &dc, data_image, code_image, &line_structure);
                }

            }
        
    }
    update_data_symbols_address(symbol_table, ic);
    
    /* Second pass */
    rewind(file_desc);
    if(success){
        for(current_line.line_number=1; fgets(tmp_line, MAX_LINE_LENGTH + 2, file_desc) != NULL; current_line.line_number++){
            line_structure = lexer_get_lex_tree_from_line(tmp_line); /* Syntax error detected in the first pass */
            success = spass_line_process(line_structure, symbol_table, current_line); 
        }
        spass_label_encoding(symbol_table, code_image, ic);
    }

    if(ic + dc > MEMORY_SIZE - 100){ /* First 100 memory cells reserved for the system. */
            print_error_msg(&current_line, "The memory size is too small for the file.");
            success = FALSE;
    }
    
    /* Print files if success */
    if(success){
        write_obj_file(filename, code_image, ic, data_image, dc);
        if(entry_sym) write_ent_file(filename, symbol_table);
        if(extern_sym) write_ext_file(filename, symbol_table, code_image, ic);
    }
    
 
    /* Free all the allocated memory */
    free(am_filename);
    fclose(file_desc);
    free_s_table(symbol_table);
    free_code_image(code_image, ic);
    return TRUE;
}
