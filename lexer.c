#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "lexer.h"
#include <ctype.h>

#define MAX_INT_VALUE 8191
#define MIN_INT_VALUE -8192

/*
    This function gets a line and stores the directive in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the directive in
*/
static void get_directive(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the string in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the string in
*/
static void get_string(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the data in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the data in
*/
static void get_data(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the entry/extern label in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the entry/extern label in
*/
static void get_ent_extern_label(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the instruction in
*/
static void get_code_instruction(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the operands of a set A instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the operands in
*/
static void get_set_a_operands(const char *line, int *index, struct lex_tree *lt);

/*
    This function gets a line and stores the operands of a set B instruction in the lex tree.
    @param line The line to parse
    @param index The index of the line to start parsing from
    @param lt The lex tree to store the operands in
*/
static void get_set_b_operands(const char *line, int *index, struct lex_tree *lt);

/* 
    This line gets an operand and stores its type in the lex tree, and returns its addressing type
    @param operand The operand to analyze
    @param lt The lex tree to store the operand type in
    @return The addressing type of the operand    
*/
static enum operand_addr_type analyze_operand(char *operand, struct  lex_tree *lt);


struct lex_tree lexer_get_lex_tree_from_line(const char * line) {
    struct lex_tree lt = {0}; /* The lex tree we will return */
    char *ptr; 
    int i = 0, index = 0; 
    char *token = NULL;
    
    SKIP_WHITE(line, i);
    ptr = (char *)(line +i);
    /* Check if the line is empty */
    if(line[i] == '\0' || line[i] == '\n' || line[i] == EOF) {
        lt.lex_union_option = lex_union_asm_empty_line;
        return lt;
    }
    /* Check if the line is comment */
    else if(line[i] == ';') {
        lt.lex_union_option = lex_union_asm_comment;
        return lt;
    }
    /* Check if label definition */
    SKIP_WHITE(line, i)
    if((ptr = strchr(line, ':')) != NULL){
        index = ptr - (line + i);
        token = malloc(sizeof(char) * index + 1);
        strncpy(token, line + i, index);
        token[index] = '\0';
        if(is_valid_label(token)){
            strcpy(lt.label, token);
            free(token);
            index += (i + 1); /* Skip the ':' */
        }
        else{
            lt.lex_union_option = lex_union_lex_error;
            strcpy(lt.lex_error, "Invalid label");
            free(token);
            return lt;
        }
    }
    else{
        lt.label[0] = '\0'; /* No label */
    }
    
    /* Check if the line is a directive (.string .data .entry .extern) */
    SKIP_WHITE(line, index) 
    if(line[index] == '.'){
        get_directive(line, &index, &lt);
        SKIP_WHITE(line, index) 
        if(lt.lex_union_option == lex_union_lex_error){
            return lt;
        }
        else if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_string){
            get_string(line, &index, &lt);
            return lt;
        }
        else if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_data){
            get_data(line, &index, &lt);
        }
        else if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry || 
                lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_extern){
            get_ent_extern_label(line, &index, &lt);
            return lt;
        }
    }
    else {
        get_code_instruction(line, &index, &lt);
        if(lt.lex_union_option == lex_union_lex_error){
            return lt;
        }
        SKIP_WHITE(line, index)
        /* First set commands (2 Parameters) - mov, cmp, add, sub, lea */
        if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_mov || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_cmp ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_add || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_sub ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_lea){
                get_set_a_operands(line, &index, &lt);
                return lt;
        }

        /* Second set commands (1 Parameter) - not, clr, inc, dec, jmp, bne, red, prn, jsr */
        if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_not || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_clr ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_inc || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_dec ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jmp || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_bne ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_red || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_prn ||
            lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jsr){
                get_set_b_operands(line, &index, &lt);
                return lt;
            }
        /* Third set commands (No parameters) - stop, rts */
        if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_stop || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_rts){
            SKIP_WHITE(line, index)
            if(line[index] != '\0' && line[index] != '\n' && line[index] != EOF){
                lt.lex_union_option = lex_union_lex_error;
                strcpy(lt.lex_error, "Detected trailing characters, Group C instructions do not have operands.");
                return lt;
            }
            return lt;
        }
    }

    return lt;
}



static void get_directive(const char *line, int *index, struct lex_tree *lt){
    (*index)++; /* Skip the '.' */
    if(strncmp(line+ (*index) , "string", 6) == 0){
        lt->lex_union_option = lex_union_asm_dir;
        lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option = lex_union_asm_dir_string;
        (*index) += 6; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "data", 4) == 0){
        lt->lex_union_option = lex_union_asm_dir;
        lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option = lex_union_asm_dir_data;
        (*index) += 4; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "entry", 5) == 0){
        lt->lex_union_option = lex_union_asm_dir;
        lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option = lex_union_asm_dir_entry;
        (*index) += 5; /* Skip the directive */
    }
    else if(strncmp(line+ (*index) , "extern", 6) == 0){
        lt->lex_union_option = lex_union_asm_dir;
        lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option = lex_union_asm_dir_extern;
        (*index) += 6; /* Skip the directive */
    }
    else{
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Invalid instruction.");
        return;
    }
}


