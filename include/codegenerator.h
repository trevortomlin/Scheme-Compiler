#ifndef codegenerator_h_guard
#define codegenerator_h_guard

#include <treenode.h>

class CodeGenerator {

    public:
    CodeGenerator(treenode ast);
    void generate(treenode ast);

};

#endif 