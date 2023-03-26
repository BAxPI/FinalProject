#include "tables.h"
#include "globals.h"
#include "lexer.h"
#include "utils.h"


bool spass_line_process(struct lex_tree lt, s_table *table, line_info line){
    s_table_entry *entry;
    /* If it is instruction verify that all the labels used in the instruction are defined! */
    if(lt.lex_union_option == lex_union_asm_inst){
        if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_add || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_sub ||
           lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_cmp || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_lea ||
           lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_mov){
            if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[0] == lex_op_addr_type_label){

                if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].label) == NULL){
                    print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[0].label);
                    return FALSE;
                }
            }
            else if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oat[1] == lex_op_addr_type_label){
                if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].label) == NULL){
                    print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setA.oatu[1].label);
                    return FALSE;
                }
            }
        }
        else if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_inc || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_dec ||
           lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_not || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_clr ||
           lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_prn || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_red){
            if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_label){
                if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label) == NULL){
                    print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label);
                    return FALSE;
                }
            }
        }

        else if(lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jsr || lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_bne ||
                lt.asm_inst_asm_dir.asm_inst.inst_name == lex_inst_jmp){
            if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.oat == lex_op_addr_type_label){
                    if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label) == NULL){
                    print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label);
                    return FALSE;
                }
            }
            else{
                if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label) == NULL){
                    print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.oatu.label);
                    return FALSE;
                }
                else if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[0] == lex_op_addr_type_label){
                    if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].label) == NULL){
                        print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[0].label);
                        return FALSE;
                    }
                }
                else if(lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oat[1] == lex_op_addr_type_label){
                    if(get_from_s_table(table, lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].label) == NULL){
                        print_error_msg(&line, "The label %s is used but not defined", lt.asm_inst_asm_dir.asm_inst.asm_inst_sets.asm_inst_setB.setB_operands.setB_param_label.oatu[1].label);
                        return FALSE;
                    }
                }
            }

        }
           
        return TRUE;
    }
    if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry){
        entry = get_from_s_table(table, lt.asm_inst_asm_dir.asm_dir.dir.label);
        if(entry == NULL){
            print_error_msg(&line, "The label %s is used but not defined"), lt.asm_inst_asm_dir.asm_dir.dir.label;
            return FALSE;
        }
        else{
            entry->symbol_type = ENTRY_SYMBOL;
        }
    }
    else if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_extern){
        entry = get_from_s_table(table, lt.asm_inst_asm_dir.asm_dir.dir.label);
        if(entry == NULL){
            print_error_msg(&line, "Extern label is declared but not used in the file");
            return FALSE;
        }
        else{
            entry->symbol_type = EXTERN_SYMBOL;
            return TRUE;
        }
    }
    return TRUE;  
}


void spass_label_encoding(s_table *symbol_table, machine_w *code_image, long ic){
    int i;
    for(i=0; i < ic; i++){
        if(code_image[i].label != NULL){
            s_table_entry *entry = get_from_s_table(symbol_table, code_image[i].label);
            if(entry == NULL){
                continue;
            }
            if(entry->symbol_type == EXTERN_SYMBOL){
                code_image[i].word.id_word = (imdt_drct_w*)calloc(1,sizeof(imdt_drct_w));
                code_image[i].word.id_word->ARE = 1;
                code_image[i].word.id_word->operand = 0; 
            } 
            else{
                code_image[i].word.id_word = (imdt_drct_w*)calloc(1, sizeof(imdt_drct_w));
                code_image[i].word.id_word->ARE = 2;
                code_image[i].word.id_word->operand= (entry->symbol_addr) + IC_INIT_VALUE;
            }
        }
    }
}


