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

    p->parse();

    delete l;
    delete p;

	return 0;

}