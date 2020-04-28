#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void Parser(string lexemes, map<int,int>* Attributes, map<string,int>* Reserved_Words, 
            map<string,int>* Identifiers);
map<string,string>* readNextLexeme(string lexemesInit = NULL);
node* signalProgram(string lexemes,map<string,int>* Identifiers);
void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
void procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
void identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
void block(node*Tree,map<string,string> currentLexeme);
void statementsList(node*Tree,map<string,string> currentLexeme);
void errorProcessing(map<string,string> currentLexeme,int error_type,string expectedLexeme="");
struct node;
struct node* newNode(string nonTerminal = "");
void errorProcessing(map<string,string> currentLexeme,int error_type,string expectedLexeme="");
