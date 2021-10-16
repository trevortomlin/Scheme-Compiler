#ifndef lexer_h_guard
#define lexer_h_guard

#include "token.h"
#include <vector>
#include <iostream>

class lexer{

    public:

        std::vector<char> *charVec = new std::vector<char>;
        char c;
        int pos = 0;

        lexer(const char *file);
        token advance();
        ~lexer();

    private:

        static bool isPunctuator(char c);
        static bool isLiteral(char c);

        std::string parse_identifier(int &pos);
        std::string parse_number(int pos);
        std::string parse_literal(int pos);
};

#endif