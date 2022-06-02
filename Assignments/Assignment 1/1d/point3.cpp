// Written by Josh Ebner

#include "point3.h"

Point3::Point3() {
    x = 0;
    y = 0;
    z = 0;
}

Point3::Point3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Point3::operator-(Point3 other) {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Point3 Point3::operator+(Vector3 vec) {
    return Point3(x + vec.x, y + vec.y, z + vec.z);
}

Point3 Point3::operator-(Vector3 vec) {
    return Point3(x - vec.x, y - vec.y, z - vec.z);
}