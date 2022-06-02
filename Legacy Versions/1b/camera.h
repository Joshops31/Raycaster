// Written by Josh Ebner

#ifndef CAMERA_H
#define CAMERA_H

#include "point3.h"
#include "vector3.h"

class Camera {
public:
    Point3 eye = Point3();
    Vector3 viewdir = Vector3(1, 0, 0);
    Vector3 updir = Vector3(0, 1, 0);
    float viewdist;
    float vfov;

    // Constructors
    Camera();
    Camera(Point3 e, Vector3 view, Vector3 up, float dist, float fov);
};

#endif