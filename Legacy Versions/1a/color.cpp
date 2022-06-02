#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    float r, g, b;

    // Constructors
    Color() {
        r = 0;
        g = 0;
        b = 0;
    }
    
    Color(float red, float green, float blue) {
        r = red;
        g = green;
        b = blue;
    }
};

#endif