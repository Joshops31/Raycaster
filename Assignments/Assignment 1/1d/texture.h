#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include <string>
#include <fstream>
#include "texCoords.h"

// Class that represents a texture
class Texture {
public:
    Color** img;
    int width, height;

    // Constructor
    Texture(std::string filename);

    // Returns a color based on texCoords passed in
    // u and v > 1 wraps
    Color getColor(TexCoords tc);
};

#endif