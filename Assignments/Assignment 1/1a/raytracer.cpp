// Written by Josh Ebner

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include "vector3.cpp"
#include "point3.cpp"
#include "ray3.cpp"
#include "color.cpp"
#include "shape.cpp"
#include "sphere.cpp"
#include "cylinder.cpp"

// Global variables
std::vector<Shape*> shapes;
float viewdist = 5;
Point3 eye = Point3();
Vector3 viewdir = Vector3(1, 0, 0);
Vector3 updir = Vector3(0, 1, 0);
float vfov = 90;
int imsize[] = {10, 10};
Color bkgcolor = Color();
Color mtlcolor = Color();

// Operator overloading for debugging
std::ostream& operator<<(std::ostream& os, const Point3& p) {
    os << "(" << p.x << "," << p.y << "," << p.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "<" << v.x << "," << v.y << "," << v.z << ">";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Ray3& r) {
    os << "(" << r.p.x << "," << r.p.y << "," << r.p.z << ")" << " <" << r.dir.x << "," << r.dir.y << "," << r.dir.z << ">";
    return os;
}

// Calculates the dot product of two vectors
float dot(Vector3 v1, Vector3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

// Calculates the cross product of two vectors
Vector3 cross(Vector3 v1, Vector3 v2) {
    return Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

// Determine the color of a pixel based on a ray from the eye into the scene
Color shadeRay(Ray3 ray, float t, Shape* s);

Color traceRay(Ray3 ray) {
    Shape* seen = NULL;
    float t = INFINITY;
    for(int i = 0; i < shapes.size(); i++) {
        float dist = shapes[i]->checkIntersect(ray);
        if(dist >= 0 && dist <= t) {
            seen = shapes[i];
            t = dist;
        }
    }
    return shadeRay(ray, t, seen);
}

// Helper function for traceRay
Color shadeRay(Ray3 ray, float t, Shape* s) {
    if(s == NULL) {
        return bkgcolor;
    }
    return s->color;
}

int main(int argc, char* argv[]) {
    // Check number of command line arguments
    if(argc != 3) {
        std::cout << "Please enter a file name (ex. input.txt)." << std::endl;
        exit(1);
    }

    // File reading
    std::ifstream in(argv[1]);
    std::string line;
    while(std::getline(in, line)) {
        std::vector<std::string> args;
        while(line.find(" ") != -1) {
            args.push_back(line.substr(0, line.find(" ")));
            line = line.substr(line.find(" ") + 1, line.length() - 1);
        }
        args.push_back(line);
        // Handle commands
        if(args[0] == "eye") {
            eye = Point3(stof(args[1]), stof(args[2]), stof(args[3]));
        }
        else if(args[0] == "viewdir") {
            viewdir = Vector3(stof(args[1]), stof(args[2]), stof(args[3])).toUnit();
        }
        else if(args[0] == "updir") {
            updir = Vector3(stof(args[1]), stof(args[2]), stof(args[3])).toUnit();
        }
        else if(args[0] == "vfov") {
            vfov = stof(args[1]);
        }
        else if(args[0] == "imsize") {
            imsize[0] = stoi(args[1]);
            imsize[1] = stoi(args[2]);
        }
        else if(args[0] == "bkgcolor") {
            bkgcolor = Color(stof(args[1]), stof(args[2]), stof(args[3]));
        }
        else if(args[0] == "mtlcolor") {
            mtlcolor = Color(stof(args[1]), stof(args[2]), stof(args[3]));
        }
        else if(args[0] == "sphere") {
            shapes.push_back(new Sphere(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtlcolor, stof(args[4])));
        }
        else if(args[0] == "cylinder") {
            shapes.push_back(new Cylinder(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtlcolor, Vector3(stof(args[4]),
                              stof(args[5]), stof(args[6])).toUnit(), stof(args[7]), stof(args[9])));
        }
    }
    in.close();

    // Initialize the pixel array
    Color** image = new Color*[imsize[0]];
    for(int i = 0; i < imsize[0]; i++) {
        image[i] = new Color[imsize[1]];
        for(int j = 0; j < imsize[1]; j++) {
            image[i][j] = bkgcolor;
        }
    }
    
    // Find corners of the view window
    Vector3 u = cross(viewdir, updir).toUnit();
    Vector3 v = cross(u, viewdir).toUnit();
    
    Vector3 viewdirn = viewdir * -1;
    Vector3 un = cross(viewdirn, updir).toUnit();
    Vector3 vn = cross(un, viewdirn).toUnit();
    
    float h = 2 * viewdist * tan(vfov * 3.14159 / 180 / 2);
    float w = h * imsize[0] / imsize[1];

    Point3 ul = eye + viewdir * viewdist - u * (w/2) + v * (h / 2);
    Point3 ur = eye + viewdir * viewdist + u * (w/2) + v * (h / 2);
    Point3 ll = eye + viewdir * viewdist - u * (w/2) - v * (h / 2);
    Point3 lr = eye + viewdir * viewdist + u * (w/2) - v * (h / 2);

    Point3 uln = eye + viewdirn * viewdist - un * (w/2) + vn * (h / 2);
    Point3 urn = eye + viewdirn * viewdist + un * (w/2) + vn * (h / 2);
    Point3 lln = eye + viewdirn * viewdist - un * (w/2) - vn * (h / 2);
    Point3 lrn = eye + viewdirn * viewdist + un * (w/2) - vn * (h / 2);

    std::cout << "Positive window corner ul ur ll lr" << ul << ur << ll << lr << std::endl;
    std::cout << "Negative window corner ul ur ll lr" << uln << urn << lln << lrn << std::endl;

    float pxLength = (ul - ur).length() / imsize[0];

    // Find colors for each pixel
    for(int i = 0; i < imsize[0]; i++) {
        for(int j = 0; j < imsize[1]; j++) {
            Ray3 ray = Ray3(eye, (ul + u * j * pxLength - v * i * pxLength) - eye);
            image[i][j] = traceRay(ray);
        }
    }

    // Write the image to a file
    std::ofstream out(argv[2]);
    int max = 255;
    out << "P3\n"
        << "# Created by Josh Ebner's raytracer\n"
        << imsize[0] << " " << imsize[1] << "\n"
        << max << std::endl;
    int pixels = 0;
    for(int y = 0; y < imsize[1]; y++) {
        for(int x = 0; x < imsize[0]; x++) {
            // Add a newline if a line has enough pixels on it already
            if(pixels > 5) {
                out << "\n";
                pixels = 0;
            }
            // Write pixel information to output file
            out << image[x][y].r * max << " " << image[x][y].g * max << " " << image[x][y].b * max << " ";
            pixels++;
        }
    }

    // Delete image array
    for(int i = 0; i < imsize[0]; i++) {
        delete image[i];
    }
    delete image;
}