#ifndef token_h_guard
#define token_h_guard

#include <string>
#include "position.h"

struct token{

    enum token_type{
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_BOOLEAN,
        TOKEN_CHARACTER,
        TOKEN_STRING,
        TOKEN_VECTOR_CONSTANT, 
        TOKEN_L_PAREN,
        TOKEN_R_PAREN,
        TOKEN_SINGLE_QUOTE,
        TOKEN_BACK_QUOTE,
        TOKEN_COMMA,
        TOKEN_COMMA_AT,
        TOKEN_PERIOD,
        TOKEN_EOF,
    } type;

    std::string value;   

    void operator=(token t);

    token(token_type _t, std::string _value);
    token(token_type _t, std::string _value, Position _p);
    token();

    Position p;

};

#endif