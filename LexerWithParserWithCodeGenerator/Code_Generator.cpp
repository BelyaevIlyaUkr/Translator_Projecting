#include "Code_Generator.h"
/*void errorProcessing(bool printErrors,FILE*f2,map<string,string> currentLexeme={},int error_type=0,string expectedLexeme=""){
    static ostringstream errorMessages;

    if (printErrors)
        errors_out(&errorMessages,f2);
    else
        error_collect(&errorMessages,error_type,currentLexeme,expectedLexeme);
}*/
bool identifier_identity(string identifier_value,string*firstUsedIdentifier){
    static vector<string>used_identifiers_value;
    if (firstUsedIdentifier != NULL){
        *firstUsedIdentifier =used_identifiers_value[0];
        return false;
    }
    for(int i = 0; i < used_identifiers_value.size();i++){
        if(used_identifiers_value[i] == identifier_value)
            return true;
    }
    used_identifiers_value.push_back(identifier_value);
    return false;
}
string tree_travel(struct node* syntax_tree,map<string,int>* Identifiers,int* state){
    string asm_code,asm_code_below;
    if (syntax_tree->Nodes.size() == 0){ //processing ends of tree branches
        auto currentLexeme = syntax_tree->terminal;

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

        if (currentLexeme["lexemValue"] == ")" || (currentLexeme["lexemValue"] == "BEGIN" 
        && *state == 4)){
            string procedure_name;
            *state = 0;
            identifier_identity("0",&procedure_name);
            asm_code = "endp\t" + procedure_name + "\ncode ends\n\tend\t" + procedure_name;
        }

        auto itr = Identifiers->find(currentLexeme["lexemValue"]);
        if (itr != Identifiers->end()){
            if(identifier_identity(currentLexeme["lexemValue"])){
                *state = 1; //means that code execution have to be ended after finding error
            }
            if (*state == 2){
                *state = 3; //means that code execution have to be ended after PROGRAM finding
                return currentLexeme["lexemValue"]+ ":\n\tnop\ncode ends\n\tend\t" + currentLexeme["lexemValue"];
            }
            if(*state == 4){ //finding procedure identifier
                return "proc\t" + currentLexeme["lexemValue"] + "\n";
            }
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
        vector<string>declaration_attributes;
        for(int i = 0; i < syntax_tree->Nodes.size() ; i++){
            asm_code_below = tree_travel(syntax_tree->Nodes[i],Identifiers,state);
            if (asm_code_below == "EXT" || asm_code_below == "COMPLEX" || asm_code_below == "SIGNAL"){
                for(int i = 0; i < declaration_attributes.size(); i++){
                    if (declaration_attributes[i] == asm_code_below){
                        *state = 1;
                        break;
                        //TODO error proccessing
                    }
                    if(i == (declaration_attributes.size() - 1)){
                        if (declaration_attributes[i] == "FLOAT" || declaration_attributes[i] == "BLOCKFLOAT" || declaration_attributes[i] == "INTEGER"){
                            *state = 1;
                            break;
                            //TODO error proccessing
                        }
                    }
                }
                if (*state == 1)
                    break;
                else declaration_attributes.push_back(asm_code_below);
            }
            else if (asm_code_below == "FLOAT" || asm_code_below == "BLOCKFLOAT" || asm_code_below == "INTEGER"){
                for (int i =0; i < declaration_attributes.size(); i++){
                    if (declaration_attributes[i] == asm_code_below){
                        *state = 1;
                        break;
                        //TODO error processing
                    }
                    if (declaration_attributes[i] == "SIGNAL" && asm_code_below == "BLOCKFLOAT"){
                        *state = 1;
                        break;
                        //TODO error processing
                    }
                }
                if (*state != 1){
                    declaration_attributes.push_back(asm_code_below);
                    asm_code = asm_code + asm_code_below = "\tDD\n";
                }
                else break;
            }
            else { //identifier or nonterminal was found 
                asm_code = asm_code + asm_code_below;
                if (*state == 3 || *state == 1) //or program identifier was found either there are few equal identifiers
                    break;
            }
        }
        return asm_code;
    }
}
void Code_Generator(struct node* syntax_tree,map<string,int>* Identifiers,FILE*f2){
    int state = 0;
    string asm_code = tree_travel(syntax_tree,Identifiers,&state);
    printAsmCodeInFile(asm_code,f2);
}