static void get_string(const char *line, int *index, struct lex_tree *lt){
    char *end_quote;
    SKIP_WHITE(line, *index)
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, ".string instruction must have a string.");
        return;
    }
    if(line[*index] != '"'){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "String must start with quotes(\").");
        return;
    }
    (*index)++; /* Skip the '"' */
    
    /* Check for closing quotes */
    end_quote = strchr(line + (*index), '"');
    if(end_quote == NULL){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "String must end with quotes(\").");
        return;
    }
    else{
        int len = end_quote - (line + (*index));
        strncpy(lt->asm_inst_asm_dir.asm_dir.dir.string, line + (*index), len);
        lt->asm_inst_asm_dir.asm_dir.dir.string[len] = '\0';
        (*index) += len + 1; /* Skip the '"' */
    }
    SKIP_WHITE(line, (*index))
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Detected trailing charachters after the end of the string.");
        return;
    } 
}

static void get_data(const char *line, int *index, struct lex_tree *lt){
    char *endPtr;
    int value;
    int num_of_ints = 0;

    do{
        SKIP_WHITE(line, (*index))
        if(line[*index] == ','){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Leading comma detected, must start with an integer.");
            return;
        }
        else if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, ".data instruction must have at least one integer.");
        return;
        }
        value = strtol(line + (*index), &endPtr, 10);
        if(endPtr == line + (*index)){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Invalid number.");
            return;
        }
        if(value > MAX_INT_VALUE || value < MIN_INT_VALUE){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Number out of range.");
            return;
        }
        lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_array[num_of_ints] = value;
        lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_count++;
        num_of_ints++;
        (*index) += endPtr - (line + (*index));
        SKIP_WHITE(line, (*index))
        if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
            return;
        }
        else{
            if(line[*index] != ','){
                lt->lex_union_option = lex_union_lex_error;
                strcpy(lt->lex_error, "Invalid character, expected ','.");
                return;
            }
            (*index)++; /* Skip the ',' */
            SKIP_WHITE(line, (*index))
            if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
                lt->lex_union_option = lex_union_lex_error;
                strcpy(lt->lex_error, "Trailing comma detected.");
                return;
            }
            else if(line[*index] == ','){
                lt->lex_union_option = lex_union_lex_error;
                strcpy(lt->lex_error, "Consecutive commas detected.");
                return;
            }
        }

    }
    while(*endPtr != '\n' && *endPtr != '\0' && *endPtr != EOF);

}

static void get_ent_extern_label(const char *line, int *index, struct lex_tree *lt){
    char *token = NULL;
    int len = 0;
    SKIP_WHITE(line, (*index))
    if(lt->label[0] != '\0'){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Entry/Extern instruction can't be in label definition.");
        return;
    }
    if(line[*index] == '\0' || line[*index] == '\n' || line[*index] == EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Entry/Extern must be followed by a label.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' && 
                line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    token = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(token, line + (*index), len);
    token[len] = '\0';
    (*index) += len;
    

    if(!is_valid_label(token)){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Invalid label.");
        free(token);
        return;
    }
    strcpy(lt->asm_inst_asm_dir.asm_dir.dir.label, token);
    free(token);
    SKIP_WHITE(line, (*index))
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Detected trailing charachters after the end of the label.");
        return;
    } 
}

static void get_code_instruction(const char *line, int *index, struct lex_tree *lt){
    int len;
    char *potential_command;

    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' && 
                line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    potential_command = malloc(sizeof(char) * len + 1);
    strncpy(potential_command, line + *index, len);
    potential_command[len] = '\0';
    (*index) += len;
    
    if(strcmp(potential_command, "mov") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_mov;
    }
    else if(strcmp(potential_command, "cmp") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_cmp;
    }
    else if(strcmp(potential_command, "add") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_add;
    }
    else if(strcmp(potential_command, "sub") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_sub;
    }
    else if(strcmp(potential_command, "not") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_not;
    }
    else if(strcmp(potential_command, "clr") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_clr;
    }
    else if(strcmp(potential_command, "lea") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_lea;
    }
    else if(strcmp(potential_command, "inc") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_inc;
    }
    else if(strcmp(potential_command, "dec") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_dec;
    }
    else if(strcmp(potential_command, "jmp") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_jmp;
    }
    else if(strcmp(potential_command, "bne") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_bne;
    }
    else if(strcmp(potential_command, "red") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_red;
    }
    else if(strcmp(potential_command, "prn") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_prn;
    }
    else if(strcmp(potential_command, "jsr") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_jsr;
    }
    else if(strcmp(potential_command, "rts") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_rts;
    }
    else if(strcmp(potential_command, "stop") == 0){
        lt->lex_union_option = lex_union_asm_inst;
        lt->asm_inst_asm_dir.asm_inst.inst_name = lex_inst_stop;
    }
    else{
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Command doesn't exist.");
    }
    free(potential_command);
    return;

}


