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


bool attribute(node*Tree,map<string,string>* currentLexeme){
    Tree->Nodes.push_back(newNode("<attribute>"));
    if((*currentLexeme)["lexCode"] == "405" || (*currentLexeme)["lexCode"] == "406"
    ||(*currentLexeme)["lexCode"] == "407" || (*currentLexeme)["lexCode"] == "408"
    || (*currentLexeme)["lexCode"] == "409" || (*currentLexeme)["lexCode"] == "410") {

        (Tree->Nodes[Tree->Nodes.size()-1])->Nodes.push_back(newNode());
        ((Tree->Nodes[Tree->Nodes.size()-1])->Nodes[0])->terminal = *currentLexeme;
        return true;
    }
    else{
        return false;
    }
}


bool attributesList(node*Tree,map<string,string>* currentLexeme){
    *currentLexeme = readNextLexeme();

    Tree->Nodes.push_back(newNode("<attributes-list>"));
    if ((*currentLexeme)["lexCode"] == "59"){     // ;
        Tree->Nodes[Tree->Nodes.size()-1]->Nodes.push_back(newNode("<empty>")); 
        return true;
    }

    if(!attribute(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme)){
        return false;
    }

    return attributesList(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme);
}


bool variableIdentifier(node*Tree,map<string,string>currentLexeme,map<string,int>*Identifiers){
    Tree->Nodes.push_back(newNode("<variable-identifier>"));
    return identifier(currentLexeme,Tree->Nodes[Tree->Nodes.size()-1],Identifiers);
}


bool identifiersList(node*Tree,map<string,string>& currentLexeme,map<string,int>* Identifiers){
    if( currentLexeme["lexCode"] == "44" ) {     // ,
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
    }
    else if(currentLexeme["lexCode"] == "58"){   // :
        Tree->Nodes.push_back(newNode("<empty>"));
        return true;
    }
    else{
        errorProcessing(false,currentLexeme,7,", or : ");
        return false;
    }

    currentLexeme = readNextLexeme();

    
    if (!variableIdentifier(Tree,currentLexeme,Identifiers)){
        return false;
    }


    currentLexeme = readNextLexeme();
    Tree->Nodes.push_back(newNode("<identifiers-list>"));
    return identifiersList(Tree->Nodes[2],currentLexeme,Identifiers);
}


bool declaration(node*Tree,map<string,string> currentLexeme,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<declaration>"));

    if (!variableIdentifier(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme,Identifiers))
        return false;

    currentLexeme = readNextLexeme();
    (Tree->Nodes[0])->Nodes.push_back(newNode("<identifiers-list>"));
    if (!identifiersList((Tree->Nodes[0])->Nodes[1],currentLexeme,Identifiers))
        return false;


    (Tree->Nodes[0])->Nodes.push_back(newNode());   
    ((Tree->Nodes[0])->Nodes[Tree->Nodes[0]->Nodes.size()-1])->terminal = currentLexeme;     // :


    currentLexeme = readNextLexeme();
    if(!attribute(Tree->Nodes[0],&currentLexeme)){
        errorProcessing(false,currentLexeme,9);
        return false;
    }
    if(!attributesList(Tree->Nodes[0],&currentLexeme)){ //empty list of attribute(begins from second attribute) are being translated correctly
        errorProcessing(false,currentLexeme,8);
        return false;
    }
    (Tree->Nodes[0])->Nodes.push_back(newNode());
    ((Tree->Nodes[0])->Nodes[(Tree->Nodes[0])->Nodes.size()-1])->terminal = currentLexeme;
    
    return true;
}


bool declarationsList(node*Tree,map<string,string>* currentLexeme,map<string,int>* Identifiers){
    if ((*currentLexeme)["lexCode"] == "41"){
        Tree->Nodes.push_back(newNode("<empty>"));
        return true;
    }
    if (!declaration(Tree,*currentLexeme,Identifiers))
        return false;
    *currentLexeme = readNextLexeme();
    Tree->Nodes.push_back(newNode("<declarations-list>"));
    return declarationsList(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme,Identifiers);
}


bool parametersList(node*Tree,map<string,string>& currentLexeme,map<string,int>* Identifiers){
    if( currentLexeme["lexCode"] == "40" ) {      //  (
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
    }
    else if (currentLexeme["lexCode"] == "59"){   //  ;
        Tree->Nodes.push_back(newNode("<empty>"));
        return true;
    }
    else{
        errorProcessing(false,currentLexeme,7," ( or ; ");
        return false;
    }

    currentLexeme = readNextLexeme();
    Tree->Nodes.push_back(newNode("<declarations-list>"));
    if (!declarationsList(Tree->Nodes[Tree->Nodes.size()-1],&currentLexeme,Identifiers))
        return false;

    if( currentLexeme["lexCode"] == "41" ) {       //   )
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
    }
    else{
        errorProcessing(false,currentLexeme,7,")");
        return false;
    }

    return true;
}


