
#include "code.h"
#include "directives.h"
#include "tables.h"
#include "utils.h"
#include "lexer.h"
#include "globals.h"




bool first_pass_line_process(line_info line, s_table *symbol_table, long *ic, long *dc,
                             data_w *data_image, machine_w *code_image, struct lex_tree *lt) {
    /* Line is a label definition */
    if(lt->label[0] != '\0'){
        /* check if label already defined */
        if(get_from_s_table(symbol_table, lt->label) != NULL){
            if(get_from_s_table(symbol_table, lt->label)->symbol_type == EXTERN_SYMBOL){
                print_error_msg(&line, "Extern label can't be defined in the same file.");
                return FALSE;
            }
            else{
                print_error_msg(&line, "Label already defined.");
                return FALSE;
            }            
        }
        if(lt->lex_union_option == lex_union_asm_dir){
            if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_string || 
                lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_data){
                    if(lt->label &&  get_from_s_table(symbol_table, lt->label) == NULL){
                    add_s_entry(symbol_table, lt->label, *dc, DATA_SYMBOL);
                }
                if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_string){
                    encode_string(lt->asm_inst_asm_dir.asm_dir.dir.string, data_image, dc);
                }
                else{
                    encode_data(lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_array, 
                                    lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_count, data_image, dc);
                    
                }
            }
            else if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry || 
                    lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_extern){
                    print_error_msg(&line, ".entry or .extern in label definition are useless.");
                return FALSE;
            }
        }
        if(lt->lex_union_option == lex_union_asm_inst){
            add_s_entry(symbol_table, lt->label, *ic, CODE_SYMBOL);
            return encode_instruction(&line, lt, code_image, ic, symbol_table);  
        }
        
    }
    else{/* Not a label decleration */ 
        if(lt->lex_union_option == lex_union_asm_dir){
            if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_string || 
            lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_data){
                if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_string){
                encode_string(lt->asm_inst_asm_dir.asm_dir.dir.string, data_image, dc);
                }
                else{
                    encode_data(lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_array, 
                                lt->asm_inst_asm_dir.asm_dir.dir.num_array.num_count, data_image, dc);
                   
                }
            }
            else if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_extern){
                add_s_entry(symbol_table, lt->asm_inst_asm_dir.asm_dir.dir.label, 0, EXTERN_SYMBOL);
                return TRUE;
            }
            else if(lt->asm_inst_asm_dir.asm_dir.lex_union_asm_dir_option == lex_union_asm_dir_entry){
                return TRUE;
            }
        }
        else if(lt->lex_union_option == lex_union_asm_inst){
            return encode_instruction(&line, lt, code_image, ic, symbol_table);
        }
    }
    return TRUE;
}



