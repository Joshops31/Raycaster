// Written by Josh Ebner

#ifndef LIGHT_H
#define LIGHT_H

#include "point3.h"
#include "color.h"

class Light {
public:
    Point3 pos;
    int type;
    Color col;

    // Constructors
    Light();
    Light(Point3 p, int type, Color color = Color(1,1,1));
};

#endif