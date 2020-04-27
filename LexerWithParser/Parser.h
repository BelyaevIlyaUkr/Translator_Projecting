#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void Parser(string lexemes, map<int,int>* Attributes, map<string,int>* Reserved_Words, 
            map<string,int>* Identifiers);