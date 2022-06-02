// Written by Josh Ebner

#include "camera.h"

Camera::Camera() {
    eye = Point3();
    viewdir = Vector3(1, 0, 0);
    updir = Vector3(0, 1, 0);
    viewdist = 5;
    vfov = 90;
}

Camera::Camera(Point3 e, Vector3 view, Vector3 up, float dist, float fov) {
    eye = e;
    viewdir = view;
    updir = up;
    viewdist = dist;
    vfov = fov;
}