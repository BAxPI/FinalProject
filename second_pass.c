#include "tables.h"
#include "globals.h"
#include "lexer.h"
#include "utils.h"


bool spass_line_process(struct lex_tree lt, s_table *table, line_info line){
    s_table_entry *entry;
    if(lt.lex_union_option == lex_union_asm_inst){
        return TRUE;
    }
    if(lt.asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry){
        entry = get_from_s_table(table, lt.asm_inst_asm_dir.asm_dir.dir.label);
        if(entry == NULL){
            print_error_msg(&line, "Label not defined");
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


