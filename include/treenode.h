#ifndef treenode_h_guard
#define treenode_h_guard

#include <vector>
#include <string>
#include <algorithm>

#include "position.h"

struct treenode{

    std::string value;
    std::vector<treenode> children;

    treenode(std::string v);

    Position p;

    void insert(treenode child);
    void delete_node(treenode child);
    static void printTree(const std::string& prefix, const treenode node);

};

#endif