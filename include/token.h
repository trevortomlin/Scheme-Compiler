#ifndef token_h_guard
#define token_h_guard
#include <string>

struct token{

    enum token_type{
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_BOOLEAN,
        TOKEN_CHARACTER,
        TOKEN_STRING,
        TOKEN_HASH_L_PAREN, 
        TOKEN_L_PAREN,
        TOKEN_R_PAREN,
        TOKEN_SINGLE_QUOTE,
        TOKEN_TILDE,
        TOKEN_COMMA,
        TOKEN_COMMA_AND,
        TOKEN_PERIOD,
        TOKEN_EOF,
    } type;

    std::string value;   

    token(token_type _t, std::string _value);

};

#endif