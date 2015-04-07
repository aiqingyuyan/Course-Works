//
//  Rectangle.cpp
//  cse-274-lab1
//
//  Created by Yan Yu on 8/27/14.
//  Copyright (c) 2014 Yan Yu. All rights reserved.
//

#include "Rectangle.h"

Rectangle::Rectangle()
{
    width = 1;
    height = 2;
}

Rectangle::Rectangle(int x, int y) : width(x), height(y) {}

Rectangle::~Rectangle() {}

int Rectangle::area()
{
    return width * height;
}

void Rectangle::setValues(int x,int y)
{
    width = x;
    height = y;
}