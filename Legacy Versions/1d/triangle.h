#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shape.h"
#include "point3.h"
#include "vector3.h"
#include "texCoords.h"

// A triangle shape
class Triangle : public Shape {
public:
    Point3 v0, v1, v2;
    Vector3 n0, n1, n2;
    TexCoords t0, t1, t2;
    
    // Constructors
    Triangle();
    Triangle(Point3 v0, Point3 v1, Point3 v2, int m);
    Triangle(Point3 v0, Point3 v1, Point3 v2, Vector3 n0, Vector3 n1, Vector3 n2, int m);
    Triangle(Point3 v0, Point3 v1, Point3 v2, TexCoords t0, TexCoords t1, TexCoords t2, int m, int texture);
    Triangle(Point3 v0, Point3 v1, Point3 v2, Vector3 n0, Vector3 n1, Vector3 n2, TexCoords t0, TexCoords t1, TexCoords t2, int m, int texture);

    // Returns the distance from the ray's origin to the point of intersection
    // Returns -1 if there is no intersection
    float checkIntersect(Ray3 ray);
    
    // Returns the normal of the point on the triangle's surface
    // Assumes the point is valid as this should only be called after checkIntersect
    Vector3 getNormal(Point3 p);

    // Returns texture coordinates for a point
    // Assumes the point is valid as this should only be called after checkIntersect
    TexCoords getTexCoords(Point3 p);
};

#endif