static void get_set_a_operands(const char *line, int *index, struct lex_tree *lt){
    char *comma;
    int len;
    char *operand;
    if((comma = strchr(line + *index, ',')) == NULL){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Set A command must have two operands, separated by a comma.");
        return;
    }
    SKIP_WHITE(line, *index)
    if(line[*index] == ','){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Leading comma detected, must start with an operand.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' && 
                line[*index + len] != EOF && !(isspace(line[*index + len])) && line[*index + len] != ','; len++);
    /* Store the first operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] = analyze_operand(operand, lt); /* 0 indicates first operand. */
    /* Check that the first operand is valid */
    if(lt->lex_union_option == lex_union_lex_error){
        strcpy(lt->lex_error, "First operand is illegal.");
        free(operand);
        return;
    }
    else{
        if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_immed){
            lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].immed = atoi(operand + 1);
        }
        else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_label){
            strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].label, operand);
        }
        else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_register){
            lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].reg = operand[1];
        }
    }
    free(operand);
    (*index) += len;
    SKIP_WHITE(line, *index)
    (*index)++; /* Skip the comma */
    SKIP_WHITE(line, *index)
    if(line[*index] == ','){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Consecutive commnas detected.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' && 
                line[*index + len] != EOF && !(isspace(line[*index + len])) && line[*index + len] != ','; len++);
    /* Store the second operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] = analyze_operand(operand, lt); /* 1 indicates first operand. */
    /* Check that the second operand is valid */
    if(lt->lex_union_option == lex_union_lex_error){
        strcpy(lt->lex_error, "Second operand is illegal.");
        free(operand);
        return;
    }
    else{
        if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_immed){
            lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].immed = atoi(operand + 1);
        }
        else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_label){
            strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].label, operand);
        }
        else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_register){
            lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].reg = operand[1] ;
        }
    }
    free(operand);
    (*index) += len;
    /* Try to detect trailing charachter */
    SKIP_WHITE(line, *index)
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Detected trailing charachters after the second operand.");
        return;
    } 
}

static void get_set_b_operands(const char *line, int *index, struct lex_tree *lt){
    int len;
    char *operand;
    SKIP_WHITE(line, *index)
    if(line[*index] == ','){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Leading comma detected, must start with an operand.");
        return;
    }
    for(len = 0; line[*index + len] != '\0' && line[*index + len] != '\n' && 
                line[*index + len] != EOF && !(isspace(line[*index + len])); len++);
    /* Store the operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line + *index, len);
    operand[len] = '\0';
    (*index) += len;
    SKIP_WHITE(line, *index)
    if(line[*index] != '\0' && line[*index] != '\n' && line[*index] != EOF){
        lt->lex_union_option = lex_union_lex_error;
        strcpy(lt->lex_error, "Detected trailing charachters after the operand.\nNote that spaces aren't allowed in group B operand, and that only one operand is allowed.");
        free(operand);
        return;
    }
    if(strchr(operand, '(') == NULL){
        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat = analyze_operand(operand, lt);
        if(lt->lex_union_option == lex_union_lex_error){
        free(operand);
        return;
        }
        else{
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_immed){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.immed = atoi(operand + 1);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_label){
                strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label, operand);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_register){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.reg = operand[1];
        }
        free(operand);
        return;
        }
    }
    else{ /* Label with parameters */
        char *delim = strchr(operand, '(');
        char *token;
        int token_len = 0, i = 0;

        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat = lex_op_addr_type_label_with_params;
        /* Check for closing parenthesis */
        if(strchr(operand, ')') == NULL){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing closing parenthesis.");
            free(operand);
            return;
        }
        else{
            if(operand[strlen(operand) - 1] != ')'){
                lt->lex_union_option = lex_union_lex_error;
                strcpy(lt->lex_error, "Operand must end with closing parenthesis.");
                free(operand);
                return;
            }
        }
        /* Getting the label */
        token_len = delim - operand;
        token = malloc((sizeof(char)) * token_len + 1);
        strncpy(token, operand, token_len);
        if(token_len == 0){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing label name.");
            free(operand);
            free(token);
            return;
        }
        token[token_len] = '\0';
        if(!is_valid_label(token)){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Invalid label name.");
            free(operand);
            free(token);
            return;
        }
        else{
            strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.label, token);
            free(token);
        }
        i += token_len + 1; /* Skip the label and parenthesis */
        /* Getting the first parameter */
        delim = strchr(operand + i, ',');
        if(delim == NULL){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing parameter.");
            free(operand);
            return;
        }
        
        token_len = delim - (operand + i);
        token = malloc((sizeof(char)) * token_len + 1);
        strncpy(token, operand + i, token_len);
        if(token_len == 0){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing first parameter.");
            free(operand);
            free(token);
            return;
        }
        token[token_len] = '\0';
        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] = analyze_operand(token, lt);
        if(lt->lex_union_option == lex_union_lex_error){
            free(operand);
            free(token);
            return;
        }
        else{
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_immed){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].immed = atoi(token + 1);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_label){
                strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].label, token);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_register){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].reg = token[1];
            }
            free(token);
        }

        /* Getting the second parameter */
        i += token_len + 1; /* Skip the first parameter and comma */
        delim = strchr(operand + i, ')'); 
        token_len = delim - (operand + i);
        token = malloc((sizeof(char)) * token_len + 1);
        strncpy(token, operand + i, token_len);
        if(token_len == 0){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing second parameter.");
            free(operand);
            free(token);
            return;
        }
        token[token_len] = '\0'; 
        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] = analyze_operand(token, lt);
        if(lt->lex_union_option == lex_union_lex_error){
            free(operand);
            free(token);
            return;
        }
        else{
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_immed){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].immed = atoi(token + 1);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_label){
                strcpy(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].label, token);
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_register){
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].reg = token[1];
            }
            free(token);
        }
        
    }
    if(operand != NULL){
        free(operand);
    } 
}

