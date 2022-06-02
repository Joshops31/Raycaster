#ifndef SHAPE_H
#define SHAPE_H

#include "point3.h"
#include "vector3.h"
#include "ray3.h"
#include "color.h"
#include "texCoords.h"

// Parent class for all shapes
class Shape {
public:
    Point3 pos;
    int m;

    // Constructors
    Shape();
    Shape(Point3 p, int mtl);

    // Returns the distance from the ray's origin to the point of intersection
    // Returns -1 if there is no intersection
    virtual float checkIntersect(Ray3 ray) = 0;
    
    // Returns the normal of the point on the shape's surface
    // Assumes the point is valid as this should only be called after checkIntersect
    virtual Vector3 getNormal(Point3 p) = 0;
};

#endif