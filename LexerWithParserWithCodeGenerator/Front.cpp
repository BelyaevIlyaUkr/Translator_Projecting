#include "Front.h"

void lexemes_out(string lexemes) {
    cout<<lexemes;
}


void error_collect(ostringstream* errorMessages,int error_type,int current_row,int current_column,int lexCode) {
    if (error_type == 1){
        *errorMessages<<"Lexer:Error:(line:" << current_row << ",column:"<< current_column
        << "):  *) expected but end of file found\n";
    }
    else if (error_type == 2){
            *errorMessages << "Lexer:Error:(line:"<<current_row<< ",column:"
            << current_column << "):Illegal symbol " << (char)lexCode <<"\n\n";
    }
    else if (error_type == 3){
            *errorMessages << "Lexer:Error:(line:"<<current_row<< ",column:"
            << current_column << "):Illegal digits using" <<"\n\n";
    }
}


void error_collect(ostringstream* errorMessages,int error_type,map<string,string> currentLexeme,string expectedLexeme){
    if (currentLexeme["lexemValue"] == "")
        currentLexeme["lexemValue"] = "nothing";

    if (error_type == -1){
        currentLexeme["primary_lexem_row"] = "1";
        currentLexeme["primary_lexem_column"] = "1";
        error_type = 4;
    }
    else if (currentLexeme["primary_lexem_row"] ==""){
        currentLexeme["primary_lexem_row"] = "last";
        currentLexeme["primary_lexem_column"] = "last + 1";
    }

    if (error_type == 4){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            <<currentLexeme["primary_lexem_column"]<< "): Keyword "<<expectedLexeme<<" here expected but "
            <<currentLexeme["lexemValue"]<< " found" <<"\n\n";
    }
    else if (error_type == 5){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            <<currentLexeme["primary_lexem_column"]<< "):"<< "Absence of any lexemes after the end of signal program expected but "<<
            currentLexeme["lexemValue"]<<" found " <<"\n\n";
    }
    else if (error_type == 6){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            <<currentLexeme["primary_lexem_column"]<< "): Identifier lexem expected but "
            <<currentLexeme["lexemValue"]<<" found.\n\n";
    }
    else if (error_type == 7){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            << currentLexeme["primary_lexem_column"] << "): Symbol  "<<expectedLexeme<< "  here expected but "
            <<currentLexeme["lexemValue"]<<" found"<<"\n\n";
    }
    else if (error_type == 8){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            <<currentLexeme["primary_lexem_column"]<< "): Attribute lexem or ; expected but "
            <<currentLexeme["lexemValue"]<<" found.\n\n";
    }    
    else if (error_type == 9){
            *errorMessages << "Parser:Error:(line:"<<currentLexeme["primary_lexem_row"]<< ",column:"
            <<currentLexeme["primary_lexem_column"]<< "): Attribute lexem expected but "
            <<currentLexeme["lexemValue"]<<" found.\n\n";
    } 
}

void error_collect(ostringstream* errorMessages,int error_type,string currentLexemeValue,string lexem_row,string lexem_column){
    if (error_type == 10){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): Identifier "<< currentLexemeValue << " was already used in declaring earlier\n";
    } 
    if (error_type == 11){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): Extra attribute "<< currentLexemeValue << " is already used in this declaration\n";
    } 
    if (error_type == 12){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): Attribute "<< currentLexemeValue << ",as an extra attribute,can't take place after base attributes\n";
    } 
    if (error_type == 13){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): Attribute "<< currentLexemeValue << " can't take place here because more than one base attribute can't be in one declaration\n";
    } 
    if (error_type == 14){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): Attribute "<< currentLexemeValue << " can't take place here because it can't be with attribute SIGNAL in one declaration\n";
    } 
    if (error_type == 15){
        *errorMessages << "\nCode Generator:Error:(line:"<< lexem_row << ",column:"<< lexem_column
        << "): There isn't any base attribute in this declaration\n";
    } 
}


void errors_out(ostringstream* errorMessages,FILE*f2){
    if (f2 != NULL)
        fputs((errorMessages->str()).c_str(),f2);
    else cout << errorMessages->str();
}


void identifiers_out(map<string,int>* Identifiers){
    cout<<"Identifiers_table\n";
    if (Identifiers->size() == 0){
        cout<<"Empty\n";
        return;
    }
    map<string,int>::iterator it;
    for(it = Identifiers->begin(); it != Identifiers->end(); it++){
        cout<< it->first <<"\t" <<it->second << "\n";
    }
    cout<<"\n";
}

void Empty_File_Msg(){
    cout<<"File is empty\n";
}

void Attributes_out(map<int,int>* Attributes){
    cout<< "Table with ASCII and their attributes\n";

    map<int,int>::iterator it;
    for(it = Attributes->begin(); it!=Attributes->end(); it++){
        cout << (char)it->first << "\t" << it->second << "\n";
    }
    cout<<"All other characters from ASCII don't match conditions and are"
        << "perceived as not correct(except small latin literatures)\n\n";
}

void Reserved_Words_out(map<string,int>* Reserved_Words){
    cout<< "Table with Reserved Words\n";

    map<string,int>::iterator it;
    for(it = Reserved_Words->begin(); it!=Reserved_Words->end(); it++){
        cout << it->first << " " << it->second << "\n";
    }
}

void printTree(node*Tree,int dotNumber){
    if (Tree == NULL)
        return;

    for(int i = 0; i<dotNumber; i++)
        cout<<".";

    if (Tree->nonTerminal != ""){
        cout<<Tree->nonTerminal<<"\n";
        for(int i = 0; i<Tree->Nodes.size(); i++)
            printTree(Tree->Nodes[i],dotNumber+2);
    }
    else {
        cout<<Tree->terminal["lexCode"]<<" "<<Tree->terminal["lexemValue"]<<"\n";
        for(int i = 0; i<Tree->Nodes.size(); i++)
            printTree(Tree->Nodes[i],dotNumber+2);
    }
}

void printTreeInFile(node*Tree,FILE* f2,int dotNumber){
    if (Tree == NULL)
        return;

    char dot = '.';
    for(int i = 0; i<dotNumber; i++){
        fputc(dot,f2);
    }

    string str = "";
    if (Tree->nonTerminal != ""){
        str += Tree->nonTerminal + "\n";
    }
    else {
        str += Tree->terminal["lexCode"] + " " + Tree->terminal["lexemValue"] + "\n";
    }
    auto cstr = str.c_str();
    fputs(cstr,f2);
    for(int i = 0; i<Tree->Nodes.size(); i++)
        printTreeInFile(Tree->Nodes[i],f2,dotNumber+2);
}

void printAsmCode(string asm_code){
    cout << asm_code << endl;
}

void printAsmCodeInFile(string asm_code,FILE* f2){
    fputs(asm_code.c_str(),f2);
}
