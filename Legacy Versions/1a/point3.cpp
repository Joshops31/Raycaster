// Written by Josh Ebner

#ifndef POINT3_H
#define POINT3_H

#include <iostream>
#include "vector3.cpp"

class Point3 {
public:
    float x, y, z;

    // Constructors
    Point3() {
        x = 0;
        y = 0;
        z = 0;
    }

    Point3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Operator overloads
    Vector3 operator-(Point3 other) {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Point3 operator+(Vector3 vec) {
        return Point3(x + vec.x, y + vec.y, z + vec.z);
    }

    Point3 operator-(Vector3 vec) {
        return Point3(x - vec.x, y - vec.y, z - vec.z);
    }
};

#endif