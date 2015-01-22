//Andrew Kaufman
//998048873

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <GL/glut.h>

using namespace std;

float *pixelbuffer;

int width;
int height;


float r = 0.19;
float g = 0.98;
float b = 0.51;

struct vertex{
    float x;
    float y;
};

struct polygon{
    vertex *points;
};

struct scanline{
    float *pix;
};

void makePix(int location)
{
    
    pixelbuffer[location] = r;
    pixelbuffer[location + 1] = g;
    pixelbuffer[location + 2] = b;
    
    return;
    
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixelbuffer);
    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void ddaline(polygon *shapes, int curr, int v1, int v2)         //works so far
{
    float dx = (shapes[curr].points[v2].x - shapes[curr].points[v1].x);
    float dy = (shapes[curr].points[v2].y - shapes[curr].points[v1].y);
    float steps = 0;
    
    float x0 = (shapes[curr].points[v1].x);
    float y0 = (shapes[curr].points[v1].y);
    
    
    float x = x0;
    float y = y0;

    float xinc = 0;
    float yinc = 0;
    
    int location;
    
    if(fabs(dx) > fabs(dy))
    {
        steps = fabs(dx);
    }
    else
    {
        steps = fabs(dy);
    }
    
    xinc = dx / steps;
    yinc = dy / steps;
    
    location = (x * 3) + (y * width * 3);
    
    makePix(location);
    
    for(int k = 0; k < steps; k++)
    {
        x += xinc;
        y += yinc;
        location = (round(x) * 3) + (round(y) * width * 3);
        
        makePix(location);
    }

    
    return;
    
}

void bresenham(polygon *shapes, int curr, int v1, int v2)      
{
    float x1 = shapes[curr].points[v1].x;
    float x2 = shapes[curr].points[v2].x;
    float y1 = shapes[curr].points[v1].y;
    float y2 = shapes[curr].points[v2].y;
    
    float temp;
    int location;

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
    
    for(int x = (int)x1; x < xmax; x++)
    {
        if(steep)
        {
            location = (y * 3) + (x * width * 3);
            makePix(location);
        }
        else
        {
            location = (x * 3) + (y * width * 3);
            makePix(location);
        }
        
        p -= fabs(dy);
        if(p < 0)                                               //if p < 0 increment y
        {
            y += yinc;
            p += dx;
        }
        
    }
    
    return;
    
}

void rasterise(polygon *shapes, int curr, int vcount)       //works so far
{
    float xmin = shapes[curr].points[0].x;
    float xmax = shapes[curr].points[0].x;
    float ymin = shapes[curr].points[0].y;
    float ymax = shapes[curr].points[0].y;
    
    int dx;
    int dy;
    int pos;
    int temp;
    
    float x0;
    float y0;
    float temp1;
    float yi;
    float yf;
    
    for(int i = 0; i < (vcount); i++)
    {
        if(shapes[curr].points[i].x < xmin)
        {
            xmin = shapes[curr].points[i].x;
        }
        
        if(shapes[curr].points[i].x > xmax)
        {
            xmax = shapes[curr].points[i].x;
        }
        
        if(shapes[curr].points[i].y < ymin)
        {
            ymin = shapes[curr].points[i].y;
        }
        
        if(shapes[curr].points[i].y > ymax)
        {
            ymax = shapes[curr].points[i].y;
        }
        
    }//computes min and max values correctly
    
    dx = fabs(xmax - xmin);        //works
    dy = fabs(ymax - ymin);
    
    
    x0 = (xmin * 3);               //works
    y0 = (ymin * width * 3);
    pos = x0 + y0;// + origin;
    temp = pos;
    
    
    scanline *edgepixels = new scanline[dx];        //allocates correctly
    int count = 0;
    
    for(int j = 0; j < dx; j++)
    {
        edgepixels[j].pix = new float[dy];          //allocates correctly

        for(int k = 0; k < dy; k++)
        {
            edgepixels[j].pix[k] = -1;          //set to negative value to check for later
            if((pixelbuffer[temp] == r) && (pixelbuffer[temp+1] == g) && (pixelbuffer[temp+2] == b))
            {
                edgepixels[j].pix[k] = temp;
            }//not sure if working correctly
            
            temp += (width * 3);
        }
        
        pos += 3;
        temp = pos;
    }
    
    for(int m = 0; m < dx; m++)
    {
        yi = 930000;
        yf = 0;
        for(int n = 0; n < dy; n++)
        {
            if(edgepixels[m].pix[n] != -1)
            {
                if(edgepixels[m].pix[n] < yi)
                {
                    yi = edgepixels[m].pix[n];
                }
                else if(edgepixels[m].pix[n] > yf)
                {
                    yf = edgepixels[m].pix[n];
                }
                
            }

        }
        
        temp = yi;
        
        while(temp <= yf && temp >= yi)
        {
            makePix(temp);
            temp += (width * 3);
        }
        
    }
    
    
    return;
}

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    ifstream infile;
    infile.open(filename);
    
    int num;
    int vertexnum;
    int current = 0;
    int choice;
    
    string line;
    
    cout << "Please enter window size (width height): ";
    cin >> width >> height;
    cout << "Which line drawing algorithm?" << endl << "1. DDA" << endl << "2. Bresenham" << endl;
    cout << "Choice (1 or 2): ";
    cin >> choice;
    
    
    pixelbuffer = new float[width * height * 3];
    
    getline(infile, line);                          //gets first line with number of polygons
    istringstream iss(line);
    iss >> num;                                     //ignores whitespace puts number of polygons into num
    
    polygon *id = new polygon[num];
    
    for(int i = 0; i < num; i++)
    {
        getline(infile, line);                      //get rid of junk line   
        getline(infile, line);
        istringstream iss2(line);
        iss2 >> vertexnum;                                    //gets next line containing number of vertices for first polygon
        
        id[i].points = new vertex[vertexnum];
        
        for(int j = 0; j < vertexnum; j++)
        {
            getline(infile, line);
            istringstream iss3(line);
            iss3 >> id[i].points[j].x >> id[i].points[j].y;
        }
        
        for(int k = 0; k < vertexnum - 1; k++)
        {
            if(choice == 1)
            {
                ddaline(id, i, k, k+1);
            }
            if(choice == 2)
            {
                bresenham(id, i, k, k+1);
            }

        }
        
        if(choice == 1)
        {
            ddaline(id, i, (vertexnum - 1), 0);
        }
        if(choice == 2)
        {
            bresenham(id, i, (vertexnum - 1), 0);
        }
        
        rasterise(id, i, vertexnum);

        //current++;     
    }
    
    
    glutInit(&argc, argv);			//initialize GLUT 
    glutInitDisplayMode(GLUT_SINGLE);	//set initial display mode
    glutInitWindowSize(width, height);	//initialize window size
    glutInitWindowPosition(100, 100);	//set window position (x,y)
    glutCreateWindow("Program 1");		//set window name
    init();                             //initializes rgb colors
  
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
