//Andrew Kaufman
//998048873

#include "object.h"

Object::Object()
{
    
}

void Object::bresenham(int current)
{
    float temp1 = edges[current].v1;
    float temp2 = edges[current].v2;
    
    
    float temp;
    int location;
    int count;

    bool steep = (fabs(y2-y1) > fabs(x2-x1));                   //absolute value of slope greater than 1
    
    if(steep)
    {
        temp = x1;                                              //swap x and y values to decrease slope to less than 1
        x1 = y1;
        y1 = temp;
        
        temp = x2;
        x2 = y2;
        y2 = temp;
    }
    
    if(x1 > x2)
    {
        temp = x1;                                              //swap points 1 and 2 so that we can increment x rather than decrement
        x1 = x2;
        x2 = temp;
        
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    float dx = x2 - x1;
    float dy = fabs(y2 - y1);
    
    int yinc;
    
    if(y1 < y2)                                                 //if y2 is above y1, we will increment y by 1 (positive direction)
    {
        yinc = 1;
    }
    else                                                        //if y2 is below y1, we will decrement y by 1 (negative direction)
    {
        yinc = -1;
    }
    
    float p = (dx / 2.0f);                                      //decision parameter p
    int y = (int)y1;
    
    int xmax = (int)x2;
    
    count = shapes[curr].count;
    
    for(int x = (int)x1; x <= xmax; x++)
    {
        count += 1;
        
        if(steep)
        {
            location = (y * 3) + (x * width * 3);
            
            shapes[curr].edges[count].x = y;
            shapes[curr].edges[count].y = x;
            
            makePix(location);
        }
        else
        {
            location = (x * 3) + (y * width * 3);
            
            shapes[curr].edges[count].x = x;
            shapes[curr].edges[count].y = y;
            
            makePix(location);
        }
        
        
        p -= fabs(dy);
        if(p < 0)                                               //if p < 0 increment y
        {
            y += yinc;
            p += dx;
        }
        
    }
    
    shapes[curr].count = count;
    
    return;
    
}

void Object::translate()
{
    
    return;
}

void Object::rotate()
{
    
    return;
}

void Object::scale()
{
    
    return;
}