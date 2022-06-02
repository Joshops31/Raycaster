// Written by Josh Ebner

#include "cylinder.h"
#include "texCoords.h"

Cylinder::Cylinder() {
    pos = Point3();
    m = 0;
    dir = Vector3();
    radius = 1;
    length = 1;
}

Cylinder::Cylinder(Point3 p, int mtl, int t, Vector3 direction, float r, float l) {
    pos = p;
    m = mtl;
    texture = t;
    dir = direction;
    radius = r;
    length = l;
}

float Cylinder::checkIntersect(Ray3 ray) {
    return -1;
}

Vector3 Cylinder::getNormal(Point3 p) {
    return Vector3();
}

TexCoords Cylinder::getTexCoords(Point3 p) {
    return TexCoords();
}