//============================================================================
// Name        : hw5.cpp
// Author      : Pierre
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ChainedHashTable.h"
#include "DataStructureTester.h"
#include "LinearHashTable.h"
#include "BinarySearchTree.h"
#include "RedBlackTree.h"

using namespace std;
using namespace ods;

int main() {
     /*****Chained Hash Table*****/
    DataStructureTester<ChainedHashTable<int>> testChained;
    /*****Linear Hash Table*****/
    DataStructureTester<LinearHashTable<int>> testLinear;
    /*****BST*****/
    DataStructureTester<BinarySearchTree<BSTNode1<int>,int> > testBST;
    /*****Red Black Tree*****/
    DataStructureTester<RedBlackTree<RedBlackNode1<int>,int> > testRedBlack;
    
    int i = 0;
    double time_chained = 0, timeLinear = 0, timeBST = 0, timeRedBlack = 0;
    while (i < 50) {
        time_chained += testChained.DoSequentialAdd(1, 50000, 1);
        time_chained += testChained.DoRandomAdd(50000);
        time_chained += testChained.DoSequentialFind(1, 50000, 1);
        time_chained += testChained.DoRandomFind(50000);
        time_chained += testChained.DoSequentialRemove(1, 50000, 1);
        time_chained += testChained.DoRandomRemove(50000);
        
        timeLinear += testLinear.DoSequentialAdd(1, 50000, 1);
        timeLinear += testLinear.DoRandomAdd(50000);
        timeLinear += testLinear.DoSequentialFind(1, 50000, 1);
        timeLinear += testLinear.DoRandomFind(50000);
        timeLinear += testLinear.DoSequentialRemove(1, 50000, 1);
        timeLinear += testLinear.DoRandomRemove(50000);
        
        timeBST += testBST.DoSequentialAdd(1, 50000, 1);
        timeBST += testBST.DoRandomAdd(50000);
        timeBST += testBST.DoSequentialFind(1, 50000, 1);
        timeBST += testBST.DoRandomFind(50000);
        timeBST += testBST.DoSequentialRemove(1, 50000, 1);
        timeBST += testBST.DoRandomRemove(50000);
        
        timeRedBlack += testRedBlack.DoSequentialAdd(1, 50000, 1);
        timeRedBlack += testRedBlack.DoRandomAdd(50000);
        timeRedBlack += testRedBlack.DoSequentialFind(1, 50000, 1);
        timeRedBlack += testRedBlack.DoRandomFind(50000);
        timeRedBlack += testRedBlack.DoSequentialRemove(1, 50000, 1);
        timeRedBlack += testRedBlack.DoRandomRemove(50000);
        i++;
    }
    std::cout << "total time for ChainedHashTable:\t" << time_chained << std::endl;
    std::cout << "total time for LinearHashTable:\t" << timeLinear << std::endl;
    std::cout << "total time for BST:\t" << timeBST << std::endl;
    std::cout << "total time for Red Black Tree:\t" << timeRedBlack << std::endl;
    return 0;
}
