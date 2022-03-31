#include "codegenerator.h"
#include <iostream>

CodeGenerator::CodeGenerator(treenode ast) {

    generate(ast);

}

void CodeGenerator::generate(treenode ast) {

    if (ast.value == "root") {

        for (auto child : ast.children) {

            generate(child);

        }

    }

    if (ast.value == "procedure") {

        generate(ast.children[1]);
        generate(ast.children[2]);

    }

    if (ast.value == "+" || ast.value == "2") {

        std::cout << ast.value << " ";

    }

    if (ast.children.empty()) return;

}