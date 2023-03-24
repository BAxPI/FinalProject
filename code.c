#include <stdlib.h>
#include "utils.h"
#include "lexer.h"
#include "globals.h"
#include "code.h"

/*
    Encodes a first word of a command.
    @param command_code - The command code.
    @param first_operand_type - The first operand type.
    @param second_operand_type - The second operand type.
    @param ic - The instruction counter.
    @param code_image - The code image.
    @param param1_type - The first parameter type.
    @param param2_type - The second parameter type.
*/
static void encode_first_word(enum inst_name command_code, enum operand_addr_type first_operand_type, enum operand_addr_type second_operand_type, 
                          long *ic, machine_w code_image[], enum operand_addr_type param1_type, enum operand_addr_type param2_type);

/*
    Encodees an immediate addressing type word.
    @param operand - The operand to be encoded.
    @param ic - The instruction counter.
    @param code_image - The code image.
*/
static void encode_immediate_word(int operand, long *ic, machine_w code_image[]);

/*
    Encodes a label addressing type word.
    @param operand - The operand to be encoded.
    @param ic - The instruction counter.
    @param code_image - The code image.
*/
static void encode_label_word(char label_operand[], long *ic, machine_w code_image[]);

/*
    Encodes a register addressing type word.
    @param first_register - The first register to be encoded.
    @param second_register - The second register to be encoded.
    @param ic - The instruction counter.
    @param code_image - The code image.
*/
static void encode_register_word(char first_register, char second_register, long *ic, machine_w code_image[]);

/*
    Encodes an integer to a 12 bits word.
    @param value - The integer to be encoded.
    @return - The encoded word.
*/
static imdt_drct_w encode_int_12_bits(int value);


bool encode_instruction(line_info *line, struct lex_tree *lt, machine_w *code_image, long *ic, s_table *symbol_table){
    /* Encoding first set commands - mov, cmp, add, sub, lea */
    if(lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_mov || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_cmp ||
        lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_add || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_sub ||
        lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_lea){
            /* lea */
            if(lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_lea){
                if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_immed ||
                    lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_register){
                        print_error_msg(line, "Invalid source operand addressing type for lea command");
                        return FALSE;
                }           
                else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_immed){
                    print_error_msg(line, "Invalid destination operand type for lea command.");
                    return FALSE;
                }
            }
            /* mov, cmp, add, sub */
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_immed && 
                lt->asm_inst_asm_dir.asm_inst.inst_name != lex_inst_cmp){
                    print_error_msg(line, "Invalid destination addressing type for mov, add, sub commands.");
                    return FALSE;
            }
            encode_first_word(lt->asm_inst_asm_dir.asm_inst.inst_name, lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0], 
                                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1], ic, code_image, 0, 0); /* No parameters */
            /* Both operands are registers */
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_register &&
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_register){
                    encode_register_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].reg,
                                        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].reg, ic, code_image);
                    return TRUE;
            }
            /* First operand immediate*/
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_immed){
                    encode_immediate_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].immed, ic, code_image);
            }
            /* First operand label */
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_label){
                encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].label, ic, code_image);
            }
            /* First operand register */
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_register){
                encode_register_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].reg, -1, ic, code_image);
            }
            /* Second operand immediate */
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_immed){
                encode_immediate_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].immed, ic, code_image);
                return TRUE;
            }
            /* Second operand label */
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_label){
                encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].label, ic, code_image);
                return TRUE;
            }
            /* Second operand register */
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_register){
                encode_register_word(-1, lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].reg, ic, code_image);
                return TRUE;
            }
    }
    /* Encoding second set commands with not parameters - not, clr, inc, dec, red, prn */
    if(lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_not || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_clr ||
        lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_inc || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_dec ||
        lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_red || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_prn){
        if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_immed &&
            lt->asm_inst_asm_dir.asm_inst.inst_name != lex_inst_prn){
                lt->lex_union_option = lex_union_lex_error;
                print_error_msg(line, "Illegal operand addressing type.");
                return FALSE;
            }
            encode_first_word(lt->asm_inst_asm_dir.asm_inst.inst_name, 0 , 
                                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat, ic, code_image, 0, 0);
                    
            if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_immed){
                encode_immediate_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.immed, ic, code_image);
                return TRUE;
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_label){
                encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label,ic,code_image);
                return TRUE;
            }
            else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_register){
                encode_register_word(-1, lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.reg, ic, code_image);
                return TRUE;
            }
    }
    /* Encoding second set commands with parameters - jmp, bne, jsr */
    else if (lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jmp || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_bne ||
            lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jsr){
                if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_immed ||
                    lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_register){
                    lt->lex_union_option = lex_union_lex_error;
                    print_error_msg(line, "Illegal operand addressing type.");
                    return FALSE;
                }
                else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_label){
                    encode_first_word(lt->asm_inst_asm_dir.asm_inst.inst_name, 0, lex_op_addr_type_label, ic, code_image, 0, 0);
                    encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label, ic, code_image);
                    return TRUE;
                }
                else{
                                    encode_first_word(lt->asm_inst_asm_dir.asm_inst.inst_name, 0, lex_op_addr_type_label_with_params, ic, code_image,
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0],
                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1]);
                encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label, ic, code_image);
                    /* Both parameters are registers */
                    if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_register &&
                        lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_register){
                            encode_register_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].reg,
                                                lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].reg, ic, code_image);
                            return TRUE;
                    }
                    /* First parameter immediate*/
                    if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_immed){
                            encode_immediate_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].immed, ic, code_image);
                    }
                    /* First parameter label */
                    else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_label){
                        encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].label, ic, code_image);
                    }
                    /* First parameter register */
                    else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_register){
                        encode_register_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].reg, -1, ic, code_image);
                    }
                    /* Second parameter immediate */
                    if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_immed){
                        encode_immediate_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].immed, ic, code_image);
                        return TRUE;
                    }
                    /* Second parameter label */
                    else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_label){
                        encode_label_word(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].label, ic, code_image);
                        return TRUE;
                    }
                    /* Second parameter register */
                    else if(lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_register){
                        encode_register_word(-1, lt->asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].reg, ic, code_image);
                        return TRUE;
                    }
                }
        }
        /* Encoding third set commands - rts, stop */
        if(lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_rts || lt->asm_inst_asm_dir.asm_inst.inst_name == lex_inst_stop){
            encode_first_word(lt->asm_inst_asm_dir.asm_inst.inst_name,0,0,ic,code_image,0,0);
            return TRUE;
        }
        return TRUE;
}            


