#include "lexer.h"
#include <fstream>

#include <iostream>

lexer::lexer(const char *file){

    std::ifstream src_code(file);
    
    if (src_code.is_open()){

        char current_char;

        while (src_code){

            current_char = src_code.get();

            charVec->push_back(current_char);

            if (current_char == EOF) break;

        }

    }

    src_code.close();

}

token lexer::advance(){

    return token(token::TOKEN_EOF, "EOF");

}

lexer::~lexer(){

    delete(charVec);

}