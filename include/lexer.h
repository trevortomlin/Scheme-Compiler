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
        static bool isDelimiter(char c);
        static bool isIdentifier(char c);

        void skipComment(int &pos);

        std::string parseCharacter(int &pos);
        std::string parseIdentifier(int &pos);
        std::string parseNumber(int &pos);
        std::string parseLiteral(int &pos);
        std::string parseString(int &pos);
        std::string parseEscapeSequence(int &pos);
}; 

#endif