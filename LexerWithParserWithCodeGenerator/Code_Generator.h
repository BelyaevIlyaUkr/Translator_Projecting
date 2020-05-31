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

bool identifier_identity(int* state,string identifier_value,string*firstUsedIdentifier = NULL,string identifiers_type="",string*few_identifiers_declaration=NULL);
string tree_travel(struct node* syntax_tree,map<string,int>* Identifiers,int* state);
void Code_Generator(struct node* syntax_tree,map<string,int>* Identifiers,FILE*f2);

#endif