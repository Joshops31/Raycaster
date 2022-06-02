// Written by Josh Ebner

#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3();
    Vector3(float x, float y, float z);

    // Operator overloads
    Vector3 operator+(Vector3 other);
    Vector3 operator-(Vector3 other);
    Vector3 operator*(float s);

    // Returns length of the vector
    float length();

    // Returns a unit vector
    Vector3 toUnit();

    // Returns dot product of this dot other
    float dot(Vector3 other);

    // Returns cross product of this cross other
    Vector3 cross(Vector3 other);
};

#endif