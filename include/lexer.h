#ifndef lexer_h_guard
#define lexer_h_guard

#include "token.h"
#include <vector>
#include <iostream>

class lexer{

    public:

        // This should not be a pointer.
        std::vector<char> *charVec = new std::vector<char>;
        char c;
        int pos = 0;

        int charNum = 0;
        int line = 1;

        lexer(const char *file);
        token advance();
        ~lexer();

    private:

        // These should go in a seperate class.
        static bool isPunctuator(char c);
        static bool isLiteral(char c);
        static bool isDelimiter(char c);
        static bool isIdentifier(char c);

        void skipComment(int &pos);

        // These should be renamed.
        std::string parseCharacter(int &pos);
        std::string parseIdentifier(int &pos);
        std::string parseNumber(int &pos);
        std::string parseLiteral(int &pos);
        std::string parseString(int &pos);
        std::string parseEscapeSequence(int &pos);
}; 

#endif