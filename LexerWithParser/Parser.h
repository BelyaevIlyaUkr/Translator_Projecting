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
node* Parser(string lexemes,map<string,int> Identifiers);
node* signalProgram(string lexemes,map<string,int>* Identifiers);
void program(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool procedureIdentifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool identifier(map<string,string> currentLexeme,node* Tree,map<string,int>* Identifiers);
bool block(node*Tree,map<string,string> currentLexeme);
bool statementsList(node*Tree,map<string,string> currentLexeme);
void errorProcessing(map<string,string> currentLexeme,int error_type,string expectedLexeme);

#endif
