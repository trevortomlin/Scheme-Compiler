#include <iostream>
#include "lexer.h"

int main(int argc, char *argv[]){

    if (argc != 2){

        std::cout << "Include file to compile." << std::endl;
        return 1;

    }

    const char *file = argv[1];

    lexer l = lexer(file);

    //std::cout << l.advance().type << std::endl;

    // for (auto i = l.charVec->begin(); i != l.charVec->end(); ++i){

    //     std::cout << *i << std::endl;

    // }

    // std::cout << l.c << std::endl;
    

	return 0;

}
