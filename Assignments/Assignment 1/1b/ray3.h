// Written by Josh Ebner

#ifndef RAY3_H
#define RAY3_H

#include "vector3.h"
#include "point3.h"

class Ray3 {
public:
    Point3 p;
    Vector3 dir;

    // Constructors
    Ray3();
    Ray3(Point3 point, Vector3 vec);
};

#endif