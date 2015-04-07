//
//  Rectangle.h
//  cse-274-lab1
//
//  Created by Yan Yu on 8/27/14.
//  Copyright (c) 2014 Yan Yu. All rights reserved.
//

#ifndef cse_274_lab1_Rectangle_h
#define cse_274_lab1_Rectangle_h

class Rectangle
{
private:
    int width, height;
    
public:
    Rectangle();
    Rectangle(int x, int y);
    ~Rectangle();
    
    void setValues(int x,int y);
    int area();
};

#endif
