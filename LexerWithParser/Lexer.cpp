#include "Lexer.h"
#include "Front.h"

struct TSymbol {
    char value;
    int attr;
};


TSymbol Gets(FILE*f, map<int,int>* Attributes, int* current_column, int* current_row) {
    TSymbol symbol;
    map <int,int> :: iterator it;
    symbol.value = toupper(fgetc(f));
    (*current_column)++;
    if (symbol.value == '\n') {
        (*current_row)++;
        *current_column = 0;
    }

    it = Attributes->find(symbol.value);
    if (it == Attributes->end())
        symbol.attr = -1;
    else
        symbol.attr = it->second;
    
    return symbol;
}


void FillAttributes(map<int,int>* Attributes){

    for(int i = 8;i < 14; i++)
        Attributes -> insert({i, 0});

    Attributes -> insert({32, 0});

    for(int i = 65;i < 91; i++)
        Attributes -> insert({i, 1});

    for(int i = 48;i < 58; i++){
        Attributes -> insert({i, 2});
    }

    Attributes -> insert({40 , 3});

    int separators[6] = {41, 42, 44, 46, 58, 59};

    for(int i:separators)
        Attributes -> insert({i, 4});
}


void FillReservedWords(map<string,int>* Reserved_Words) {
    string reserved_wrds[10] = {"PROGRAM","PROCEDURE","BEGIN","END","SIGNAL","COMPLEX","INTEGER",
                                "FLOAT","EXT","BLOCKFLOAT"};
    for(int i = 0 ;i < 10 ;i++) {
        Reserved_Words -> insert({reserved_wrds[i],401 + i});
    }
}


int searchReservedWord(string Word, map<string,int>* Reserved_Words) {
    auto itr = Reserved_Words->find(Word);
    if( itr == Reserved_Words->end() ) {
        return -1;
    }
    return itr->second;
}

int searchIdentifier(string Ident,map<string,int>* Identifiers){
    for(auto itr = Identifiers->begin() ; itr != Identifiers->end() ; itr++) {
        if (itr->first == Ident)
            return itr -> second;
    }
    return -1;
}


int AddToIdentifierTable(string Ident,map<string,int>* Identifiers) {

    Identifiers->insert({Ident,1001+Identifiers->size()});
    return (Identifiers->find(Ident))->second;
}


string Lexer(FILE* f,map<int,int>* Attributes,map<string,int>* Reserved_Words,
            map<string,int>* Identifiers)
{
    string lexemes = "";

    FillAttributes(Attributes);
    FillReservedWords(Reserved_Words);

    int current_row = 1,primary_lexem_row = 1;
    int current_column = 0,primary_lexem_column = 0;

    fseek(f,0,SEEK_END);
    if(ftell(f) == 0){
        Empty_File_Msg();
        return lexemes;
    }
    fseek(f,0,SEEK_SET);

    ostringstream errorMessage;

    TSymbol symbol = Gets(f, Attributes, &current_column, &current_row);
    do {
        string buf = "";
        int lexCode = 0;
        bool SuppressOutput = false;
        switch(symbol.attr) {
        case 0://whitespaces
            while(!feof(f)) {
                symbol = Gets(f,Attributes,&current_column,&current_row);
                if(symbol.attr != 0)
                    break; 
            } 
            SuppressOutput = true;
            break;
        case 1://identifiers and reserved words(begin from letter)
            primary_lexem_row = current_row;
            primary_lexem_column = current_column;
            while (!feof(f) && (symbol.attr == 2 || symbol.attr == 1)) {
                buf = buf + symbol.value; 
                symbol = Gets(f,Attributes,&current_column, &current_row);
            }
            lexCode = searchReservedWord(buf,Reserved_Words);
            if (lexCode == -1)
                lexCode = searchIdentifier(buf,Identifiers);
                if (lexCode == -1)
                    lexCode = AddToIdentifierTable(buf,Identifiers);
            break;
        case 3://comments(begin from open brace)
            primary_lexem_row = current_row;
            primary_lexem_column = current_column;
            if (feof(f)) {
                lexCode = 40; 
                buf = '(';
            }
            else {
                symbol = Gets(f,Attributes,&current_column, &current_row); 
                if (symbol.value == '*') {
                    if (feof(f)) {
                        error_collect(&errorMessage,1,current_row,current_column);
                        SuppressOutput = true;
                    }
                    else {
                        symbol = Gets(f,Attributes,&current_column,&current_row); 
                        do {
                            while (!feof(f) && (symbol.value != '*')) {
                                symbol = Gets(f,Attributes,&current_column,&current_row);
                            }
                            if (feof(f)){
                                error_collect(&errorMessage,1,current_row,current_column);
                                symbol.value = '+';
                                SuppressOutput = true;
                                break;
                            }
                            else {
                                symbol = Gets(f,Attributes,&current_column,&current_row);
                            }
                        } while (symbol.value != ')'); 
                        if (symbol.value == ')') {
                            SuppressOutput = true;
                        }
                        if (!feof(f)) {
                            symbol = Gets(f,Attributes,&current_column,&current_row);
                        }
                    }
                }
                else {
                    lexCode = 40;
                    buf = '(';
                }
            }
            break;
        case 4://single separators
            primary_lexem_row = current_row;
            primary_lexem_column = current_column;
            lexCode = symbol.value;
            buf = buf + symbol.value; 

            if(!feof(f))
                symbol = Gets(f,Attributes,&current_column,&current_row);
            break;
        default://error case
            lexCode = symbol.value;
            if (symbol.attr == 2) 
                error_collect(&errorMessage,3,current_row,current_column);
            else
                error_collect(&errorMessage,2,current_row,current_column,lexCode);
            symbol = Gets(f,Attributes,&current_column,&current_row);
            SuppressOutput = true;
        }
        if(!SuppressOutput) {
            lexemes = lexemes + to_string(primary_lexem_row) + "\t" + 
            to_string(primary_lexem_column) + "\t" + to_string(lexCode) + "\t" + buf + "\n";
        }
    } while(!feof(f));

    lexemes_out(lexemes);
    errors_out(&errorMessage);
    identifiers_out(Identifiers);

    return lexemes;
}