#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Make sure there are the proper number of arguments
    if(argc != 3) {
        std::cout << "Program takes 2 arguments, the name of the image descriptor file and the output file name (imsize.txt output.ppm for example).\n";
        exit(1);
    }
    // Create streams
    std::ifstream in(argv[1], std::ios::out);
    std::ofstream out(argv[2], std::ios::out);
    int width, height, max;
    std::string junk;
    // Get image info and check that it is valid
    in >> junk >> width >> height;
    if(junk != "imsize") {
        std::cout << "Use a file that starts with imsize and is followed by width and height (imsize 10 15 for example).\n";
        exit(2);
    }
    if(width < 1 || height < 1) {
        std::cout << "Width and height must be positive numbers.\n";
        exit(2);
    }
    max = 255;
    // Write header info to output file
    out << "P3\n"
        << width << " " << height << "\n"
        << max << "\n";
    // Fill in the image with pixel information
    int pixels = 0;
    for(double y = 0; y < height; y++) {
        for(double x = 0; x < width; x++) {
            // Add a newline if a line has enough pixels on it already
            if(pixels > 5) {
                out << "\n";
                pixels = 0;
            }
            // Write pixel information to output file
            out << (int) ((x / width) * max) << " " << (int) ((y / height) * max) << " " << (int) (((x + y) / (height * width)) * max) << " ";
            pixels++;
        }
    }
    // Close streams
    in.close();
    out.close();
    return 0;
}