static void encode_first_word(enum inst_name command_code, enum operand_addr_type first_operand_type, enum operand_addr_type second_operand_type, 
                          long *ic, machine_w code_image[], enum operand_addr_type param1_type, enum operand_addr_type param2_type){
    machine_w word;
    word.label = NULL;
    word.word.f_word = (first_w *)calloc(1, sizeof(first_w));
    word.word.f_word->op_code = command_code;
    word.word.f_word->src_op_addr = first_operand_type;
    word.word.f_word->dest_op_addr = second_operand_type;
    word.word.f_word->first_param = param1_type;
    word.word.f_word->second_param = param2_type;
    code_image[(*ic)++] = word;
}


static void encode_immediate_word(int operand, long *ic, machine_w code_image[]){
    machine_w word = {0};
    word.label = NULL;
    word.word.id_word = (imdt_drct_w*)calloc(1, sizeof(imdt_drct_w));
    *(word.word.id_word) = encode_int_12_bits(operand);
    word.word.id_word->ARE = 0;
    code_image[(*ic)++] = word;
}


static void encode_label_word(char label_operand[], long *ic, machine_w code_image[]){
    machine_w word;
    int len;
    for(len=0; label_operand[len] != '\0'; len++); /* Get the length of the label */
    word.label = calloc(len+1, sizeof(char));
    word.label[len] = '\0';
    word.word.f_word = NULL;
    strcpy(word.label, label_operand);
    code_image[(*ic)++] = word;
}


static void encode_register_word(char first_register, char second_register, long *ic, machine_w code_image[]){
    machine_w word;
    word.label = NULL; /* Set label to NULL to prevent problem when working with the word */
    word.word.r_word = (register_w *)calloc(1,sizeof(register_w));
    if(first_register >= 0){
        word.word.r_word->src_register = first_register - '0';
    }
    if(second_register >= 0){
        word.word.r_word->dest_register = second_register - '0';
    }
    code_image[(*ic)++] = word;
}


static imdt_drct_w encode_int_12_bits(int value) {
    imdt_drct_w result;
    /* Convert negative value to two's complement representation */
    if (value < 0) {
        unsigned int abs_value = abs(value);
        
        /* Complement and add one to get two's complement representation */
        unsigned int complement = (~abs_value) + 1;

        /* Set the most significant bit to indicate negative number */
        result.operand = (1 << 11) | (complement & 0x1FFF);
    } else {
        /* Set positive value to binary representation */
        result.operand = value & 0x1FFF;
    }
    return result;
}