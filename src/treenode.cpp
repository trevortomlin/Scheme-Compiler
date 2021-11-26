#include "treenode.h"
#include <iostream>

treenode::treenode(std::string v){

    value = v;

}

void treenode::insert(treenode child){

    children.push_back(child);

}

void treenode::printTree(const std::string& prefix, const treenode node) {

    std::cout << prefix << node.value << std::endl;

    if (node.children.size() != 0) {

        for (auto &child : node.children) {
            
            treenode::printTree(prefix + "  ", child);    

        }

    }

}