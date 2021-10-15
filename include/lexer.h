#ifndef lexer_h_guard
#define lexer_h_guard

#include "token.h"

class lexer{

    public:
        lexer(const char *file);
        token advance();

};

#endif