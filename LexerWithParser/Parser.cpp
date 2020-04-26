#include "Parser.h"

struct node  { 
    string terminal;
    string nonTerminal; 
    struct node *left; 
    struct node *right; 
}; 


struct node* newNode(string terminal,string nonTerminal) {  
    struct node* node = new struct node*; 

    node->terminal = terminal;
    node->nonTerminal = nonTerminal;
    node->left = NULL; 
    node->right = NULL; 

    return(node); 
}


void errorProcessing(){

}


void block(){

}


void identifier(){

}

void procedureIdentifier(node* tree){
    node* Tree = newNode(NULL,"<signal-program>");
    identifier()
}

void program(map<string,string> currentLexeme,node* Tree){
    if (currentLexeme["lexCode"] == "401"){
        procedureIdentifier(readNextLexeme(),Tree->right);
        if(readNextLexeme() == ";")
            block();
    }
    else if (currentLexeme["lexCode"] == "402")
        procedureIdentifier(readNextLexeme(),Tree->right);
        if(readNextLexeme() == ";")
            block();

    else {
        errorProcessing();
    }
    block()
}

node* signalProgram(string lexemes){
    node* Tree = newNode(NULL,"<signal-program>");

    auto firstLexeme = readNextLexeme(lexemes);
    Tree = program(firstLexeme,Tree->right);

    return Tree;
}

map<string,string>* readNextLexeme(string lexemesInit = NULL){
    static string lexemes;

    if (lexemesInit != NULL)
        lexemes = lexemesInit;
   
    map<string,string> nextLexeme;

    static int scannerPosition = 0;

    while(lexemes[scannerPosition]!= '\n'){
        string strTmp= "";
        while(lexemes[scannerPosition]!= '\n' && lexemes[scannerPosition]!= '\t'){
            strTmp += lexemes[scannerPosition];
            scannerPosition++;
        }
        switch(nextLexeme.size()) {
            case 0:
                nextLexeme.insert("primary_lexem_row",strTmp);
            case 1:
                nextLexeme.insert("primary_lexem_column",strTmp);
            case 2:
                nextLexeme.insert("lexCode",strTmp);
            case 3:
                nextLexeme.insert("lexemValue",strTmp);
        }
    }

    return nextLexeme
}


void Parser(string lexemes,map<string,int>* Identifiers){
    node* Tree = signalProgram(readNextLexeme(lexemes));
}