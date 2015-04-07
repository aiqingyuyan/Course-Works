//
//  main.cpp
//  cse-274-lab1
//
//  Created by Yan Yu on 8/27/14.
//  Copyright (c) 2014 Yan Yu. All rights reserved.
//

#include <iostream>
#include "Rectangle.h"

using std::cout;
using std::cin;
using std::endl;


int main(int argc, const char * argv[])
{

    // insert code here...
    Rectangle rect;
    
    Rectangle rect2 = rect;
    
    rect2.setValues(4, 5);
    
    cout << "area for rect2: " << rect2.area() << endl;
    
    cout << "area for rect: " << rect.area() << endl;
    
    rect.setValues(2, 5);
    
    cout<< "area for rect: " << rect.area() << endl;
    
    // create a pointer to a rectangle
    Rectangle *pRect;
    
    pRect = &rect;
    cout << "area for rect: " << pRect->area() << endl;
    cout << "memory address of rect: " << pRect << endl;
    
    pRect = &rect2;
    cout<< "area for rect2: " << pRect->area() << endl;
    cout << "memory address of rect2: " << pRect << endl;
    
    // create a reference
    Rectangle &refRect = rect;
    cout << "area for rect (reference): " << refRect.area() << endl;
    
    
    return 0;
}

