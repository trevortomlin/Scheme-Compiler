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

    //std::cout << c << std::endl;

    int state = 0;

    while(c != EOF){

        c = charVec->at(pos);

        switch (state){

            case 0: {

                if (isIdentifier(c)) {state = 1; break;}
                else if (isspace(c)) {pos++; break;}
                else if (isdigit(c)){state = 4; break;}
                else if (c == '.' || c == '+' || c == '...' || c == '-'){state = 6; break;}
                else if (c == ';') {skipComment(pos); break;}
                else if (c == '"') {state = 2; break;}
                else if (c == '#') {state = 3; break;}
                else if (c == '(') {pos++; return token(token::TOKEN_L_PAREN, std::string(1, c));}
                else if (c == ')') {pos++; return token(token::TOKEN_R_PAREN, std::string(1, c));}
                else if (c == '\'') {pos++; return token(token::TOKEN_SINGLE_QUOTE, std::string(1, c));}
                else if (c == ',') {state = 5; break;}
                //else if (c == '.') {pos++; return token(token::TOKEN_PERIOD, std::string(1, c));}
                else if (c == '`') {pos++; return token(token::TOKEN_BACK_QUOTE, std::string(1, c));}

                break;
            }
            // Identifier State
            case 1: {

                std::string id = parseIdentifier(pos);
                return token(token::TOKEN_IDENTIFIER, id);
                break;
            }
            // String State
            case 2: {
                
                std::string str = parseString(pos);
                return token(token::TOKEN_STRING, str);
                break;

            }

            // Constant State
            case 3: {

                char next_char = charVec->at(pos + 1);

                switch (next_char){

                    case 't':
                    case 'f': pos+=2; return token(token::TOKEN_BOOLEAN, "#" + std::string(1, next_char)); break;

                    case '\\': pos += 2; return token(token::TOKEN_CHARACTER, parseCharacter(pos)); break;
                    
                    case '(': pos += 2; return token(token::TOKEN_VECTOR_CONSTANT, "#" + std::string(1, next_char)); break;


                    // Radix and exactness markers
                    case 'e':
                    case 'i':
                    case 'b':
                    case 'o':
                    case 'd':
                    case 'x': state = 4; break;
                    //pos += 2; return token(token::TOKEN_CHARACTER, "#" + std::string(1, next_char)); break;
                }

                break;
 
            }

            // Number State
            case 4: {

                std::string prefix = "";

                if (c == '#'){

                    char next_char = charVec->at(pos+1);

                    switch (next_char)
                    {
                    case 'i':
                    case 'e':
                        pos += 2;
                        std::string(1, c) + std::string(1, next_char);
                        break;

                    case 'b':
                    case 'o':
                    case 'd':
                    case 'x':
                        pos += 2;
                        std::string(1, c) + std::string(1, next_char);
                        break;    
                    
                    }

                }


                return token(token::TOKEN_NUMBER, prefix + parseNumber(pos));
                
                //pos++;
                //
                break;

            }

            // Comma State
            case 5: {
            
                char next_char = charVec->at(pos+1);

                if (next_char == '@'){pos +=2; return token(token::TOKEN_COMMA_AT, std::string(1, c) + std::string(1, next_char));}
                else {pos +=2; return token(token::TOKEN_COMMA, std::string(1, c));}
            
            }

            // Case Delimited op
            case 6: {

                char next_char = charVec->at(pos + 1);

                if (c == '.'){

                    if (isDelimiter(next_char) && next_char != '.'){pos++; return token(token::TOKEN_PERIOD, std::string(1, c));}
                    else if (charVec->at(pos + 2) == '.'){
                        pos+=3;
                        return token(token::TOKEN_IDENTIFIER, "...");     

                    }

                }


                // if (c == '.' && isDelimiter(next_char)){
                //     pos++; 
                //     return token(token::TOKEN_PERIOD, std::string(1, c));
                // }
                // // Fix this. It is ugly.
                // // else if ((c == '.') && (next_char == '.') && (char_2nd == '.') && (isDelimiter(char_3rd))){
                // //     pos += 3; 
                // //     return token(token::TOKEN_IDENTIFIER, "...");}

                // bool ellipse = true;
                
                // for (int i = 1; i <= 3; ++i){

                //     char ahead = charVec->at(pos + i);
                //     ellipse &= (ahead == '.');
                // }   

                // if (ellipse){return token(token::TOKEN_IDENTIFIER, "...";}

                if (next_char = ' ') {pos++; return token(token::TOKEN_IDENTIFIER, std::string(1, c));}

                else {state = 4;}

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

void lexer::skipComment(int &pos){

    pos++;

    while (charVec->at(pos) != '\n'){

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

bool lexer::isLiteral(char c){}

std::string lexer::parseCharacter(int &pos){

    std::string character = parseIdentifier(pos);

    if (character.size() == 1){return character;}

    else if (character == "space"){return " ";}

    else if (character == "newline"){return "\\n";}

}

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

    pos++;

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
    const std::string extendedCharacter = "_+-.*/<=>!?:$%_&~^";

    std::string id;

    char current_char = charVec->at(pos);

    //id += current_char; 

    //char current_char = charVec->at(pos + offset);

    while (isalnum(current_char) || extendedCharacter.find(current_char) != std::string::npos){

        id += current_char;
        offset++;
        current_char = charVec->at(pos + offset);
 
    }

    pos += offset;

    //std::cout << id << std::endl;

    return id;

}

std::string lexer::parseNumber(int &pos){

    char current_char = charVec->at(pos);

    std::string number = "";

    while (!isDelimiter(current_char)){

        number += current_char;
        pos++;
        current_char = charVec->at(pos);


    }

    //std::cout << number << std::endl;

    return number;

}

//std::string lexer::parseLiteral(int &pos){}