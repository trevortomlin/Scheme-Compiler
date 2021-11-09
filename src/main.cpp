#include <iostream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]){

    if (argc != 2){

        std::cout << "Include file to compile." << std::endl;
        return 1;

    }

    const char *file = argv[1];

    lexer *l = new lexer(file);
    parser *p = new parser(l);

    //std::cout << p->parse_expression().value << std::endl;
    p->parse();

    delete l;
    delete p;

    //std::cout << l.c << isalpha(l.c) << std::endl;-

    // token token1 = l.advance();
    // token token2 = l.advance();
    // std::cout << token1.type << " " << token1.value << std::endl;
    // std::cout << token2.type << " " << token2.value << std::endl;

    // std::cout << l.charVec->size() << std::endl;

    // for (auto i = l.charVec->begin(); i != l.charVec->end(); ++i){

    //     std::cout << *i << std::endl;

    // }

    //token t(token::TOKEN_EOF, "EOF");

    // do{

    //     //t = l.advance();
    //     //std::cout << "(" << t.type << ", " << "\"" << t.value << "\")" << std::endl;
    //     std::cout << p.parse_expression().value << std::endl;


    // }while(t.type != token::TOKEN_EOF);


    //std::cout << l.advance().type << std::endl;


    // std::cout << l.c << std::endl;
    

	return 0;

}