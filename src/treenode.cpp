#include "treenode.h"

treenode::treenode(std::string v){

    value = v;

}

void treenode::insert(treenode *child){

    children.push_back(child);

}

void treenode::delete_node(treenode *child){

    auto it = std::find(children.begin(), children.end(), child);
    if(it != children.end())
        children.erase(it);
//children.erase(std::remove(children.begin(), children.end(), child), children.end());

}