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
#include "triangle.h"
#include "texture.h"
#include "texCoords.h"

// Global variables
std::vector<Shape*> shapes;
std::vector<Light*> lights;
Camera cam = Camera();
int imsize[] = {10, 10};
Color bkgcolor = Color();
float sceneRefraction = 1;
std::vector<Material*> mtls;
std::vector<Point3> vertices;
std::vector<Vector3> normals;
std::vector<TexCoords> textureCoords;
std::vector<Texture> textures;
std::vector<Texture> normalMaps;
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

// Determine the color of a pixel based on a ray from the eye into the scene
Color shadeRay(Ray3 ray, float t, Shape* s, bool incidentInObject, int rayDepth);

Color traceRay(Ray3 ray, bool incidentInObject = false, int rayDepth = 0) {
    if(rayDepth > 3) {
        // Is too deep of an array to keep calculating (reflection/refraction)
        return bkgcolor;
    }
    Shape* seen = NULL;
    float t = INFINITY;
    for(int i = 0; i < shapes.size(); i++) {
        float dist = shapes[i]->checkIntersect(ray);
        if(dist >= 0 && dist <= t) {
            seen = shapes[i];
            t = dist;
        }
    }
    return shadeRay(ray, t, seen, incidentInObject, rayDepth);
}

Color shadeRay(Ray3 ray, float t, Shape* s, bool incidentInObject = false, int rayDepth = 0) {
    if(s == NULL) {
        // Misses objects
        return bkgcolor;
    }
    // Get color at the point of intersection, including lighting
    Point3 intersect = ray.p + (ray.dir * t);
    Vector3 n = s->getNormal(intersect);
    Material* mat = mtls[s->m];
    Color matColor = mat->obj;
    if(s->texture != -1) {
        matColor = textures[s->texture].getColor(s->getTexCoords(intersect));
    }
    Color c = matColor * mat->ka;
    for(int i = 0; i < lights.size(); i++) {
        // Determine if the current light's light is blocked
        float shadows = 1;
        if(lights[i]->type == 0) { // Directional (pos becomes the direction)
            for(int j = 0; j < shapes.size(); j++) {
                float dist = shapes[j]->checkIntersect(Ray3(intersect + n * 0.01, Vector3(-lights[i]->pos.x, -lights[i]->pos.y, -lights[i]->pos.z)));
                if(dist >= 0) {
                    shadows *= (1 - mtls[shapes[j]->m]->opacity);
                    if(shadows == 0) {
                        break;
                    }
                }
            }
        }
        else if(lights[i]->type == 1) { // Point light
            for(int j = 0; j < shapes.size(); j++) {
                float dist = shapes[j]->checkIntersect(Ray3(intersect + n * 0.01, lights[i]->pos - intersect));
                if(dist >= 0 && dist <= (lights[i]->pos - intersect).length()) {
                    shadows *= (1 - mtls[shapes[j]->m]->opacity);
                    if(shadows == 0) {
                        break;
                    }
                }
            }
        }
        // If the light isn't blocked, add to the final color
        if(shadows > 0) {
            Vector3 l;
            if(lights[i]->type == 0) {
                l = Vector3(-lights[i]->pos.x, -lights[i]->pos.y, -lights[i]->pos.z).toUnit();
            }
            else if(lights[i]->type == 1) {
                l = (lights[i]->pos - intersect).toUnit();
            }
            Vector3 h = ((ray.p - intersect).toUnit() + l).toUnit();
            Color diffuse;
            Color specular;
            if(n.dot(l) < 0) {
                diffuse = matColor * mat->kd * 0;
            }
            else {
                diffuse = matColor * mat->kd * n.dot(l);
            }
            specular = (mat->highlight * mat->ks * pow((n.dot(h) < 0?0:n.dot(h)),mat->shine));
            c += ((diffuse + specular) * shadows);
        }
    }
    // Reflectance handling
    Vector3 incidentVector = ray.dir * -1;
    Vector3 actingN = n;
    if(incidentInObject) {
        actingN = n * -1;
    }
    Ray3 reflectRay = Ray3(intersect + actingN * 0.01, actingN * actingN.dot(incidentVector) * 2 - incidentVector);
    float fres0 = pow((mat->indRef - 1.0) / (mat->indRef + 1.0), 2);
    float fres = fres0 + (1.0 - fres0) * pow(1.0 - incidentVector.dot(actingN), 5);
    Color reflectCol = traceRay(reflectRay, incidentInObject, rayDepth = rayDepth + 1) * fres;

    // Refraction handling
    float etaT;
    float etaI;
    float thetaI;
    if(incidentInObject) {
        // Incident ray is inside an object so refraction is outside object
        etaI = mat->indRef;
        etaT = sceneRefraction;
        thetaI = acos((n * -1).dot(incidentVector));
    }
    else {
        // Incident ray is outside an object so refraction is inside object
        etaT = mat->indRef;
        etaI = sceneRefraction;
        thetaI = acos(n.dot(incidentVector));
    }
    Vector3 refractDir;
    Ray3 refractRay;
    Color refractCol;
    if(incidentInObject && thetaI >= asin(sceneRefraction / etaI)) {
        // Refracted ray does not leave the object
        refractCol = Color();
    }
    else if(incidentInObject) {
        // Refracted ray will be outside the object
        refractDir = n * sqrt(1 - pow(etaI / etaT,2) * (1 - pow(cos(thetaI), 2)))
                     + (n * cos(thetaI) + ray.dir) * (etaI / etaT);
        refractRay = Ray3(intersect + n * 0.01, refractDir);
        refractCol = traceRay(refractRay, false, rayDepth = rayDepth + 1) * (1 - fres) * (1 - mat->opacity);
    }
    else {
        // Refracted ray will be inside the object
        refractDir = n * -1 * sqrt(1 - pow(etaI / etaT,2) * (1 - pow(cos(thetaI), 2)))
                     + (n * cos(thetaI) + ray.dir) * (etaI / etaT);
        refractRay = Ray3(intersect - n * 0.01, refractDir);
        refractCol = traceRay(refractRay, true, rayDepth = rayDepth + 1) * (1 - fres) * (1 - mat->opacity);
    }
    // if(mat->indRef != 1) {
    //     std::cout << "mat->indRef: " << mat->indRef << " Fres0: " << fres0 << " Fres: " << fres <<  " RayDepth: " << rayDepth << std::endl;
    //     std::cout << "InObject? " << incidentInObject << "    I: " << ray.dir * -1 << "   N: " << n << "    refractDir: " << refractDir << std::endl;
    // }
    c += reflectCol + refractCol;
    //c += reflectCol;   // Use this to test reflectance if refraction isn't working right
    return dcue.col(c, t);
}

