#include <fstream>
#include <vector>
#include <math.h>
#include <iostream>
#include <string>
#include "texture.h"

Texture::Texture(std::string filename) {
    std::ifstream in(filename);
    std::string line;
    std::vector<std::string> args;
    bool badSpacing = true;

    // Read first line, assumed to have all the header info
    std::getline(in, line);
    while(line.find("  ") != -1) {
        badSpacing = true;
        line = line.substr(0, line.find("  ")) + line.substr(line.find("  ") + 1, line.length() - 1);
    }
    while(line.find(" ") != -1) {
        args.push_back(line.substr(0, line.find(" ")));
        line = line.substr(line.find(" ") + 1, line.length() - 1);
    }
    args.push_back(line);

    // Initiialize img
    width = stoi(args[1]);
    height = stoi(args[2]);
    int max = stoi(args[3]);
    img = new Color*[width];
    for(int i = 0; i < width; i++) {
        img[i] = new Color[height];
        for(int j = 0; j < height; j++) {
            img[i][j] = Color();
        }
    }

    // Get all pixels from file
    std::string pixels = "";
    while(std::getline(in, line)) {
        pixels += line + " ";
    }

    in.close();
    
    // Transfer all file numbers to Color objects
    while(pixels.find("  ") != -1) {
        badSpacing = true;
        pixels = pixels.substr(0, pixels.find("  ")) + pixels.substr(pixels.find("  ") + 1, pixels.length() - 1);
    }
    std::vector<int> rgbs;
    while(pixels.length() > 0) {
        rgbs.push_back(stoi(pixels.substr(0, pixels.find(" "))));
        pixels = pixels.substr(pixels.find(" ") + 1, pixels.length() - 1);
    }
    for(int h = 0; h < height; h++) {
        for(int w = 0; w < width; w++) {
            img[w][h] = Color(rgbs[3 * (w + h * width)] / (1.0 * max), rgbs[3 * (w + h * width) + 1] / (1.0 * max), rgbs[3 * (w + h * width) + 2] / (1.0 * max));
        }
    }
}

Color Texture::getColor(TexCoords tc) {
    float u = fmod(tc.u, 1);
    float v = fmod(tc.v, 1);
    int pixW = round(u * (width - 1));
    int pixH = round(v * (height - 1));
    return img[pixW][pixH];
}