// Written by Josh Ebner

#include "ray3.h"

Ray3::Ray3() {
    p = Point3();
    dir = Vector3(1, 0, 0);
}

Ray3::Ray3(Point3 point, Vector3 vec) {
    p = point;
    dir = vec.toUnit();
}