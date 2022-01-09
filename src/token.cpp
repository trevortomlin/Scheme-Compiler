#include "token.h"
#include "position.h"

token::token(){

    type = token::TOKEN_EOF;
    value = "";
    p = Position(0, 0);

}

token::token(token_type _t, std::string _value){

    type = _t;
    value = _value;
    p = Position(0, 0);
    
}

token::token(token_type _t, std::string _value, Position _p) : p(_p) {

    type = _t;
    value = _value;

}

void token::operator=(token t){

    type = t.type;
    value = t.value;
    p = t.p;

}