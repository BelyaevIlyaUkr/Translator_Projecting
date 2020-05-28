#pragma once
#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
struct node{
    map<string,string> terminal;
    string nonTerminal; 
    vector<node*> Nodes;
};
#include "Front.h"


struct node* newNode(string nonTerminal="");
map<string,string> readNextLexeme(string lexemesInit="");
node* Parser(string lexemes,map<string,int> Identifiers,FILE*f2);
node* signalProgram(string lexemes,map<string,int>* Identifiers);
void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool block(node*Tree,map<string,string> currentLexeme);
bool statementsList(node*Tree,map<string,string> currentLexeme);
void errorProcessing(bool printErrors,FILE* f2,map<string,string> currentLexeme,int error_type,string expectedLexeme);
bool variableIdentifier(node*Tree,map<string,string>currentLexeme,map<string,int>*Identifiers);
bool identifiersList(node*Tree,map<string,string>& currentLexeme,map<string,int>* Identifiers);
bool attribute(node*Tree,map<string,string>* currentLexeme);
bool attributesList(node*Tree,map<string,string>* currentLexeme);
bool parametersList(node*Tree,map<string,string>& currentLexeme,map<string,int>* Identifiers);
bool declaration(node*Tree,map<string,string> currentLexeme,map<string,int>* Identifiers);
bool declarationsList(node*Tree,map<string,string>* currentLexeme,map<string,int>* Identifiers);


#endif
