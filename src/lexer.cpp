#include "lexer.h"
#include <fstream>
#include <iterator>
#include <cassert>
#include <string>

#include <iostream>

lexer::lexer(const char *file){

    std::ifstream src_code(file);

    assert(src_code.is_open());
    
    char current_char;

    while (src_code){

        current_char = src_code.get();

        charVec->push_back(current_char);

        if (current_char == EOF) break;

    }

    src_code.close();

}

token lexer::advance(){

    int state = 0;

    while(c != EOF){

        c = charVec->at(pos);

        switch (state){

            case 0: {

                if (isIdentifier(c)) {state = 1; break;}
                else if (isspace(c)) {pos++; if (c=='\n') { line++; charNum=0; } break;}
                else if (isdigit(c)){state = 4; break;}
                else if (c == '.' || c == '+' || c == '-'){state = 6; break;}
                else if (c == ';') {skipComment(pos); break;}
                else if (c == '"') {state = 2; break;}
                else if (c == '#') {state = 3; break;}
                else if (c == '(') {pos++; charNum++; return token(token::TOKEN_L_PAREN, std::string(1, c), Position(charNum, line));}
                else if (c == ')') {pos++; charNum++; return token(token::TOKEN_R_PAREN, std::string(1, c), Position(charNum, line));}
                else if (c == '\'') {pos++; charNum++; return token(token::TOKEN_SINGLE_QUOTE, std::string(1, c), Position(charNum, line));}
                else if (c == ',') {state = 5; break;}
                else if (c == '`') {pos++; charNum++; return token(token::TOKEN_BACK_QUOTE, std::string(1, c), Position(charNum, line));}

                break;
            }
            // Identifier State
            case 1: {

                std::string id = parseIdentifier(pos);
                charNum++;
                return token(token::TOKEN_IDENTIFIER, id, Position(charNum, line));
                break;
            }
            // String State
            case 2: {
                
                std::string str = parseString(pos);
                charNum++;
                return token(token::TOKEN_STRING, str, Position(charNum, line));
                break;

            }

            // Constant State
            case 3: {

                char next_char = charVec->at(pos + 1);

                switch (next_char){

                    case 't':
                    case 'f': pos+=2; charNum++; return token(token::TOKEN_BOOLEAN, "#" + std::string(1, next_char), Position(charNum, line)); break;

                    case '\\': pos += 2; charNum++; return token(token::TOKEN_CHARACTER, parseCharacter(pos), Position(charNum, line)); break;
                    
                    case '(': pos += 2; charNum++; return token(token::TOKEN_VECTOR_CONSTANT, "#" + std::string(1, next_char), Position(charNum, line)); break;

                    // Radix and exactness markers
                    case 'e':
                    case 'i':
                    case 'b':
                    case 'o':
                    case 'd':
                    case 'x': state = 4; break;
                }

                break;
 
            }

            // Number State
            case 4: {

                std::string prefix = "";

                char current_char, next_char;

                do{

                    current_char = charVec->at(pos);
                    next_char = charVec->at(pos+1);

                    switch (next_char)
                    {
                    case 'i':
                    case 'e':
                        pos += 2;
                        prefix += std::string(1, c) + std::string(1, next_char);
                        break;

                    case 'b':
                    case 'o':
                    case 'd':
                    case 'x':
                        pos += 2;
                        prefix += std::string(1, c) + std::string(1, next_char);
                        break;    
                    
                    }

                } while (current_char == '#');

                charNum++;
                return token(token::TOKEN_NUMBER, prefix + parseNumber(pos), Position(charNum, line));

            }

            // Comma State
            case 5: {
            
                char next_char = charVec->at(pos+1);

                if (next_char == '@'){pos +=2; charNum++; return token(token::TOKEN_COMMA_AT, std::string(1, c) + std::string(1, next_char), Position(charNum, line));}
                else {pos +=2; charNum++; return token(token::TOKEN_COMMA, std::string(1, c), Position(charNum, line));}
            
            }

            // Case Delimited op
            case 6: {

                char next_char = charVec->at(pos + 1);

                if (c == '.'){

                    if (isDelimiter(next_char) && next_char != '.'){pos++; return token(token::TOKEN_PERIOD, std::string(1, c), Position(charNum, line));}
                    else if (charVec->at(pos + 2) == '.'){
                        pos+=3;
                        charNum++;
                        return token(token::TOKEN_IDENTIFIER, "...", Position(charNum, line));     

                    }

                }

                if (next_char == ' ') {pos++; charNum++; return token(token::TOKEN_IDENTIFIER, std::string(1, c), Position(charNum, line));}

                else {state = 4;}

                break;

            }
        }
    
    }

    charNum++;
    return token(token::TOKEN_EOF, "EOF", Position(charNum, line));

}

void lexer::skipComment(int &pos){

    while (charVec->at(pos) != '\n' && charVec->at(pos) != EOF){

        pos++;

    }

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

bool lexer::isDelimiter(char c){

    std::string delimiters = " \n()\";";
    return (delimiters.find(c) != std::string::npos);

}

//bool lexer::isLiteral(char c){}

std::string lexer::parseCharacter(int &pos){

    std::string character = parseIdentifier(pos);

    if (character.size() == 1){return character;}

    else if (character == "space"){return " ";}

    else if (character == "newline"){return "\\n";}

    return "";

}

std::string lexer::parseString(int &pos){

    pos++;
    std::string str;

    char current_char;

    do {

        current_char = charVec->at(pos);

        if (current_char == '\\') {str += parseEscapeSequence(pos); continue;}

        else {str += current_char;}

        pos++;

        current_char = charVec->at(pos);

    } while (current_char != EOF && current_char != '\"');

    pos++;

    return str;

}

std::string lexer::parseEscapeSequence(int &pos){

    char current_char = charVec->at(pos);
    char next_char = charVec->at(pos + 1);

    const std::string escapeChars = "abfnrtv'\"\\\?";
    
    // Common Escape Sequence
    if (escapeChars.find(next_char) != std::string::npos){

        pos += 2;
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

    return "";
}

std::string lexer::parseIdentifier(int &pos){

    int offset = 0;
    const std::string extendedCharacter = "_+-.*/<=>!?:$%_&~^";

    std::string id;

    char current_char = charVec->at(pos);

    while (isalnum(current_char) || extendedCharacter.find(current_char) != std::string::npos){

        id += current_char;
        offset++;
        current_char = charVec->at(pos + offset);
 
    }

    pos += offset;

    return id;

}

std::string lexer::parseNumber(int &pos){

    char current_char = charVec->at(pos);

    std::string number = "";

    while (!isDelimiter(current_char) && current_char != EOF){

        number += current_char;
        pos++;
        current_char = charVec->at(pos);

    }

    return number;

}