#ifndef treenode_h_guard
#define treenode_h_guard

#include <vector>
#include <string>
#include <algorithm>

struct treenode{

    std::string value;
    std::vector<treenode*> children;

    treenode(std::string v);

    void insert(treenode *child);
    void delete_node(treenode *child);

};

struct treenode 

#endif