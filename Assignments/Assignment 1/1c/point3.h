// Written by Josh Ebner

#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include "vector3.h"

class Point3 {
public:
    float x, y, z;

    // Constructors
    Point3();
    Point3(float x, float y, float z);

    // Operator overloads
    Vector3 operator-(Point3 other);
    Point3 operator+(Vector3 vec);
    Point3 operator-(Vector3 vec);
};

#endif