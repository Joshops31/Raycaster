// Written by Josh Ebner

#include "color.h"
#include <math.h>

Color::Color() {
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(float red, float green, float blue) {
    r = fabs(red) < 1 ? fabs(red):1;
    g = fabs(green) < 1 ? fabs(green):1;
    b = fabs(blue) < 1 ? fabs(blue):1;
}

Color Color::operator+(Color other) {
    return Color((r + other.r < 1)?(r + other.r):1, (g + other.g < 1)?(g + other.g):1, (b + other.b < 1)?(b + other.b):1);
}

Color Color::operator+=(Color other) {
    *this = *this + other;
    return *this;
}

Color Color::operator*(float s) {
    float as = fabs(s);
    return Color((r*as < 1)?(r*as):1, (g*as < 1)?(g*as):1, (b*as < 1)?(b*as):1);
}