#include "token.h"

token::token(){

    type = token::TOKEN_EOF;
    value = "";

}

token::token(token_type _t, std::string _value){

    type = _t;
    value = _value;
    
}

void token::operator=(token t){

    type = t.type;
    value = t.value;

}