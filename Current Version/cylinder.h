#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "texCoords.h"

// Cylinder shape
class Cylinder : public Shape{
public:
    // pos is center, dir is the direction the top of the cylinder points to, length is the total length of the cylinder
    Vector3 dir;
    float radius;
    float length;

    // Constructors
    Cylinder();
    Cylinder(Point3 p, int mtl, int t, Vector3 direction, float r, float l);

    // Find the distance between the ray and the intersection point it has with the cylinder if any.
    // -1 correlates to no intersection
    float checkIntersect(Ray3 ray);
    
    // Returns the normal of the point that a ray hits
    // Returns null on miss
    Vector3 getNormal(Point3 p);

    // Returns texture coordinates for a point
    // Assumes the point is valid as this should only be called after checkIntersect
    TexCoords getTexCoords(Point3 p);
};

#endif