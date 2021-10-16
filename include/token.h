#ifndef token_h_guard
#define token_h_guard
#include <string>

struct token{

    enum token_type{
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_OPERATOR,
        TOKEN_PUNCTUATOR,
        TOKEN_LITERAL,
        TOKEN_EOF,
    } type;

    std::string value;   

    token(token_type _t, std::string _value);

};

#endif