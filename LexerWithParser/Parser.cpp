#include "Parser.h"
#include "Front.h"


struct node  { 
    map<string,string> terminal;
    string nonTerminal; 
    vector<node*> Nodes;
}; 


struct node* newNode(string nonTerminal = "") {  
    node* node = new struct node; 
    node->nonTerminal = nonTerminal;
    return(node); 
}


void errorProcessing(map<string,string> currentLexeme,int error_type,string expectedLexeme=""){
    static ostringstream errorMessages;
    
    error_collect(&errorMessages,error_type,currentLexeme,expectedLexeme);
}


void statementsList(node*Tree,map<string,string> currentLexeme){

    if (currentLexeme["lexCode"] == "403") {
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[0])->terminal = currentLexeme;
    else
        errorProcessing(currentLexeme,4,"BEGIN");

    currentLexeme = readNextLexeme();
    if(currentLexeme["lexCode"] == "404") {
            Tree->Nodes.push_back(newNode("<empty>"));

            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[1])->terminal = currentLexeme;

            currentLexeme = readNextLexeme();
    }
    else{
        errorProcessing(currentLexeme,5,"");
        errorProcessing(currentLexeme,4,"END");
    }
    
}


void block(node*Tree,map<string,string> currentLexeme){
    Tree->Nodes.push_back(newNode("<statements-list>"));
    statementsList(Tree->Nodes[0],currentLexeme);
}


void identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<identifier>"));
    auto itr = Identifiers->find(currentLexeme);
    if( itr == Identifiers->end() ) {
        errorProcessing(currentLexeme,6,"");
    }
    else
        (Tree->Nodes[0])->terminal = currentLexeme;
}

void procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<procedure-identifier>"));
    identifier(currentLexeme,Tree->Nodes[0],Identifiers);
}

void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    if (currentLexeme["lexCode"] == "401"){

        procedureIdentifier(readNextLexeme(),Tree,Identifiers);

        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "46" ) {

            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[1])->terminal = currentLexeme;

            currentLexeme = readNextLexeme();
        }
        else
            errorProcessing(currentLexeme,7,";");

        Tree->Nodes.push_back(newNode("<block>"));
        block(Tree->Nodes[2],currentLexeme);

        if( currentLexeme["lexCode"] == "59" ) {
            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[3])->terminal = currentLexeme;

            currentLexeme = readNextLexeme();
        }
        else
            errorProcessing(currentLexeme,7,".");
        
        if (currentLexeme != NULL)
            errorProcessing(currentLexeme,5,"");
        
    }
    else if (currentLexeme["lexCode"] == "402")
        if(readNextLexeme() == ";")
            block();

    else {
        errorProcessing();
    }
    block()
}

node* signalProgram(string lexemes,map<string,int>* Identifiers){
    node* Tree = newNode("<signal-program>");
    Tree->Nodes.push_back(newNode("<program>"));

    auto firstLexeme = readNextLexeme(lexemes);
    program(firstLexeme,Tree->Nodes[0],Identifiers);

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

        scannerPosition++;
    }

    return nextLexeme
}


void Parser(string lexemes,map<string,int> Identifiers){
    node* Tree = signalProgram(lexemes,&Identifiers);
    printTree(Tree);
}