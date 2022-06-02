// Written by Josh Ebner

#include "vector3.h"

Vector3::Vector3() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::operator+(Vector3 other) {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator*(float s) {
    return Vector3(s*x, s*y, s*z);
}

float Vector3::length() {
    return sqrt(x*x + y*y + z*z);
}

Vector3 Vector3::toUnit() {
    return Vector3(x / length(), y / length(), z / length());
}

float Vector3::dot(Vector3 other) {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(Vector3 other) {
    return Vector3(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
}