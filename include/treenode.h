#ifndef treenode_h_guard
#define treenode_h_guard

#include <vector>
#include <string>

struct treenode{

    std::string value;
    std::vector<treenode> children;

};

#endif