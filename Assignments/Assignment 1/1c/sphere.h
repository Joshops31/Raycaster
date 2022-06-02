#ifndef SPHERE_H
#define SPHERE_H

#include <math.h>
#include "shape.h"
#include "vector3.h"
#include "texCoords.h"

// A sphere shape
class Sphere : public Shape{
public:
    // pos is center
    float radius;

    // Constructors
    Sphere();
    Sphere(Point3 p, int mtl, int t, float r);

    // Find the distance between the ray and the intersection point it has with the sphere if any.
    // -1 correlates to no intersection
    float checkIntersect(Ray3 ray);

    // Returns the normal of the point on the sphere's surface
    // Assumes the point is valid as this should only be called after checkIntersect
    Vector3 getNormal(Point3 p);

    // Returns texture coordinates for a point
    // Assumes the point is valid as this should only be called after checkIntersect
    TexCoords getTexCoords(Point3 p);
};

#endif