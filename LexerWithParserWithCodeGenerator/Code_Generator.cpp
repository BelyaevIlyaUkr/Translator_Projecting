/*void errorProcessing(bool printErrors,FILE*f2,map<string,string> currentLexeme={},int error_type=0,string expectedLexeme=""){
    static ostringstream errorMessages;

    if (printErrors)
        errors_out(&errorMessages,f2);
    else
        error_collect(&errorMessages,error_type,currentLexeme,expectedLexeme);
}*/
bool identifier_identity(string identifier_value,string*firstUsedIdentifier = NULL){
    static vector<string>used_identifiers_value;
    if (firstUsedIdentifier != NULL){
        *firstUsedIdentifier =used_identifiers_value[0];
        return false;
    }
    for(int i = 0; i < used_identifiers_value->size();i++){
        if(used_identifiers_value[i] == identifier_value)
            return true;
    }
    used_identifiers_value.insert(identifier_value);
    return false;
}
string tree_travel(node* syntax_tree,<string,int>* Identifiers,int* state){
    string asm_code;
    if (syntax_tree->Nodes.size() == 0){
        auto current_lexeme = syntax_tree->terminal;
        if (current_lexeme["lexCode"] == "PROGRAM"){
            asm_code = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *state = 2;
            return asm_code;
        }
        if (current_lexeme["lexCode"] == "PROCEDURE"){
            asm_code = "data segment\ndata ends\ncode segment\n\tassume cs:code,ds:data\n";
            *state = 4;
            return asm_code;
        }
        if (current_lexeme["lexCode"] == ")"){
            string procedure_name;
            identifier_identity("0",&procedure_name);
            asm_code = "endp\t" + procedure_name + "\ncode ends\n\tend\t" + procedure_name;
            return asm_code;
        }
        auto itr = Identifiers->find(currentLexeme["lexemValue"]);
        if (itr != Identifiers->end()){
            if(identifier_identity(currentLexeme["lexemValue"])){
                *state = 1; //means that code execution have to be ended
            }
            if (*state == 2){
                *state = 3; //means that code execution have to be ended
                return currentLexeme["lexemValue"]+ ":\n\tnop\ncode ends\n\tend\t" + currentLexeme["lexemValue"];
            }
            if(*state == 4){
                *state = 0;
                return "proc\t" + currentLexeme["lexemValue"] + "\n";
            }
        }
        return currentLexeme["lexemValue"];
    }
    else{
        vector<string>declaration_attributes;
        for(int i = 0; i < Nodes.size() ; i++){
            asm_code_below = tree_travel(syntax_tree->Nodes[i],Identifiers);
            if (*state == 3){
                return asm_code_below;
            }
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
            }
            if (asm_code_below == "FLOAT" || asm_code_below == "BLOCKFLOAT" || asm_code_below == "INTEGER"){
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
                    asm_code_below = "\tdd\n";
                }
                else break;
            }
            if ((*state) == 0 || (*state) == 2){
                asm_code = asm_code + asm_code_below;
                break;
            }
        }
        return asm_code;
    }
}
void Code_Generator(node* syntax_tree,<string,int>* Identifiers,FILE*f2){
    int state = 0;
    string asm_code = tree_travel(syntax_tree,Identifiers,&state);
    cout << asm_code;
}