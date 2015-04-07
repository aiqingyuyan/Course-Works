#include "BinarySearchTree.h"
#include <iostream>

using namespace ods;

int main() {
    BinarySearchTree<BSTNode1<int>,int> bst;
    bst.add(16);
    bst.add(13);
    bst.add(18);
    bst.add(10);
    bst.add(14);
    bst.add(22);
    bst.add(9);
    bst.add(11);
    bst.add(15);
    bst.add(21);
    bst.add(23);
  
    std::cout << "height of BST(height()): " << bst.height() << std::endl;
    std::cout << "height of BST(height2()): " << bst.height2() << std::endl;
    std::cout << "Is tree balanced? " << (bst.isBalanced() ? "true" : "false") << std::endl;
  
    bst.preTraverse();
    bst.postTraverse();
    bst.inTraverse();

    DLList<int> list = bst.getLE(18);
    std::cout << "Items that are less than 18: ";

    for (int i = 0; i < list.size(); i++) {
        std::cout << list.get(i) << " ";
    }
    std::cout << std::endl;

    return 0;
}
