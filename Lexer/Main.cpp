#include "Lexer.h"
int main() {
    map<int,int> Attributes;
    map<string,int> Reserved_Words;
    map<string,int> Identifiers;
    FILE*f;
    if((f=fopen("test-false1.txt", "rt"))==NULL) {
        cout << "Cannot open file\n";
        exit(1);
    }
    string lexemes = Lexer(f,&Attributes,&Reserved_Words,&Identifiers);
    fclose(f);
}