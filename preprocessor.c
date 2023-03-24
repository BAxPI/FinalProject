#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "globals.h"
#include "tables.h"
#include "utils.h"

#define MAX_MACRO_NAME_SIZE 30

bool preprocess_file(char *filename){
    bool reading_macro = FALSE;
    int number_of_words_in_line = 0, index=0;
    char current_line[MAX_LINE_LENGTH + 2];
    char **splitted_line;
    char *macro_name = NULL; 
    char *am_filename = str_allocate_cat(filename, ".am");
    char *origin_filename = str_allocate_cat(filename, ".as");
    FILE *am_file_des;
    FILE *origin_file_des = fopen(origin_filename, "r");
    m_table *macros_table = create_m_table();


    /* Verify we were able to open the original file */
    if(origin_file_des == NULL){
        printf("Couldn't open the file %s\n", origin_filename);
        free(am_filename);
        free(origin_filename);
        free_m_table(macros_table);
        return FALSE;
    }

    /* Verify we were able to create a new am file */
    am_file_des = fopen(am_filename, "w+");
    if(am_file_des == NULL){
        printf("Couldn't open a file named %s.am\n", am_filename);
        free(am_filename);
        free(origin_filename);
        fclose(origin_file_des);
        fclose(am_file_des);
        free_m_table(macros_table);
        return FALSE;
    }

    while(fgets(current_line, MAX_LINE_LENGTH+2,origin_file_des)){
        index=0; /* Index to check where white space ends*/
        SKIP_WHITE(current_line, index)
        /* If the line is empty, EOF or comment then skip it */
        if (current_line[index] == '\n' || current_line[index] == EOF || current_line[index] == ';' || current_line[index] == '\0')
            continue;
        
        splitted_line = split_string(current_line + index, " ", &number_of_words_in_line); /* Split the line from where whitespace ends */
        
        /* If defined macro, print it */
        if(get_from_m_table(macros_table, splitted_line[0]) != NULL){
            fputs(get_from_m_table(macros_table, splitted_line[0]), am_file_des);
        }
        /* If there's a macro definition */
        else if (strcmp(splitted_line[0], "mcr") == 0){
            reading_macro = TRUE;
            macro_name = (char *)malloc(sizeof(char) * MAX_MACRO_NAME_SIZE);
            strcpy(macro_name, splitted_line[1]);
            add_m_entry(macros_table, macro_name, "");
        }
        /* If there's an end of macro definition */
        else if (strcmp(splitted_line[0], "endmcr") == 0){
            reading_macro = FALSE;
        }
        /* If still in a macro definition */
        else if(reading_macro == TRUE){
            char *updated_macro = str_allocate_cat(get_from_m_table(macros_table, macro_name), current_line);
            add_m_entry(macros_table, macro_name, updated_macro);
            free(updated_macro);
        }
        /* Regular line */
        else{
            fputs(current_line, am_file_des);
        }
        
        /* Free the memory of splitted line before iterating to the next line */
        free_splitted_string(splitted_line, number_of_words_in_line);
        
    }

    /* Free all the memory that we've used in the file */
    free(am_filename);
    free(origin_filename);
    fclose(origin_file_des);
    fclose(am_file_des);
    free_m_table(macros_table);
    if(macro_name != NULL){
        free(macro_name); 
    }
    

    return TRUE;
    
}





