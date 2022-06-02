#ifndef SHAPE_H
#define SHAPE_H

#include "point3.cpp"
#include "ray3.cpp"
#include "color.cpp"

// Parent class for all shapes
class Shape {
public:
    Point3 pos;
    Color color;
    int m;

    // Constructors
    Shape() {
        pos = Point3();
        color = Color(1, 1, 1);
        m = 0;
    }

    Shape(Point3 p, Color col, int mtl) {
        pos = p;
        color = col;
        m = mtl;
    }

    virtual float checkIntersect(Ray3 ray) = 0;
};

#endif