// Written by Josh Ebner

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include "vector3.h"
#include "point3.h"
#include "ray3.h"
#include "color.h"
#include "shape.h"
#include "sphere.h"
#include "cylinder.h"
#include "material.h"
#include "camera.h"
#include "light.h"

// Global variables
std::vector<Shape*> shapes;
std::vector<Light*> lights;
Camera cam = Camera();
int imsize[] = {10, 10};
Color bkgcolor = Color();
std::vector<Material*> mtls;
struct DepthCue {
    Color c = Color();
    float amax = 1;
    float amin = 1;
    float dmax = 0;
    float dmin = 0;

    float a(float dist) {
        if(dist < dmin) {
            return amax;
        }
        else if(dist > dmax) {
            return amin;
        }
        return amin + (amax - amin) * ((dmax - dist) / (dmax - dmin));
    }

    Color col(Color col, float dist) {
        float aa = a(dist);
        return col * aa + c * (1 - aa);
    }
} dcue;

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
        // Misses objects
        return bkgcolor;
    }
    // Get color at the point of intersection, including lighting
    Point3 intersect = ray.p + (ray.dir * t);
    Vector3 n = s->getNormal(intersect);
    Material* mat = mtls[s->m];
    Color c = mat->obj * mat->ka;
    for(int i = 0; i < lights.size(); i++) {
        // Determine if the current light's light is blocked
        bool litUp = true;
        if(lights[i]->type == 0) { // Directional (pos becomes the direction)
            for(int j = 0; j < shapes.size(); j++) {
                float dist = shapes[j]->checkIntersect(Ray3(intersect + n * 0.01, Vector3(-lights[i]->pos.x, -lights[i]->pos.y, -lights[i]->pos.z)));
                if(dist >= 0) {
                    litUp = false;
                    break;
                }
            }
        }
        else if(lights[i]->type == 1) { // Point light
            for(int j = 0; j < shapes.size(); j++) {
                float dist = shapes[j]->checkIntersect(Ray3(intersect + n * 0.01, lights[i]->pos - intersect));
                if(dist >= 0 && dist <= (lights[i]->pos - intersect).length()) {
                    litUp = false;
                    break;
                }
            }
        }
        // If the light isn't blocked, add to the final color
        if(litUp) {
            Vector3 l;
            if(lights[i]->type == 0) {
                l = Vector3(-lights[i]->pos.x, -lights[i]->pos.y, -lights[i]->pos.z).toUnit();
            }
            else if(lights[i]->type == 1) {
                l = (lights[i]->pos - intersect).toUnit();
            }
            Vector3 h = ((ray.p - intersect).toUnit() + l).toUnit();
            Color diffuse = (mat->obj * mat->kd * (dot(n,l) < 0?0:dot(n,l)));
            Color specular = (mat->highlight * mat->ks * pow((dot(n,h) < 0?0:dot(n,h)),mat->shine));
            c = c + diffuse + specular;
        }
    }
    return dcue.col(c, t);
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
            cam.eye = Point3(stof(args[1]), stof(args[2]), stof(args[3]));
        }
        else if(args[0] == "viewdir") {
            cam.viewdir = Vector3(stof(args[1]), stof(args[2]), stof(args[3])).toUnit();
        }
        else if(args[0] == "updir") {
            cam.updir = Vector3(stof(args[1]), stof(args[2]), stof(args[3])).toUnit();
        }
        else if(args[0] == "vfov") {
            cam.vfov = stof(args[1]);
        }
        else if(args[0] == "imsize") {
            imsize[0] = stoi(args[1]);
            imsize[1] = stoi(args[2]);
        }
        else if(args[0] == "bkgcolor") {
            bkgcolor = Color(stof(args[1]), stof(args[2]), stof(args[3]));
        }
        else if(args[0] == "mtlcolor") {
            Color obj = Color(stof(args[1]), stof(args[2]), stof(args[3]));
            Color highlight = Color(stof(args[4]), stof(args[5]), stof(args[6]));
            float ka = stof(args[7]);
            float kd = stof(args[8]);
            float ks = stof(args[9]);
            float shine = stof(args[10]);
            mtls.push_back(new Material(obj, highlight, ka, kd, ks, shine));
        }
        else if(args[0] == "depthcueing") {
            dcue.c = Color(stof(args[1]), stof(args[2]), stof(args[3]));
            dcue.amax = stof(args[4]);
            dcue.amin = stof(args[5]);
            dcue.dmax = stof(args[6]);
            dcue.dmin = stof(args[7]);
        }
        else if(args[0] == "light") {
            Point3 p = Point3(stof(args[1]), stof(args[2]), stof(args[3]));
            int t = stoi(args[4]);
            Color c = Color(stof(args[5]), stof(args[6]), stof(args[7]));
            lights.push_back(new Light(p, t, c));
        }
        else if(args[0] == "sphere") {
            shapes.push_back(new Sphere(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtls.size() - 1, stof(args[4])));
        }
        else if(args[0] == "cylinder") {
            shapes.push_back(new Cylinder(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtls.size() - 1,
                                          Vector3(stof(args[4]), stof(args[5]), stof(args[6])).toUnit(),
                                          stof(args[7]), stof(args[9])));
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
    Vector3 u = cross(cam.viewdir, cam.updir).toUnit();
    Vector3 v = cross(u, cam.viewdir).toUnit();
    
    float h = 2 * cam.viewdist * tan(cam.vfov * 3.14159 / 180 / 2);
    float w = h * imsize[0] / imsize[1];

    Point3 ul = cam.eye + cam.viewdir * cam.viewdist - u * (w/2) + v * (h / 2);
    Point3 ur = cam.eye + cam.viewdir * cam.viewdist + u * (w/2) + v * (h / 2);
    Point3 ll = cam.eye + cam.viewdir * cam.viewdist - u * (w/2) - v * (h / 2);
    Point3 lr = cam.eye + cam.viewdir * cam.viewdist + u * (w/2) - v * (h / 2);

    float pxLength = (ul - ur).length() / imsize[0];

    // Find colors for each pixel
    for(int i = 0; i < imsize[0]; i++) {
        for(int j = 0; j < imsize[1]; j++) {
            Ray3 ray = Ray3(cam.eye, (ul + u * j * pxLength - v * i * pxLength) - cam.eye);
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