static enum operand_addr_type analyze_operand(char *operand, struct  lex_tree *lt){
    char *endptr;
    long value;
    char *value_str;
    int i;
    char *hash_pos;
    if (strlen(operand) == 0) {
        lt->lex_union_option = lex_union_lex_error; /* Error: operand is NULL */
        strcpy(lt->lex_error, "Missing operand.");
        return lex_op_addr_type_error;
    }

    if (operand[0] == '#'  || is_all_digits(operand)) {
        if(is_all_digits(operand)){
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Immediate operand must start with '#'.");
            return lex_op_addr_type_error;
        }
        /* Immediate number */
        value_str = &operand[1];

        /* Check for multiple '#' signs before integer */
        hash_pos = strchr(value_str, '#');
        if (hash_pos != NULL) {
            /* Error: multiple '#' signs before integer */
            lt->lex_union_option = lex_union_lex_error; 
            strcpy(lt->lex_error, "Multiple '#' symbols before immediate operad.");
            return lex_op_addr_type_error;
        }

        if (*value_str == '\0') {
            /* Error: no integer after '#' */
            lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Missing integer after '#' sign.");
            return lex_op_addr_type_error;
        }

        /* Check that the rest of the string only contains digits and optional '-' sign */
        for (i = 0; i < strlen(value_str); i++) {
            if (!isdigit(value_str[i]) && value_str[i] != '-') {
                /* Error: non-digit character in immediate operand */
                lt->lex_union_option = lex_union_lex_error;
                strcpy(lt->lex_error, "Non-digit charchter was detected in operand.");
                return lex_op_addr_type_error;
            }
        }

        /* Check that the entire operand is a valid integer */

        value = strtol(value_str, &endptr, 10);
        if (*endptr != '\0') {
            /* Error: invalid integer */
            lt->lex_union_option = lex_union_lex_error; 
            strcpy(lt->lex_error, "Invalid iteger.");
            return lex_op_addr_type_error;
        }

        if (value >= -(1 << 11) && value < (1 << 11)) {
            /* Valid immediate operand */
            return lex_op_addr_type_immed;
        } else {
            /* Error: value out of range */
            lt->lex_union_option = lex_union_lex_error; 
            strcpy(lt->lex_error, "Out of range integer.");
            return lex_op_addr_type_error;
        }

    } else if (strlen(operand) == 2 && operand[0] == 'r' && isdigit(operand[1])) {
        /* Register */
        int reg_num = operand[1] - '0';
        if (reg_num >= 0 && reg_num <= 7) {
            return lex_op_addr_type_register;
        } else {
            lt->lex_union_option = lex_union_lex_error; 
            strcpy(lt->lex_error, "Invalid register number");
            return lex_op_addr_type_error;
        }
    } else if (is_valid_label(operand)) {
        /* Might be a defined label or not we'll save it for now */
        return lex_op_addr_type_label;
    } else {
        /* Error: unrecognized operand */
        lt->lex_union_option = lex_union_lex_error;
            strcpy(lt->lex_error, "Illegal operand.");
            return lex_op_addr_type_error;
    }
}