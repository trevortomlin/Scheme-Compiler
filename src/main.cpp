#include <iostream>
#include "lexer.h"

int main(int argc, char *argv[]){

    if (argc != 2){

        std::cout << "Include file to compile." << std::endl;
        return 1;

    }

    const char *file = argv[1];

    lexer l = lexer(file);

	return 0;

}
