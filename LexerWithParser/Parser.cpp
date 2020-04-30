#include "Parser.h"

struct node* newNode(string nonTerminal) {  
    node* node = new struct node; 
    node->nonTerminal = nonTerminal;
    return(node); 
}


void errorProcessing(bool printErrors,map<string,string> currentLexeme={},int error_type=0,string expectedLexeme=""){
    static ostringstream errorMessages;

    if (printErrors)
        errors_out(&errorMessages);
    else
        error_collect(&errorMessages,error_type,currentLexeme,expectedLexeme);

}


void statementsList(node*Tree,map<string,string> currentLexeme){
    if(currentLexeme["lexCode"] == "404") {
            Tree->Nodes.push_back(newNode("<empty>"));
    }
    else{
        errorProcessing(false,currentLexeme,5);
        errorProcessing(false,currentLexeme,4,"END");
    }
    
}


void block(node*Tree,map<string,string> currentLexeme){
     if (currentLexeme["lexCode"] == "403") {
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[0])->terminal = currentLexeme;
    }
    else
        errorProcessing(false,currentLexeme,4,"BEGIN");


    currentLexeme = readNextLexeme();
    Tree->Nodes.push_back(newNode("<statements-list>"));
    statementsList(Tree->Nodes[1],currentLexeme);


    if((Tree->Nodes[1])->Nodes.size() != 0){
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[2])->terminal = currentLexeme;
    }
}


void identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<identifier>"));
    auto itr = Identifiers->find(currentLexeme["lexemValue"]);
    if( itr == Identifiers->end() ) {
        errorProcessing(false,currentLexeme,6);
    }
    else{
        (Tree->Nodes[0])->Nodes.push_back(newNode());
        ((Tree->Nodes[0])->Nodes[0])->terminal = currentLexeme;
    }
}

void procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<procedure-identifier>"));
    identifier(currentLexeme,Tree->Nodes[1],Identifiers);
}

void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    if (currentLexeme["lexCode"] == "401"){

        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[0])->terminal = currentLexeme;

        procedureIdentifier(readNextLexeme(),Tree,Identifiers);

        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "59" ) {

            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[2])->terminal = currentLexeme;
        }
        else
            errorProcessing(false,currentLexeme,7,";");

        currentLexeme = readNextLexeme();
        Tree->Nodes.push_back(newNode("<block>"));
        block(Tree->Nodes[3],currentLexeme);

        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "46" ) {
            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[4])->terminal = currentLexeme;
        }
        else
            errorProcessing(false,currentLexeme,7,".");
        
        currentLexeme = readNextLexeme();
        if (currentLexeme.size() != 0)
            errorProcessing(false,currentLexeme,5);
        
    }
    else if (currentLexeme["lexCode"] == "402"){}
}

node* signalProgram(string lexemes,map<string,int>* Identifiers){
    node* Tree = newNode("<signal-program>");
    Tree->Nodes.push_back(newNode("<program>"));

    auto firstLexeme = readNextLexeme(lexemes);
    program(firstLexeme,Tree->Nodes[0],Identifiers);

    return Tree;
}

map<string,string> readNextLexeme(string lexemesInit){
    static string lexemes;

    if (lexemesInit != "")
        lexemes = lexemesInit;
   
    map<string,string> nextLexeme;

    static int scannerPosition = 0;

    while(lexemes[scannerPosition]!= '\n' && lexemes[scannerPosition]!= '\0'){
        string strTmp="";
        if (lexemes[scannerPosition] == '\t'){
            scannerPosition++;
        }
        do{
            strTmp += lexemes[scannerPosition];
            scannerPosition++;
        }while(lexemes[scannerPosition]!= '\n' && lexemes[scannerPosition]!= '\t');
        switch(nextLexeme.size()) {
            case 0:
                nextLexeme.insert({"primary_lexem_row",strTmp});
                break;
            case 1:
                nextLexeme.insert({"primary_lexem_column",strTmp});
                break;
            case 2:
                nextLexeme.insert({"lexCode",strTmp});
                break;
            case 3:
                nextLexeme.insert({"lexemValue",strTmp});
        }
    }

    scannerPosition++;

    return nextLexeme;
}


node* Parser(string lexemes,map<string,int> Identifiers){
    node* Tree = signalProgram(lexemes,&Identifiers);
    printTree(Tree);
    errorProcessing(true);
    return Tree;
}