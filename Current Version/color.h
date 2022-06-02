#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    // RGB values on a scale of 0 - 1
    float r, g, b;

    // Constructors
    Color();
    Color(float red, float green, float blue);

    // Overloaders
    Color operator+(Color other);
    Color operator+=(Color other);
    Color operator*(float s);
};

#endif