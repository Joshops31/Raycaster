// Written by Josh Ebner

#include "sphere.h"

Sphere::Sphere() {
    pos = Point3();
    m = 0;
    radius = 1;
}

Sphere::Sphere(Point3 p, int mtl, float r) {
    pos = p;
    m = mtl;
    radius = r;
}

float Sphere::checkIntersect(Ray3 ray) {
    // Plug ray equation into sphere equation
    float a = ray.dir.x*ray.dir.x + ray.dir.y*ray.dir.y + ray.dir.z*ray.dir.z;
    float b = 2 * (ray.dir.x * (ray.p.x - pos.x) + ray.dir.y * (ray.p.y - pos.y) + ray.dir.z * (ray.p.z - pos.z));
    float c = (ray.p.x - pos.x)*(ray.p.x - pos.x) + (ray.p.y - pos.y)*(ray.p.y - pos.y) +
              (ray.p.z - pos.z)*(ray.p.z - pos.z) - radius*radius;
    float discriminant = b*b - 4 * a * c;

    // Use the discriminant to determine how to handle finding distance
    if(discriminant == 0 && ((-b + sqrt(discriminant)) / (2*a)) >= 0) {
        // Grazes sphere
        return (-b + sqrt(discriminant)) / (2*a);
    }
    else if(discriminant > 0) {
        // Pierces sphere
        float plus = (-b + sqrt(discriminant)) / (2*a);
        float minus = (-b - sqrt(discriminant)) / (2*a);
        if((plus >= 0 && plus <= minus) || (plus >= 0 && minus < 0)) {
            return plus;
        }
        else if((minus >= 0 && minus <= plus) || (minus >= 0 && plus < 0)) {
            return minus;
        }
    }
    // Misses sphere
    return -1;
}

Vector3 Sphere::getNormal(Point3 p) {
    return (p - pos).toUnit();
}