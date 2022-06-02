// Written by Josh Ebner

#ifndef RAY3_H
#define RAY3_H

#include "vector3.cpp"
#include "point3.cpp"

class Ray3 {
public:
    Point3 p;
    Vector3 dir;

    // Constructors
    Ray3() {
        p = Point3();
        dir = Vector3(1, 0, 0);
    }

    Ray3(Point3 point, Vector3 vec) {
        p = point;
        dir = vec.toUnit();
    }
};

#endif