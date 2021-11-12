#include "treenode.h"
#include <iostream>

treenode::treenode(std::string v){

    value = v;

}

void treenode::insert(treenode child){

    children.push_back(child);

}

// void treenode::delete_node(treenode child){

//     // for (auto &childIt : children) {
            
//     //     if(&child == childIt) {

//     //         children.erase(std::remove(children.begin(), children.end(), &child), children.end());

//     //     }

//     // }

//     auto it = std::find(children.begin(), children.end(), &child);
//     if(it != children.end()) {
//         children.erase(it);
//         children.erase(std::remove(children.begin(), children.end(), &child), children.end());
//     }

// }

void treenode::printTree(const std::string& prefix, const treenode node) {

    std::cout << prefix << node.value << std::endl;

    if (node.children.size() != 0) {

        for (auto &child : node.children) {
            
            treenode::printTree(prefix + "  ", child);    

        }

    }

}