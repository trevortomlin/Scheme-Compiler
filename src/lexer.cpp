#include "lexer.h"
#include <fstream>
#include <iterator>
#include <cassert>
#include <string>

#include <iostream>

lexer::lexer(const char *file){

    std::ifstream src_code(file);

    assert(src_code.is_open());

    // Doesn't work with whitspace currently.
    // std::copy(std::istream_iterator<char>(src_code), std::istream_iterator<char>(),
    // std::back_inserter(*charVec));
    
    char current_char;

    while (src_code){

        current_char = src_code.get();

         charVec->push_back(current_char);

        if (current_char == EOF) break;

    }

    src_code.close();

}

token lexer::advance(){

    c = charVec->at(pos);

    if (isspace(c)){

        pos++;
        return;

    }

    if (isalpha(c)){

    

    }

    if (isPunctuator(c)){



    }

    if (isLiteral(c)){



    }

    //return(token(token::TOKEN_PUNCTUATOR, std::string(1, c));
    return token(token::TOKEN_EOF, "EOF");

}

lexer::~lexer(){

    delete(charVec);

}

bool lexer::isPunctuator(char c){}

bool lexer::isLiteral(char c){}

std::string lexer::parse_identifier(int pos){}

std::string lexer::parse_number(int pos){}

std::string lexer::parse_literal(int pos){}