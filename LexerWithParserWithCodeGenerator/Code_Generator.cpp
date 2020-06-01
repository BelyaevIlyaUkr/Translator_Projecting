#include "Code_Generator.h"
void data_from_below_branches_processing(int*processing_state,string data_from_tree_branches_below,string* data,map<string,int>*Identifiers){
    static vector<string>declaration_attributes;
    if (data_from_tree_branches_below == "EXT" || data_from_tree_branches_below == "COMPLEX" || data_from_tree_branches_below == "SIGNAL"){ //checking extra attributes
        for(int j = 0; j < declaration_attributes.size(); j++){
            if (declaration_attributes[j] == data_from_tree_branches_below){ //check whether this attribute was already used in this declaration
                *processing_state = 1;
                error_processing(false,false,false,"-1","-1",NULL,data_from_tree_branches_below,11);
                break;
            }
            if (declaration_attributes[j] == "FLOAT" || declaration_attributes[j] == "BLOCKFLOAT" || declaration_attributes[j] == "INTEGER"){ //check whether before extra attribute there is base attribute
                *processing_state = 1;
                error_processing(false,false,false,"-1","-1",NULL,data_from_tree_branches_below,12);
                break;
            }
        }
        if (*processing_state == 1)
            return;
        else{
            declaration_attributes.push_back(data_from_tree_branches_below);
        }
    }
    else if (data_from_tree_branches_below == "FLOAT" || data_from_tree_branches_below == "BLOCKFLOAT" || data_from_tree_branches_below == "INTEGER"){ //checking base attributes
        for (int j =0; j < declaration_attributes.size(); j++){
            if (declaration_attributes[j] == "FLOAT" || declaration_attributes[j] == "BLOCKFLOAT" || declaration_attributes[j] == "INTEGER"){ //check whether there are more than one base attribute in declaration
                error_processing(false,false,false,"-1","-1",NULL,data_from_tree_branches_below,13);
                *processing_state = 1;
                break;
            }
            if (declaration_attributes[j] == "SIGNAL" && data_from_tree_branches_below == "BLOCKFLOAT"){ //check whether SIGNAL and BLOCKFLOAT are in one declaration(they are incompatible with each other because SIGNAL is for port and BLOCKFLOAT is array)
                error_processing(false,false,false,"-1","-1",NULL,data_from_tree_branches_below,14);
                *processing_state = 1;
                break;
            }
        }
        if (*processing_state != 1){
            declaration_attributes.push_back(data_from_tree_branches_below);
        }
        else return;
    }
    else if (*processing_state == 5){ //check presence of base attributes in declaration
        *processing_state = 0;
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
            error_processing(false,false,true,"-1","-1",NULL,data_from_tree_branches_below,15);
            *processing_state = 1;
            return;
        }
        declaration_attributes.clear();
        data_from_tree_branches_below = "";
        identifiers_processing(NULL,"0",NULL,"DD",&data_from_tree_branches_below); //get declaration in appropriate form
        *data = *data + data_from_tree_branches_below;
    }
    else {
        auto itr = Identifiers->find(data_from_tree_branches_below);
        if (itr == Identifiers->end()){ //just receive ready asm code parts from branches below
            *data = *data + data_from_tree_branches_below;
        }
        if (*processing_state == 3 || *processing_state == 1){
            return;
        }
    }
}
void error_processing(bool print_errors,bool write_position_for_base_absent_error,bool base_absent_error_process,string new_lexem_row,string new_lexem_column,FILE*f2,string currentLexeme,int error_type){
    static ostringstream errorMessages;

    static string current_lexem_column;
    static string current_lexem_row;
    static string declaration_begin_row;
    static string declaration_begin_column;

    if (write_position_for_base_absent_error){
        declaration_begin_row = new_lexem_row;
        declaration_begin_column = new_lexem_column;
        return;
    }

    if (print_errors)
        errors_out(&errorMessages,f2);
    else if (base_absent_error_process){
        error_collect(&errorMessages,error_type,currentLexeme,declaration_begin_row,declaration_begin_column);   
    }
    else if (new_lexem_row == "-1"){
        error_collect(&errorMessages,error_type,currentLexeme,current_lexem_row,current_lexem_column);
    }
    else{
        current_lexem_row = new_lexem_row;
        current_lexem_column = new_lexem_column;
    }
}
bool identifiers_processing(int* processing_state,string identifier_value,string*firstUsedIdentifier,string identifiers_type,string*identifiers_declaration){
    static vector<string>used_identifiers_value;
    static int current_declaration_identifiers_counter = 0;//count number of identifiers in current declaration
    if (firstUsedIdentifier != NULL){ //if we want to get first used identificator in code(it is always program or procedure identifier)
        *firstUsedIdentifier =used_identifiers_value[0];
        return false;
    }
    else if (identifiers_type != ""){ //if we want to get current declaration in appropriate form
        for(int i = 0; i < current_declaration_identifiers_counter; i++){

            if ((i == 0) && (used_identifiers_value.size() == current_declaration_identifiers_counter + 1)){
                *identifiers_declaration = *identifiers_declaration + "\n\t";
            }
            else if ((i == 0) && (used_identifiers_value.size() != current_declaration_identifiers_counter + 1)){
                *identifiers_declaration = *identifiers_declaration + ",";
            }

            *identifiers_declaration = *identifiers_declaration + 
            used_identifiers_value[used_identifiers_value.size() - current_declaration_identifiers_counter + i] + " : " + 
            identifiers_type;

            if ( (i >= 0) && (i != current_declaration_identifiers_counter - 1)){
                *identifiers_declaration = *identifiers_declaration + ",";
            }
        }

        current_declaration_identifiers_counter = 0;
        return false;
    }
    else{ //if we want to check current identifier on using in already processed declarations
        for(int i = 0; i < used_identifiers_value.size();i++){
            if(used_identifiers_value[i] == identifier_value)
                return true;
        }
        used_identifiers_value.push_back(identifier_value);
        if(*processing_state != 4)
            current_declaration_identifiers_counter++;
        return false;
    }
}
string tree_travel(struct node* syntax_tree,map<string,int>* Identifiers,int* processing_state){
    string data,data_from_tree_branches_below;
    if (syntax_tree->Nodes.size() == 0){ //processing ends of tree branches
        auto currentLexeme = syntax_tree->terminal;
        error_processing(false, false,false,currentLexeme["primary_lexem_row"],currentLexeme["primary_lexem_column"]);
        if (currentLexeme["lexemValue"] == "PROGRAM"){
            data = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *processing_state = 2;
        }

        if (currentLexeme["lexemValue"] == "PROCEDURE"){
            data = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *processing_state = 4;
        }

        if(currentLexeme["lexemValue"] == "("){
            *processing_state = 0;
        }

        if((currentLexeme["lexemValue"] == ";") && (*processing_state == 0)){
            *processing_state = 5;
        }

        if (currentLexeme["lexemValue"] == ")" || (currentLexeme["lexemValue"] == "BEGIN" 
        && *processing_state == 4)){
            string procedure_name;
            *processing_state = 6;
            identifiers_processing(NULL,"0",&procedure_name); // getting procedure name
            data = "\nendp " + procedure_name + "\ncode ends\n\tend " + procedure_name;
        }

        auto itr = Identifiers->find(currentLexeme["lexemValue"]);
        if (itr != Identifiers->end()){
            if(identifiers_processing(processing_state,currentLexeme["lexemValue"])){
                *processing_state = 1;
                error_processing(false,false,false,"-1","-1",NULL,currentLexeme["lexemValue"],10);
            }
            if (*processing_state == 2){
                *processing_state = 3; 
                return currentLexeme["lexemValue"]+ ":\n\tnop\ncode ends\n\tend " + currentLexeme["lexemValue"];
            }
            if(*processing_state == 4){
                return "proc " + currentLexeme["lexemValue"];
            }
            error_processing(false,true,false,currentLexeme["primary_lexem_row"],currentLexeme["primary_lexem_column"]);
            return currentLexeme["lexemValue"];
        }

        if (currentLexeme["lexemValue"] == "FLOAT" || currentLexeme["lexemValue"] =="BLOCKFLOAT" 
        || currentLexeme["lexemValue"] == "INTEGER" || currentLexeme["lexemValue"] == "EXT" 
        || currentLexeme["lexemValue"] == "SIGNAL" || currentLexeme["lexemValue"] == "COMPLEX"){
            return currentLexeme["lexemValue"];
        }

        return data;
    }
    else{ //processing parts which have children tree branches
        for(int i = 0; i < syntax_tree->Nodes.size() ; i++){
            data_from_tree_branches_below = tree_travel(syntax_tree->Nodes[i],Identifiers,processing_state);
            data_from_below_branches_processing(processing_state,data_from_tree_branches_below,&data,Identifiers);
            if (*processing_state == 1 || *processing_state == 3)
                break; 
        }
        return data;
    }
}
void Code_Generator(struct node* syntax_tree,map<string,int>* Identifiers,FILE*f2){
    int processing_state = 0;/* 1 - error found,2 - PROGRAM keyword found,3 - program identifier found
    4 - PROCEDURE keyword found,5 - ; was found in parentheses,0 - initial state and also - all symbols in parentheses except ; 
    6 - all symbols after ) or after procedure identifier(when parameters and parentheses are absent) */
    string data = tree_travel(syntax_tree,Identifiers,&processing_state);
    printAsmCodeInFile(data,f2);
    error_processing(true,false,false,"-1","-1",f2); // error printing
}