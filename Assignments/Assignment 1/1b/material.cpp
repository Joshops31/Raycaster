// Written by Josh Ebner

#include "material.h"

Material::Material() {
    obj = Color();
    highlight = Color();
    ka = 0.1;
    kd = 0;
    ks = 0;
    shine = 5;
}

Material::Material(Color object, Color highlight, float ka, float kd, float ks, float shine) {
    obj = object;
    this->highlight = highlight;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->shine = shine;
}