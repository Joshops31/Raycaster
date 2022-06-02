// Written by Josh Ebner

#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Operator overloads
    Vector3 operator+(Vector3 other) {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator*(float s) {
        return Vector3(s*x, s*y, s*z);
    }

    // Returns length of the vector
    float length() {
        return sqrt(x*x + y*y + z*z);
    }

    // Returns a unit vector
    Vector3 toUnit() {
        return Vector3(x / length(), y / length(), z / length());
    }
};

#endif