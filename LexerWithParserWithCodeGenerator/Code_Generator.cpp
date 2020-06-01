#include "Code_Generator.h"
void declaration_attributes_actions(int*state,string asm_code_below,string* asm_code,map<string,int>*Identifiers){
    static vector<string>declaration_attributes;
        if (asm_code_below == "EXT" || asm_code_below == "COMPLEX" || asm_code_below == "SIGNAL"){
            for(int j = 0; j < declaration_attributes.size(); j++){
                if (declaration_attributes[j] == asm_code_below){
                    *state = 1;
                    errorProcessing(false,false,false,"-1","-1",NULL,asm_code_below,11);
                    break;
                }
                if (declaration_attributes[j] == "FLOAT" || declaration_attributes[j] == "BLOCKFLOAT" || declaration_attributes[j] == "INTEGER"){
                    *state = 1;
                    errorProcessing(false,false,false,"-1","-1",NULL,asm_code_below,12);
                    break;
                }
            }
            if (*state == 1)
                return;
            else{
                declaration_attributes.push_back(asm_code_below);
            }
        }
        else if (asm_code_below == "FLOAT" || asm_code_below == "BLOCKFLOAT" || asm_code_below == "INTEGER"){
            for (int j =0; j < declaration_attributes.size(); j++){
                if (declaration_attributes[j] == "FLOAT" || declaration_attributes[j] == "BLOCKFLOAT" || declaration_attributes[j] == "INTEGER"){
                    errorProcessing(false,false,false,"-1","-1",NULL,asm_code_below,13);
                    *state = 1;
                    break;
                }
                if (declaration_attributes[j] == "SIGNAL" && asm_code_below == "BLOCKFLOAT"){
                    errorProcessing(false,false,false,"-1","-1",NULL,asm_code_below,14);
                    *state = 1;
                    break;
                }
            }
            if (*state != 1){
                declaration_attributes.push_back(asm_code_below);
            }
            else return;
        }
        else if (*state == 5){
            *state = 0;
            bool base_attribute_absent = true;
            for(int j = 0; j < declaration_attributes.size(); j++){
                if (declaration_attributes[j] == "FLOAT" 
                || declaration_attributes[j] == "BLOCKFLOAT" 
                || declaration_attributes[j] == "INTEGER"){
                    base_attribute_absent = false;
                    break;
                }
            }
            if(base_attribute_absent){
                errorProcessing(false,false,true,"-1","-1",NULL,asm_code_below,15);
                *state = 1;
                return;
            }
            declaration_attributes.clear();
            asm_code_below = "";
            identifier_identity(NULL,"0",NULL,"DD",&asm_code_below);
            *asm_code = *asm_code + asm_code_below;
        }
        else { //ready returned phrases
            auto itr = Identifiers->find(asm_code_below);
            if (itr == Identifiers->end()){
                *asm_code = *asm_code + asm_code_below;
            }
            if (*state == 3 || *state == 1){ //or program identifier was found either there are few equal identifiers
                return;
            }
        }
}
void errorProcessing(bool printErrors,bool base_absent,bool base_absent_error,string new_lexem_row,string new_lexem_column,FILE*f2,string currentLexeme,int error_type,string expectedLexeme){
    static ostringstream errorMessages;

    static string lexem_column;
    static string lexem_row;
    static string declaration_begin_row;
    static string declaration_begin_column;

    if (base_absent){
        declaration_begin_row = new_lexem_row;
        declaration_begin_column = new_lexem_column;
        return;
    }

    if (printErrors)
        errors_out(&errorMessages,f2);
    else if (base_absent_error){
        error_collect(&errorMessages,error_type,currentLexeme,declaration_begin_row,declaration_begin_column,expectedLexeme);   
    }
    else if (new_lexem_row == "-1"){
        error_collect(&errorMessages,error_type,currentLexeme,lexem_row,lexem_column,expectedLexeme);
    }
    else{
        lexem_row = new_lexem_row;
        lexem_column = new_lexem_column;
    }
}
bool identifier_identity(int* state,string identifier_value,string*firstUsedIdentifier,string identifiers_type,string*few_identifiers_declaration){
    static vector<string>used_identifiers_value;
    static int one_type_identifiers_count = 0;
    if (firstUsedIdentifier != NULL){
        *firstUsedIdentifier =used_identifiers_value[0];
        return false;
    }
    else if (identifiers_type != ""){
        for(int i = 0; i < one_type_identifiers_count; i++){
            if (used_identifiers_value.size() > 2){
                *few_identifiers_declaration = *few_identifiers_declaration + ", ";
            }
            else{
                *few_identifiers_declaration = *few_identifiers_declaration + "\n\t";
            }

            *few_identifiers_declaration = *few_identifiers_declaration + 
            used_identifiers_value[used_identifiers_value.size() - one_type_identifiers_count + i] + " : " + 
            identifiers_type;
        }

        one_type_identifiers_count = 0;
        return false;
    }
    else{
        for(int i = 0; i < used_identifiers_value.size();i++){
            if(used_identifiers_value[i] == identifier_value)
                return true;
        }
        used_identifiers_value.push_back(identifier_value);
        if(*state != 4)
            one_type_identifiers_count++;
        return false;
    }
}
string tree_travel(struct node* syntax_tree,map<string,int>* Identifiers,int* state){
    string asm_code,asm_code_below;
    if (syntax_tree->Nodes.size() == 0){ //processing ends of tree branches
        auto currentLexeme = syntax_tree->terminal;
        errorProcessing(false, false,false,currentLexeme["primary_lexem_row"],currentLexeme["primary_lexem_column"]);
        if (currentLexeme["lexemValue"] == "PROGRAM"){
            asm_code = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *state = 2;
        }

        if (currentLexeme["lexemValue"] == "PROCEDURE"){
            asm_code = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *state = 4;
        }

        if(currentLexeme["lexemValue"] == "("){
            *state = 0;
        }

        if((currentLexeme["lexemValue"] == ";") && (*state == 0)){
            *state = 5;
        }

        if (currentLexeme["lexemValue"] == ")" || (currentLexeme["lexemValue"] == "BEGIN" 
        && *state == 4)){
            string procedure_name;
            *state = 6;
            identifier_identity(NULL,"0",&procedure_name);
            asm_code = "\nendp\t" + procedure_name + "\ncode ends\n\tend\t" + procedure_name;
        }

        auto itr = Identifiers->find(currentLexeme["lexemValue"]);
        if (itr != Identifiers->end()){
            if(identifier_identity(state,currentLexeme["lexemValue"])){
                *state = 1; //means that code execution have to be ended after finding error
                errorProcessing(false,false,false,"-1","-1",NULL,currentLexeme["lexemValue"],10);
            }
            if (*state == 2){
                *state = 3; //means that code execution have to be ended after PROGRAM finding
                return currentLexeme["lexemValue"]+ ":\n\tnop\ncode ends\n\tend\t" + currentLexeme["lexemValue"];
            }
            if(*state == 4){ //finding procedure identifier
                return "proc\t" + currentLexeme["lexemValue"];
            }
            errorProcessing(false,true,false,currentLexeme["primary_lexem_row"],currentLexeme["primary_lexem_column"]);
            return currentLexeme["lexemValue"]; // returning identifiers
        }

        if (currentLexeme["lexemValue"] == "FLOAT" || currentLexeme["lexemValue"] =="BLOCKFLOAT" 
        || currentLexeme["lexemValue"] == "INTEGER" || currentLexeme["lexemValue"] == "EXT" 
        || currentLexeme["lexemValue"] == "SIGNAL" || currentLexeme["lexemValue"] == "COMPLEX"){
            return currentLexeme["lexemValue"];
        }

        return asm_code;
    }
    else{
        for(int i = 0; i < syntax_tree->Nodes.size() ; i++){
            asm_code_below = tree_travel(syntax_tree->Nodes[i],Identifiers,state);
            declaration_attributes_actions(state,asm_code_below,&asm_code,Identifiers);
            if (*state == 1 || *state == 3)
                break; 
        }
        return asm_code;
    }
}
void Code_Generator(struct node* syntax_tree,map<string,int>* Identifiers,FILE*f2){
    int state = 0;
    string asm_code = tree_travel(syntax_tree,Identifiers,&state);
    printAsmCodeInFile(asm_code,f2);
    errorProcessing(true,false,false,"-1","-1",f2);
}