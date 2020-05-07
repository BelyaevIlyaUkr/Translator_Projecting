#include "Lexer.h"
#include "Parser.h"
int main(int argc,char** argv) {
    map<int,int> Attributes;
    map<string,int> Reserved_Words;
    map<string,int> Identifiers;
    FILE*f,*f2;

    printf("\33c\e[3J");

    if((f=fopen((string(argv[1])+"input.sig").c_str(),"rt"))==NULL) {
        cout << "Can't open file\n";
        exit(1);
    }
    f2=fopen((string(argv[1])+"generated.txt").c_str(),"w+");

    string lexemes = Lexer(f,&Attributes,&Reserved_Words,&Identifiers);

    struct node*syntax_tree = Parser(lexemes,Identifiers,f2);

    fclose(f2);
    fclose(f);
}