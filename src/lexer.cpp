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

    //std::cout << c << std::endl;

    if (isspace(c)){

        // pos++;
        // return;

    }

    if (isalpha(c) || c == '_'){

        std::string id = lexer::parse_identifier(pos);
        return token(token::TOKEN_IDENTIFIER, id);
    }

    if (isPunctuator(c)){

        pos++;
        return(token(token::TOKEN_PUNCTUATOR, std::string(1, c)));

    }

    if (isLiteral(c)){



    }

    //return(token(token::TOKEN_PUNCTUATOR, std::string(1, c));
    pos++;
    return token(token::TOKEN_EOF, "EOF");

}

lexer::~lexer(){

    delete(charVec);

}

bool lexer::isPunctuator(char c){

    std::string punctuators = "()";
    if(punctuators.find(c) != std::string::npos)
        return true;
    return false;

}

bool lexer::isLiteral(char c){}

std::string lexer::parse_identifier(int &pos){

    int offset = 1;
    std::string extendedCharacter = "+-.*/<=>!?:$%_&~^";

    std::string id;

    id += charVec->at(pos); 

    char current_char = charVec->at(pos + offset);

    while (isalnum(current_char) || current_char == '_' || extendedCharacter.find(current_char) != std::string::npos){

        current_char = charVec->at(pos + offset);
        id += current_char;
        offset++;
 
    }

    pos += offset;

    //std::cout << id << std::endl;

    return id;

}

std::string lexer::parse_number(int pos){}

std::string lexer::parse_literal(int pos){}