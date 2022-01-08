#ifndef treenode_error_h_guard
#define treenode_error_h_guard

#include "treenode.h"
#include "position.h"

struct treenode_error : treenode {

    std::string value = "Error";

    std::string errorMessage;

    treenode_error(std::string v);

    Position p;

};

#endif