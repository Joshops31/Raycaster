// Written by Josh Ebner

#include "shape.h"

Shape::Shape() {
    pos = Point3();
    m = 0;
    texture = -1;
}

Shape::Shape(Point3 p, int mtl, int t) {
    pos = p;
    m = mtl;
    texture = t;
}