// Splits a string based on a delimiter string
std::vector<std::string> split(std::string str, std::string del) {
    std::vector<std::string> strings;
    while(str.find(del) != -1) {
        strings.push_back(str.substr(0, str.find(del)));
        str = str.substr(str.find(del) + 1, str.length() - 1);
    }
    if(str != "") {
        strings.push_back(str);
    }
    return strings;
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
        while(line.find("  ") != -1) {
            line = line.substr(0, line.find("  ")) + line.substr(line.find("  ") + 1, line.length() - 1);
        }
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
            float opacity = stof(args[11]);
            float indexOfRefraction = stof(args[12]);
            mtls.push_back(new Material(obj, highlight, ka, kd, ks, shine, opacity, indexOfRefraction));
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
            shapes.push_back(new Sphere(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtls.size() - 1,
                                        textures.size() - 1, stof(args[4])));
        }
        else if(args[0] == "cylinder") {
            shapes.push_back(new Cylinder(Point3(stof(args[1]), stof(args[2]), stof(args[3])), mtls.size() - 1,
                                          textures.size() - 1, Vector3(stof(args[4]), stof(args[5]), stof(args[6])).toUnit(),
                                          stof(args[7]), stof(args[9])));
        }
        else if(args[0] == "v") {
            vertices.push_back(Point3(stof(args[1]), stof(args[2]), stof(args[3])));
        }
        else if(args[0] == "f") {
            if(split(args[1], "/")[0] == args[1]) { // Only vertices
                shapes.push_back(new Triangle(vertices[stoi(args[1]) - 1], vertices[stoi(args[2]) - 1],
                                 vertices[stoi(args[3]) - 1], mtls.size() - 1));
            }
            else if(split(args[1], "/").size() == 2) { // Vertices + texcoords
                std::vector<std::string> ind1 = split(args[1], "/");
                std::vector<std::string> ind2 = split(args[2], "/");
                std::vector<std::string> ind3 = split(args[3], "/");
                shapes.push_back(new Triangle(vertices[stoi(ind1[0]) - 1], vertices[stoi(ind2[0]) - 1],
                                 vertices[stoi(ind3[0]) - 1], textureCoords[stoi(ind1[1]) - 1],
                                 textureCoords[stoi(ind2[1]) - 1], textureCoords[stoi(ind3[1]) - 1],
                                 mtls.size() - 1, textures.size() - 1));
            }
            else {
                std::vector<std::string> ind1 = split(args[1], "/");
                std::vector<std::string> ind2 = split(args[2], "/");
                std::vector<std::string> ind3 = split(args[3], "/");
                if(ind1[1] == "") { // Vertices + normals
                    shapes.push_back(new Triangle(vertices[stoi(ind1[0]) - 1], vertices[stoi(ind2[0]) - 1],
                                 vertices[stoi(ind3[0]) - 1], normals[stoi(ind1[2]) - 1],
                                 normals[stoi(ind2[2]) - 1], normals[stoi(ind3[2]) - 1],
                                 mtls.size() - 1));
                }
                else { // Vertices + texcoords + normals
                    shapes.push_back(new Triangle(vertices[stoi(ind1[0]) - 1], vertices[stoi(ind2[0]) - 1],
                                     vertices[stoi(ind3[0]) - 1], normals[stoi(ind1[2]) - 1],
                                     normals[stoi(ind2[2]) - 1], normals[stoi(ind3[2]) - 1],
                                     textureCoords[stoi(ind1[1]) - 1], textureCoords[stoi(ind2[1]) - 1],
                                     textureCoords[stoi(ind3[1]) - 1], mtls.size() - 1, textures.size() - 1));
                }
            }
        }
        else if(args[0] == "vn") {
            normals.push_back(Vector3(stof(args[1]), stof(args[2]), stof(args[3])).toUnit());
        }
        else if(args[0] == "vt") {
            textureCoords.push_back(TexCoords(stof(args[1]), stof(args[2])));
        }
        else if(args[0] == "texture") {
            textures.push_back(Texture(args[1]));
        }
        else if(args[0] == "bump") {
            normalMaps.push_back(Texture(args[1]));
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
    Vector3 u = cam.viewdir.cross(cam.updir).toUnit();
    Vector3 v = u.cross(cam.viewdir).toUnit();
    
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
    out.close();

    // Delete image array
    for(int i = 0; i < imsize[0]; i++) {
        delete image[i];
    }
    delete image;
}