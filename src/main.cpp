#include <iostream>

int main(int argc, char *argv[]){

    if (argc != 2){

        std::cout << "Include file to compile." << std::endl;
        return 1;

    }

    char *file = argv[1];

    std::cout << file << std::endl;

	return 0;

}
