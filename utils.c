#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "globals.h"
#include "tables.h"



char *str_allocate_cat(char *first_str, char* second_str) {
	char *str = (char *)malloc(strlen(first_str) + strlen(second_str) + 1);
	strcpy(str, first_str);
	strcat(str, second_str);
	return str;
}


char **split_string(char *string, char *separators, int *count) {
    int len = strlen(string);
    char buffer[MAX_LINE_LENGTH + 2];
    int j, i=0, k = 0;
    int old_i;
    char *copy;
    char **strings;
    int string_index = 0;
    int to_allocate;
    *count = 0;
    

    copy = (char *) malloc(sizeof(char) * (len + 1));
    if (copy == NULL) {
        /* Failed to allocate memory for the copy */
        return NULL;
    }
    strncpy(copy, string, len+1);

    /* Ignore the newline character in the last word */
    if (len > 0 && copy[len - 1] == '\n') {
        copy[len - 1] = '\0';
        len--;
    }

    while (i < len) {
        while (i < len) {
            if (strchr(separators, copy[i]) == NULL)
                break;
            i++;
        }
        old_i = i;
        while (i < len) {
            if (strchr(separators, copy[i]) != NULL)
                break;
            i++;
        }
        if (i > old_i)
            *count = *count + 1;
    }

    /* Allocate an array to hold the resulting strings */
    strings = malloc(sizeof(char *) * *count);
    if (strings == NULL) {
        /* Failed to allocate memory for the array */
        free(copy);
        return NULL;
    }

    /* Split the input string into words */
    i = 0;
    while (i < len) {
        while (i < len) {
            if (strchr(separators, copy[i]) == NULL)
                break;
            i++;
        }
        j = 0;
        while (i < len) {
            if (strchr(separators, copy[i]) != NULL)
                break;
            buffer[j] = copy[i];
            i++;
            j++;
        }
        if (j > 0) {
            buffer[j] = '\0';
            to_allocate = sizeof(char) * (strlen(buffer) + 1);
            strings[string_index] = malloc(to_allocate);
            if (strings[string_index] == NULL) {
                /* Failed to allocate memory for a string */
                free(copy);
                for (k=0; k < string_index; k++) {
                    free(strings[k]);
                }
                free(strings);
                return NULL;
            }
            strcpy(strings[string_index], buffer);
            string_index++;
        }
    }

    /* Free the copy of the input string */
    free(copy);

    /* Return the array of strings */
    return strings;
}

void free_splitted_string(char **splitted_string, int count){
    int i;
    for(i=0; i < count; i++){
        free(splitted_string[i]);
    }
    free(splitted_string);
}

void print_error_msg(line_info *line, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("\033[0;31m"); /* Red */
    printf("ERROR: ");
    printf("\033[0m"); /* Reset */
    vprintf(format, args);
    printf(" in the file: ");
    printf("\033[0;35m"); /* Purple */
    printf("%s", line->filename);
    printf("\033[0m"); /* Reset */
    printf(" in line: ");
    printf("\033[0;35m"); /* Purple */
    printf("%d\n", line->line_number);
    printf("\033[0m"); /* Reset */
    va_end(args);
}


void print_dw(data_w word){
    int i;
    for (i = 13; i >= 0; i--) {
        putchar((word.data >> i) & 1 ? '1' : '0');
    }
    putchar('\n');
}





void free_code_image(machine_w code_image[], long ic) {
    int i;
    for (i = 0; i < ic; i++) {
        if (code_image[i].label != NULL){
            free(code_image[i].label);
            code_image[i].label = NULL;
        }
        if(code_image[i].word.f_word != NULL) {
            free(code_image[i].word.f_word);
            code_image[i].word.f_word = NULL;
        } 
        else if (code_image[i].word.id_word != NULL) {
            free(code_image[i].word.id_word);
            code_image[i].word.id_word = NULL;
        } 
         if (code_image[i].word.r_word != NULL) {
            free(code_image[i].word.r_word);
            code_image[i].word.r_word = NULL;
        }
    }
}

void print_mw(machine_w mw) {
    int i =0;
    unsigned int value;
    if(mw.word.f_word != NULL){
        value = *(unsigned int*) mw.word.f_word;
    }
    if (mw.label != NULL){
        printf("%s", mw.label);
    }
    else {
        for (i = 13; i >= 0; i--) {
            printf("%d", (value >> i) & 1);
        }
    }
    printf("\n");
}


void update_data_symbols_address(s_table *table, long ic) {
    int i;
    for (i = 0; i < TABLE_SIZE; ++i) {
        s_table_entry *entry = table->entries[i];
        while (entry != NULL) {
            if (entry->symbol_type == DATA_SYMBOL) {
                entry->symbol_addr += ic;
            }
            entry = entry->next;
        }
    }
}


bool is_all_digits(const char *operand) {
    while (*operand != '\0') {
        if (!isdigit(*operand)) {
            return FALSE;  /* operand contains a non-digit character */
        }
        operand++;
    }
    return TRUE;  /* operand contains only digits */
}


bool is_valid_label(char *word){
    char label[MAX_LABEL_SIZE];
    int i;
    const char* reserved_words[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                                    "and", "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                    "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    const int num_reserved_words = 25;
    
    if (strlen(word) > MAX_LABEL_SIZE){
        return FALSE;
    }
    /* Get rid of the ':' */
    if(word[strlen(word) -1] == ':'){
        for(i=0;i<strlen(word)-1;i++){
                label[i] = word[i];
            }
        label[i] = '\0';
    }
    else{ /*Label coming from analyzing operands */
        for(i=0;i<strlen(word);i++){
                label[i] = word[i];
            }
        label[i] = '\0';
    }
    
    /* Check if starting with alpha character */
    if(!isalpha(label[0])){
        return FALSE;
    }
    /* Check that the label isn't a reserved word */
    for(i=0; i<num_reserved_words; i++){
        if(strcmp(label, reserved_words[i]) == 0){
            return FALSE;
        }
    }
    /* Check the all characters are alphanumeric in the label */
    for(i=0;label[i] != '\0'; i++){
        if(!isalnum(label[i])){
            return FALSE;
        }
    }
    return TRUE;
}