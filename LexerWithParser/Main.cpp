#include "Lexer.h"
#include "Parser.h"
int main() {
    map<int,int> Attributes;
    map<string,int> Reserved_Words;
    map<string,int> Identifiers;
    FILE*f;

    printf("\33c\e[3J");

    if((f=fopen("test-true1.txt", "rt"))==NULL) {
        cout << "Cannot open file\n";
        exit(1);
    }

    string lexemes = Lexer(f,&Attributes,&Reserved_Words,&Identifiers);

    string syntax_tree = Parser(lexemes,Identifiers);

    fclose(f);
}