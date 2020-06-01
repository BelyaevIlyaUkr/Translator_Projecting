#pragma once
#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
//#include "Front.h"
#include "Parser.h"

void declaration_attributes_actions(int*state,string currentLexeme,string asm_code_below,string* asm_code);
void errorProcessing(bool printErrors,bool base_absent,bool base_absent_error,string new_lexem_row,string new_lexem_column,FILE*f2=NULL,string currentLexeme="",int error_type=0,string expectedLexeme="");
bool identifier_identity(int* state,string identifier_value,string*firstUsedIdentifier = NULL,string identifiers_type="",string*few_identifiers_declaration=NULL);
string tree_travel(struct node* syntax_tree,map<string,int>* Identifiers,int* state);
void Code_Generator(struct node* syntax_tree,map<string,int>* Identifiers,FILE*f2);

#endif