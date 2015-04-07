//
//  main.cpp
//  Trequeue
//
//  Created by Yan Yu on 9/5/14.
//  Copyright (c) 2014 Yan Yu. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include "trequeue.h"

int main(int argc, const char * argv[]) {

    // insert code here...
    Trequeue<int> t;
    for (int i = 0; i < 420; i++) {
        t.add(0, i);
    }
    
    t.set(210, 5);
    t.remove(209);
    std::cout << t.get(209) << std::endl;

    std::cout << std::endl;
    
    for (int i = 0; i < t.size(); i++) {
        std::cout << t.get(i) << std::endl;
    }
    
    t.add(209, 10);
    std::cout << t.get(209) << std::endl;
    
    return 0;
}

