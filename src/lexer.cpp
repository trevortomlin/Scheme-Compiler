#include "lexer.h"
#include <fstream>

#include <iostream>

lexer::lexer(const char *file){

    std::ifstream src_code(file);
    
    if (src_code.is_open()){

        char current_char;

        while (src_code){

            current_char = src_code.get();

            std::cout << current_char << std::endl;
            
            if (current_char == EOF) break;

        }

    }

    src_code.close();

}

token lexer::advance(){


}