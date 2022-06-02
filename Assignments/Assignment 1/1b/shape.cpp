// Written by Josh Ebner

#include "shape.h"

Shape::Shape() {
    pos = Point3();
    m = 0;
}

Shape::Shape(Point3 p, int mtl) {
    pos = p;
    m = mtl;
}