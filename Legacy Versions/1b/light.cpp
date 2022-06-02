// Written by Josh Ebner

#include "light.h"

Light::Light() {
    pos = Point3();
    type = 0;
    col = Color(1, 1, 1);
}

Light::Light(Point3 pos, int type, Color col) {
    this->pos = pos;
    this->type = type;
    this->col = col;
}