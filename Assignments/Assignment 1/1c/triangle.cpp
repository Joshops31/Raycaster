#include "triangle.h"
#include "texCoords.h"

Triangle::Triangle() {
    v0 = Point3();
    v1 = Point3();
    v2 = Point3();
    n0 = Vector3();
    n1 = Vector3();
    n2 = Vector3();
    t0 = TexCoords();
    t1 = TexCoords();
    t2 = TexCoords();
    m = 0;
    texture = -1;
}

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2, int m) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    Vector3 planeDir = e1.cross(e2).toUnit();
    n0 = planeDir;
    n1 = planeDir;
    n2 = planeDir;
    t0 = TexCoords();
    t1 = TexCoords();
    t2 = TexCoords();
    this->m = m;
    texture = -1;
}

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2, Vector3 n0, Vector3 n1, Vector3 n2, int m) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    this->n0 = n0;
    this->n1 = n1;
    this->n2 = n2;
    t0 = TexCoords();
    t1 = TexCoords();
    t2 = TexCoords();
    this->m = m;
    texture = -1;
}

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2, TexCoords t0, TexCoords t1, TexCoords t2, int m, int texture) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    Vector3 planeDir = e1.cross(e2).toUnit();
    n0 = planeDir;
    n1 = planeDir;
    n2 = planeDir;
    this->t0 = t0;
    this->t1 = t1;
    this->t2 = t2;
    this->m = m;
    this->texture = texture;
}

Triangle::Triangle(Point3 v0, Point3 v1, Point3 v2, Vector3 n0, Vector3 n1, Vector3 n2, TexCoords t0, TexCoords t1, TexCoords t2, int m, int texture) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
    this->n0 = n0;
    this->n1 = n1;
    this->n2 = n2;
    this->t0 = t0;
    this->t1 = t1;
    this->t2 = t2;
    this->m = m;
    this->texture = texture;
}

float Triangle::checkIntersect(Ray3 ray) {
    // Get plane equation
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    Vector3 planeDir = e1.cross(e2);
    float d = -1 * (planeDir.x * v0.x + planeDir.y * v0.y + planeDir.z * v0.z);
    if(planeDir.x * ray.dir.x + planeDir.y * ray.dir.y + planeDir.z * ray.dir.z == 0) {
        // Plane and ray are parallel
        return -1;
    }
    // Find point of intersection with the plane
    float t = -1 * (planeDir.x * ray.p.x + planeDir.y * ray.p.y + planeDir.z * ray.p.z + d) /
              (planeDir.x * ray.dir.x + planeDir.y * ray.dir.y + planeDir.z * ray.dir.z);
    Point3 p = ray.p + ray.dir * t;
    // Check if the point of intersection is in the triangle
    float area = 0.5 * planeDir.length();
    float alpha = 0.5 * (v1 - p).cross(v2 - p).length() / area;
    float beta = 0.5 * (v2 - p).cross(v0 - p).length() / area;
    float gamma = 0.5 * (v0 - p).cross(v1 - p).length() / area;
    if(alpha >= 0 && alpha <= 1.001 && beta >= 0 && beta <= 1.001 && gamma >= 0 && gamma <= 1.001 && alpha + beta + gamma <= 1.001) {
        return t;
    }
    return -1;
}

Vector3 Triangle::getNormal(Point3 p) {
    // Obtain barycentric coords of p
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    float area = 0.5 * e1.cross(e2).length();
    float alpha = 0.5 * (v1 - p).cross(v2 - p).length() / area;
    float beta = 0.5 * (v2 - p).cross(v0 - p).length() / area;
    float gamma = 0.5 * (v0 - p).cross(v1 - p).length() / area;

    // Return interpolated normal
    return (n0 * alpha + n1 * beta + n2 * gamma).toUnit();
}

TexCoords Triangle::getTexCoords(Point3 p) {
    // Obtain barycentric coords of p
    Vector3 e1 = v1 - v0;
    Vector3 e2 = v2 - v0;
    float area = 0.5 * e1.cross(e2).length();
    float alpha = 0.5 * (v1 - p).cross(v2 - p).length() / area;
    float beta = 0.5 * (v2 - p).cross(v0 - p).length() / area;
    float gamma = 0.5 * (v0 - p).cross(v1 - p).length() / area;

    // Use interpolation to get texcoords
    float u = alpha * t0.u + beta * t1.u + gamma * t2.u;
    float v = alpha * t0.v + beta * t1.v + gamma * t2.v;
    return TexCoords(u, v);
}