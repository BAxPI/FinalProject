/*
    This file creates the lexer for the assembler.
    The lexer is a recursive descent parser that creates an Abstract Syntax Tree of the line.
*/

#ifndef assembler_lexer
#define assembler_lexer


#define MAX_LABEL_SIZE 30
#define MAX_LEX_ERROR_BUFF_LEN 150
#define MAX_STRING_LEN 80
#define MAX_NUMBER_SIZE 80

enum inst_name {
    /* Set A commands */
    lex_inst_mov,
    lex_inst_cmp,
    lex_inst_add,
    lex_inst_sub,
    lex_inst_lea,
    /* Set B commands */
    lex_inst_not,
    lex_inst_clr,
    lex_inst_inc,
    lex_inst_dec,
    lex_inst_jmp,
    lex_inst_bne,
    lex_inst_red,
    lex_inst_prn,
    lex_inst_jsr,
    /* Set C commands */
    lex_inst_rts,
    lex_inst_stop
};

enum operand_addr_type {
    lex_op_addr_type_error = -1,
    lex_op_addr_type_immed,
    lex_op_addr_type_label,
    lex_op_addr_type_label_with_params,
    lex_op_addr_type_register
};

union operand_addr_type_u {
    char label[MAX_LABEL_SIZE +1];
    char reg;
    int immed;
};

struct lex_tree {
    char label[MAX_LABEL_SIZE + 1];
    char lex_error[MAX_LEX_ERROR_BUFF_LEN + 1];
    enum {
        lex_union_lex_error = -1,
        lex_union_asm_inst,
        lex_union_asm_dir,   /* .dir,.string,.extern,.entry */  
        lex_union_asm_empty_line,
        lex_union_asm_comment
    }lex_union_option;
    union {
        
        struct {
            enum {
                lex_union_asm_dir_string,
                lex_union_asm_dir_data,
                lex_union_asm_dir_entry,
                lex_union_asm_dir_extern
            }lex_union_asm_dir_option;
            union {
                char string[MAX_STRING_LEN +1];
                struct {
                     int num_array[MAX_NUMBER_SIZE];
                     int num_count;
                }num_array;
                char label[MAX_LABEL_SIZE +1];
            }dir;
        }asm_dir;
        
        struct {
            enum inst_name inst_name;
            union {
                struct {
                    enum operand_addr_type      oat[2];
                    union operand_addr_type_u   oatu[2];
                }asm_inst_setA;

                struct {
                    enum operand_addr_type oat;
                    union {
                        struct {
                            char label[MAX_LABEL_SIZE +1];
                            enum operand_addr_type oat[2];
                            union operand_addr_type_u oatu[2];
                        }setB_param_label;
                        union operand_addr_type_u oatu;
                    }setB_operands;
                }asm_inst_setB;
            }asm_inst_sets;
        }asm_inst;

    }asm_inst_asm_dir;
};

/*
    This function gets a line and returns an Abstract Syntax Tree of the line.
    @param line The line to parse
    @return The Abstract Syntax Tree of the line (struct lex_tree)
*/
struct lex_tree lexer_get_lex_tree_from_line(const char * line);

#endif
