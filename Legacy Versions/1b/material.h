// Written by Josh Ebner

#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"

class Material {
public:
    Color obj;
    Color highlight;
    float ka;
    float kd;
    float ks;
    float shine;

    // Constructors
    Material();
    Material(Color object, Color specular, float ka, float kd, float ks, float shine);
};

#endif