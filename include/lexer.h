#ifndef lexer_h_guard
#define lexer_h_guard

#include "token.h"
#include <vector>
#include <iostream>

class lexer{

    public:

        std::vector<char> *charVec;
        char current_char;

        lexer(const char *file);
        token advance();
        ~lexer();

};

#endif