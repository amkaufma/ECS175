//Andrew Kaufman
//998048873

#ifndef PROJ2_H
#define	PROJ2_H

struct vertex
{
    float x;
    float y;
    float z;
};

struct line
{
    int v1;
    int v2;
};

class Object
{
    int vcount;
    int ecount;
    vertex *points;
    line *edges;
    
public:
    Object();
    void bresenham(int current);
    void translate();
    void rotate();
    void scale();
};

#endif	/* PROJ2_H */

