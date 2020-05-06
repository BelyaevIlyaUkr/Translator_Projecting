#pragma once
#ifndef _FRONT_H
#define _FRONT_H

#include "Parser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;


void lexemes_out(string lexemes);
void error_collect(ostringstream* errorMessages,int error_type,
                    int current_row,int current_column,int lexCode=-1);
void errors_out(ostringstream* errorMessages);
void identifiers_out(map<string,int>* Identifiers);
void Empty_File_Msg();
void Attributes_out(map<int,int>* Attributes);
void Reserved_Words_out(map<string,int>* Reserved_Words);
void printTree(node*Tree,int dotNumber = 0);
void error_collect(ostringstream* errorMessages,int error_type,map<string,string> currentLexeme,string expectedLexeme);

#endif