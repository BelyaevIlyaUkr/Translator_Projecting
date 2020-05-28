#pragma once
#ifndef _LEXER_H
#define _LEXER_H

#include "Parser.h"
#include "Front.h"
#include <iostream>
#include <map>
#include <string>
#include <sstream>

using namespace std;

struct TSymbol;

TSymbol Gets(FILE*f, map<int,int>* Attributes, int* current_column, int* current_row);
void FillAttributes(map<int,int>* Attributes);
void FillReservedWords(map<string,int>* Reserved_Words);
int searchReservedWord(string Word, map<string,int>* Reserved_Words);
int searchIdentifier(string Ident,map<string,int>* Identifiers);
int AddToIdentifierTable(string Ident,map<string,int>* Identifiers);
string Lexer(FILE*f,map<int,int>* Attributes,map<string,int>* Reserved_Words,map<string,
            int>* Identifiers);

#endif