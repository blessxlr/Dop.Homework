#include "splay_tree_api.h"
#include <iostream>
#include <string>

int main() {
    SplayTree<int, std::string> tree;
    
    tree.insert(42, "Ответ");
    tree.insert(7, "Счастливое число");
    
    auto* val = tree.search(42);
    if (val) {
        std::cout << *val << std::endl;
    }
    
    std::cout << "Size: " << tree.size() << std::endl;
    
    return 0;
}
