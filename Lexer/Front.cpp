#include "Front.h"

void lexemes_out(string lexemes) {
    cout<<lexemes;
}


void error_collect(ostringstream* errorMessages,int error_type,int current_row,int current_column,int lexCode) {
    if (error_type == 1){
        *errorMessages<<"Lexer:Error:(line:" << current_row << ",column:"<< current_column
        << "):  *) expected but end of file found\n";
    }
    else if (error_type == 2){
            *errorMessages << "Lexer:Error:(line:"<<current_row<< ",column:"
            << current_column << "):Illegal symbol " << (char)lexCode <<"\n\n";
    }
    else if (error_type == 3){
            *errorMessages << "Lexer:Error:(line:"<<current_row<< ",column:"
            << current_column << "):Illegal digits using" <<"\n\n";
    }

}


void errors_out(ostringstream* errorMessages){
    cout<<errorMessages->str();
}


void identifiers_out(map<string,int>* Identifiers){
    cout<<"Identifiers_table\n";
    if (Identifiers->size() == 0){
        cout<<"Empty\n";
        return;
    }
    map<string,int>::iterator it;
    for(it = Identifiers->begin(); it != Identifiers->end(); it++){
        cout<< it->first <<"\t" <<it->second << "\n";
    }
    cout<<"\n";
}

void Empty_File_Msg(){
    cout<<"File is empty\n";
}

void Attributes_out(map<int,int>* Attributes){
    cout<< "Table with ASCII and their attributes\n";

    map<int,int>::iterator it;
    for(it = Attributes->begin(); it!=Attributes->end(); it++){
        cout << (char)it->first << "\t" << it->second << "\n";
    }
    cout<<"All other characters from ASCII don't match conditions and are"
        << "perceived as not correct(except small latin literatures)\n\n";
}

void Reserved_Words_out(map<string,int>* Reserved_Words){
    cout<< "Table with Reserved Words\n";

    map<string,int>::iterator it;
    for(it = Reserved_Words->begin(); it!=Reserved_Words->end(); it++){
        cout << it->first << " " << it->second << "\n";
    }
}