bool statementsList(node*Tree,map<string,string> currentLexeme){
    if(currentLexeme["lexCode"] == "404") {
         Tree->Nodes.push_back(newNode("<empty>"));
         return true;
    }
    else{
        errorProcessing(false,currentLexeme,4,"END");
        return false;
    }
    
}


bool block(node*Tree,map<string,string> currentLexeme){
    if (currentLexeme["lexCode"] == "403") {
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[0])->terminal = currentLexeme;
    }
    else{
        errorProcessing(false,currentLexeme,4,"BEGIN");
        return false;
    }


    currentLexeme = readNextLexeme();
    Tree->Nodes.push_back(newNode("<statements-list>"));
    if (!statementsList(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme))
        return false;


    if((Tree->Nodes[Tree->Nodes.size()-1])->Nodes.size() != 0){
        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
    }
    else {
        errorProcessing(false,currentLexeme,4,"END");
        return false;
    }


    return true;
}


bool identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<identifier>"));
    auto itr = Identifiers->find(currentLexeme["lexemValue"]);
    if( itr == Identifiers->end() ) {
        errorProcessing(false,currentLexeme,6);
        return false;
    }
    else{
        (Tree->Nodes[0])->Nodes.push_back(newNode());
        ((Tree->Nodes[0])->Nodes[0])->terminal = currentLexeme;
        return true;
    }
}


bool procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    Tree->Nodes.push_back(newNode("<procedure-identifier>"));
    return identifier(currentLexeme,Tree->Nodes[Tree->Nodes.size()-1],Identifiers);
}


void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers){
    if (currentLexeme["lexCode"] == "401"){   // PROGRAM

        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;

        if (!procedureIdentifier(readNextLexeme(),Tree,Identifiers)){
            return;
        }

        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "59" ) {   // ;

            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
        }
        else{
            errorProcessing(false,currentLexeme,7,";");
            return;
        }

        currentLexeme = readNextLexeme();
        Tree->Nodes.push_back(newNode("<block>"));
        if (!block(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme))
            return;

        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "46" ) {    // .
            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
        }
        else{
            errorProcessing(false,currentLexeme,7,".");
            return;
        }
        
        currentLexeme = readNextLexeme();
        if (currentLexeme.size() != 0)
            errorProcessing(false,currentLexeme,5);
        
    }
    else if (currentLexeme["lexCode"] == "402"){   //PROCEDURE

        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;


        if (!procedureIdentifier(readNextLexeme(),Tree,Identifiers)){
            return;
        }


        currentLexeme = readNextLexeme();
        Tree->Nodes.push_back(newNode("<parameters-list>"));
        if(!parametersList(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme,Identifiers)){
            return;
        }

        if (currentLexeme["lexCode"] == "41"){   // )
            currentLexeme = readNextLexeme();
            if( currentLexeme["lexCode"] != "59" ) {
                errorProcessing(false,currentLexeme,7,";");
                return;
            }
        }

        Tree->Nodes.push_back(newNode());
        (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;


        currentLexeme = readNextLexeme();
        Tree->Nodes.push_back(newNode("<block>"));
        if (!block(Tree->Nodes[Tree->Nodes.size()-1],currentLexeme))
            return;


        currentLexeme = readNextLexeme();
        if( currentLexeme["lexCode"] == "59" ) {    // ;
            Tree->Nodes.push_back(newNode());
            (Tree->Nodes[Tree->Nodes.size()-1])->terminal = currentLexeme;
        }
        else{
            errorProcessing(false,currentLexeme,7,";");
            return;
        }
        

        currentLexeme = readNextLexeme();
        if (currentLexeme.size() != 0)
            errorProcessing(false,currentLexeme,5);
    }
    else {
        errorProcessing(false,currentLexeme,-1," PROGRAM or PROCEDURE ");
        return;
    }
}


node* signalProgram(string lexemes,map<string,int>* Identifiers){
    node* Tree = newNode("<signal-program>");
    Tree->Nodes.push_back(newNode("<program>"));

    auto firstLexeme = readNextLexeme(lexemes);
    program(firstLexeme,Tree->Nodes[Tree->Nodes.size()-1],Identifiers);

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