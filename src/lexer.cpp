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

    int state = 0;

    //std::cout << c << std::endl;

    while(c != EOF){

        switch (state){

            case 0: {

                if (isIdentifier(c)) {state = 1; break;}
                else if (isspace(c)) {pos++; break;}
                else if (c == '(') {pos++; return token(token::TOKEN_L_PAREN, std::string(1, c));}
                else if (c == ')') {pos++; return token(token::TOKEN_R_PAREN, std::string(1, c));}
                else if (c == '"') {state = 2; break;}

                break;
            }
            // Identifier State
            case 1: {

                std::string id = parseIdentifier(pos);
                return token(token::TOKEN_IDENTIFIER, id);
                break;
            }
            // String Case
            case 2:{
                
                std::string str = parseString(pos);
                return token(token::TOKEN_STRING, str);
                break;

            }
        }
    
    }
    //std::cout << c << std::endl;

    // if (isspace(c)){

    //     // pos++;
    //     // return;

    // }

    // if (isPunctuator(c)){

    //     pos++;
    //     return(token(token::TOKEN_PUNCTUATOR, std::string(1, c)));

    // }

    // if (isLiteral(c)){



    // }

    // //return(token(token::TOKEN_PUNCTUATOR, std::string(1, c));
    // pos++;
    return token(token::TOKEN_EOF, "EOF");

}

lexer::~lexer(){

    delete(charVec);

}
 
bool lexer::isIdentifier(char c){
    const std::string specialInitial = "!$%&*/:<=>?^_~";
    return isalpha(c) || specialInitial.find(c) != std::string::npos;

}

bool lexer::isPunctuator(char c){

    std::string punctuators = "()";
    if(punctuators.find(c) != std::string::npos)
        return true;
    return false;

}

bool lexer::isDelimeter(char c){}

bool lexer::isLiteral(char c){}

std::string lexer::parseString(int &pos){

    pos++;
    std::string str;

    char current_char = charVec->at(pos);

    while (current_char != EOF && current_char != '\"'){

        if (current_char == '\\') {str += parseEscapeSequence(pos); continue;}

        else {str += current_char;}

        pos++;

        current_char = charVec->at(pos);

    }

    return str;

}

std::string lexer::parseEscapeSequence(int &pos){

    char current_char = charVec->at(pos);
    char next_char = charVec->at(pos + 1);

    const std::string escapeChars = "abfnrtv'\"\\\?";
    
    // Common Escape Sequence
    if (escapeChars.find(next_char) != std::string::npos){

        return std::string(1, current_char) + std::string(1, next_char); 

    }

    // ASCII octal notations
    else if (isdigit(next_char)){

        return "";

    }

    // ASCII or UNICODE in hexadecimal notation
    else if (next_char == 'x'){

        return "";

    }
}

std::string lexer::parseIdentifier(int &pos){

    int offset = 0;
    const std::string extendedCharacter = "+-.*/<=>!?:$%_&~^";

    std::string id;

    char current_char = charVec->at(pos);

    //id += current_char; 

    //char current_char = charVec->at(pos + offset);

    while (isalnum(current_char) || current_char == '_' || extendedCharacter.find(current_char) != std::string::npos){

        id += current_char;
        offset++;
        current_char = charVec->at(pos + offset);
 
    }

    pos += offset + 1;

    //std::cout << id << std::endl;

    return id;

}

std::string lexer::parseNumber(int &pos){}

std::string lexer::parseLiteral(int &pos){}