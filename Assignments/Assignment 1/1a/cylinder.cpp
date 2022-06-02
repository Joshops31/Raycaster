#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.cpp"

// Cylinder shape
class Cylinder : public Shape{
public:
    // pos is center, dir is the direction the top of the cylinder points to, length is the total length of the cylinder
    Vector3 dir;
    float radius;
    float length;

    // Constructors
    Cylinder() {
        pos = Point3();
        color = Color(1, 1, 1);
        dir = Vector3();
        radius = 1;
        length = 1;
    }

    Cylinder(Point3 p, Color col, Vector3 direction, float r, float l) {
        pos = p;
        color = col;
        dir = dir;
        radius = r;
        length = l;
    }

    // Find the distance between the ray and the intersection point it has with the cylinder if any.
    // -1 correlates to no intersection
    float checkIntersect(Ray3 ray) {
        
        return -1;
    }
};

#endif