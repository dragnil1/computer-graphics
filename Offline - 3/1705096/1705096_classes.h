#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

#include <windows.h>
#include <GL/glut.h>

using namespace std;

#define PI (2 * acos(0.0))
#define INF 1e9
#define EVIL_EPSILON 1e-9

#endif

struct point
{
    double x, y, z;
};

class Vector3D
{
    double x;
    double y;
    double z;

public:
    Vector3D()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    Vector3D(point p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }
    Vector3D(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3D(const Vector3D &v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    Vector3D operator+(const Vector3D &v)
    {
        Vector3D temp;
        temp.x = this->x + v.x;
        temp.y = this->y + v.y;
        temp.z = this->z + v.z;
        return temp;
    }
    Vector3D operator-(const Vector3D &v)
    {
        Vector3D temp;
        temp.x = this->x - v.x;
        temp.y = this->y - v.y;
        temp.z = this->z - v.z;
        return temp;
    }
    Vector3D operator*(const double &d)
    {
        Vector3D temp;
        temp.x = this->x * d;
        temp.y = this->y * d;
        temp.z = this->z * d;
        return temp;
    }
    double operator*(const Vector3D &v)
    {
        Vector3D temp;
        temp.x = this->x * v.x;
        temp.y = this->y * v.y;
        temp.z = this->z * v.z;
        return temp.x + temp.y + temp.z;
    }
    Vector3D operator^(const Vector3D &v)
    {
        Vector3D temp;
        temp.x = this->y * v.z - this->z * v.y;
        temp.y = this->z * v.x - this->x * v.z;
        temp.z = this->x * v.y - this->y * v.x;
        return temp;
    }
    double getX()
    {
        return x;
    }
    double getY()
    {
        return y;
    }
    double getZ()
    {
        return z;
    }
    void setX(double x)
    {
        this->x = x;
    }
    void setY(double y)
    {
        this->y = y;
    }
    void setZ(double z)
    {
        this->z = z;
    }
    Vector3D normalize()
    {
        double magnitude = sqrt(x * x + y * y + z * z);
        return Vector3D(x / magnitude, y / magnitude, z / magnitude);
    }
    double getDistance(Vector3D point)
    {
        return sqrt((x - point.x) * (x - point.x) + (y - point.y) * (y - point.y) + (z - point.z) * (z - point.z));
    }
    ~Vector3D()
    {
    }
};

class Ray
{
    Vector3D start;
    Vector3D direction;

public:
    Ray()
    {
        start = Vector3D();
        direction = Vector3D();
    }
    Ray(Vector3D start, Vector3D direction)
    {
        this->start = start;
        this->direction = direction.normalize();
    }
    /*Ray(Vector3D start, Vector3D end)
    {
        this->start = start;
        this->direction = (end - start).normalize();
    }*/
    Vector3D getStart()
    {
        return start;
    }
    Vector3D getDirection()
    {
        return direction;
    }
    ~Ray()
    {
    }
};

class Color
{
public:
    double r, g, b;
    Color()
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
    }
    Color(double r, double g, double b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Color operator*(const double &d)
    {
        Color temp;
        temp.r = this->r * d;
        temp.g = this->g * d;
        temp.b = this->b * d;
        return temp;
    }
    Color operator+(const Color &c)
    {
        Color temp;
        temp.r = this->r + c.r;
        temp.g = this->g + c.g;
        temp.b = this->b + c.b;
        return temp;
    }
    void operator+=(const Color &c)
    {
        this->r += c.r;
        this->g += c.g;
        this->b += c.b;
    }
    Color operator*(const Color &c)
    {
        Color temp;
        temp.r = this->r * c.r;
        temp.g = this->g * c.g;
        temp.b = this->b * c.b;
        return temp;
    }
    void clip()
    {
        if (r > 1)
            r = 1;
        if (g > 1)
            g = 1;
        if (b > 1)
            b = 1;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
    }
    ~Color()
    {
    }
};

class CoEfficients
{
public:
    double ambient, diffuse, specular, reflection;
    CoEfficients()
    {
        ambient = 0.0;
        diffuse = 0.0;
        specular = 0.0;
        reflection = 0.0;
    }
    CoEfficients(double ambient, double diffuse, double specular, double reflection)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
    }
    ~CoEfficients()
    {
    }
};

class Object
{
protected:
    Vector3D reference_point;
    double height, width, length;
    Color color;
    CoEfficients coEfficients;
    int shine;

public:
    Object() {}
    virtual void draw()
    {
        // cout << "Drawing" << endl;
    }
    void setReferencePoint(Vector3D reference_point)
    {
        this->reference_point = reference_point;
    }
    void setHeight(double height)
    {
        this->height = height;
    }
    void setWidth(double width)
    {
        this->width = width;
    }
    void setLength(double length)
    {
        this->length = length;
    }
    void setColor(Color color)
    {
        this->color = color;
    }
    void setColor(double r, double g, double b)
    {
        this->color = Color(r, g, b);
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setCoEfficients(double ambient, double diffuse, double specular, double reflection)
    {
        this->coEfficients = CoEfficients(ambient, diffuse, specular, reflection);
    }
    Color getColor()
    {
        return color;
    }
    CoEfficients getCoEfficients()
    {
        return coEfficients;
    }
    int getShine()
    {
        return shine;
    }
    virtual double intersect(Ray r, Color &color, int level) { return -1; }
    virtual Vector3D getNormal(Ray r, Vector3D intersectionPoint) {}
    virtual ~Object() {}

};

class PointLight
{
private:
    Vector3D light_pos;
    Color color;

public:
    PointLight()
    {
        light_pos = Vector3D(0.0, 0.0, 0.0);
        color = Color(0, 0, 0);
    }
    PointLight(Vector3D light_pos, double r, double g, double b)
    {
        this->light_pos = light_pos;
        this->color = Color(r, g, b);
    }
    void setLightPos(Vector3D pos)
    {
        light_pos = pos;
    }
    void setColor(double r, double g, double b)
    {
        color = Color(r, g, b);
    }
    Vector3D getLightPos()
    {
        return light_pos;
    }
    Color getColor()
    {
        return color;
    }
    void draw()
    {
        glPushMatrix();
        {
            glTranslatef(light_pos.getX(), light_pos.getY(), light_pos.getZ());
            glColor3f(color.r, color.g, color.b);
            glutSolidSphere(1, 10, 10);
        }
        glPopMatrix();
    }
    ~PointLight()
    {
    }
};

class SpotLight
{
private:
    Vector3D light_pos;
    Color color;
    Vector3D light_dir;
    double angle;

public:
    SpotLight()
    {
        light_pos = Vector3D(0.0, 0.0, 0.0);
        color = Color(0, 0, 0);
        light_dir = Vector3D(0.0, 0.0, 0.0);
        angle = 0.0;
    }
    SpotLight(Vector3D light_pos, double r, double g, double b, Vector3D light_dir, double angle)
    {
        this->light_pos = light_pos;
        this->color = Color(r, g, b);
        this->light_dir = light_dir.normalize();
        this->angle = angle;
    }
    void setLightDir(Vector3D light_dir)
    {
        this->light_dir = light_dir;
    }
    Vector3D getLightDir()
    {
        return light_dir;
    }
    void setAngle(double angle)
    {
        this->angle = angle;
    }
    double getAngle()
    {
        return angle;
    }
    Vector3D getLightPos()
    {
        return light_pos;
    }
    Color getColor()
    {
        return color;
    }
    void draw()
    {
        glPushMatrix();
        {
            glTranslatef(light_pos.getX(), light_pos.getY(), light_pos.getZ());
            glColor3f(color.r, color.g, color.b);
            glutSolidSphere(1, 10, 10);
        }
        glPopMatrix();
    }
    ~SpotLight()
    {
    }
};

extern vector<Object *> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;
extern int level_of_recursion;

class Sphere : public Object
{
private:
    Vector3D center;
    double radius;
    int slices;
    int stacks;

public:
    Sphere(Vector3D center, double radious)
    {
        this->center = center;
        this->radius = radious;
        slices = 24;
        stacks = 30;
    }
    Sphere(Vector3D center, double radius, int slices, int stacks)
    {
        this->center = center;
        this->radius = radius;
        this->slices = slices;
        this->stacks = stacks;
    }
    void setCenter(Vector3D center)
    {
        this->center = center;
    }
    void setRadius(double radius)
    {
        this->radius = radius;
    }
    void setStacks(int stacks)
    {
        this->stacks = stacks;
    }
    void setSlices(int slices)
    {
        this->slices = slices;
    }
    void draw()
    {
        /// cout << "Drawing Sphere" << endl;
        Vector3D points[stacks + 1][stacks + 1];
        int i, j;
        double h, r;
        // generate points
        for (i = 0; i <= stacks; i++)
        {
            h = radius * sin(((double)i / (double)stacks) * (PI / 2));
            r = radius * cos(((double)i / (double)stacks) * (PI / 2));
            for (j = 0; j <= slices; j++)
            {
                points[i][j].setX(r * cos(((double)j / (double)slices) * 2 * PI));
                points[i][j].setY(r * sin(((double)j / (double)slices) * 2 * PI));
                points[i][j].setZ(h);
                // points[i][j] = points[i][j] + center;
            }
        }
        // draw quads using generated points
        for (i = 0; i < stacks; i++)
        {
            // glColor3f((double)i / (double)stacks, (double)i / (double)stacks, (double)i / (double)stacks);
            glColor3f(color.r, color.g, color.b);
            for (j = 0; j < slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    /* upper hemisphere */
                    glVertex3f((center + points[i][j]).getX(), (center + points[i][j]).getY(), (center + points[i][j]).getZ());
                    glVertex3f((center + points[i][j + 1]).getX(), (center + points[i][j + 1]).getY(), (center + points[i][j + 1]).getZ());
                    glVertex3f((center + points[i + 1][j + 1]).getX(), (center + points[i + 1][j + 1]).getY(), (center + points[i + 1][j + 1]).getZ());
                    glVertex3f((center + points[i + 1][j]).getX(), (center + points[i + 1][j]).getY(), (center + points[i + 1][j]).getZ());

                    /* lower hemisphere */
                    glVertex3f((center + points[i][j]).getX(), (center + points[i][j]).getY(), (center - points[i][j]).getZ());
                    glVertex3f((center + points[i][j + 1]).getX(), (center + points[i][j + 1]).getY(), (center - points[i][j + 1]).getZ());
                    glVertex3f((center + points[i + 1][j + 1]).getX(), (center + points[i + 1][j + 1]).getY(), (center - points[i + 1][j + 1]).getZ());
                    glVertex3f((center + points[i + 1][j]).getX(), (center + points[i + 1][j]).getY(), (center - points[i + 1][j]).getZ());
                }
                glEnd();
            }
        }
    }
    void illuminateWithPhong(Ray r, Color &color, double t_min, Color intersectionPointColor)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        color = intersectionPointColor * coEfficients.ambient;
        // Vector3D normal = getNormal(r, intersectionPoint);
        for (auto pl : pointLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r1, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1.00)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
        }
        for (auto pl : spotLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double angle = acos(r1.getDirection() * (pl.getLightDir())) * (180 / PI);
            //angle %= 360;
            if (!(angle >= -pl.getAngle() && angle <= pl.getAngle()))
            {
                continue;
            }
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
        }
    }
    void recursiveReflection(Ray r, Color &color, double t_min, int level)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        Vector3D normal = getNormal(r, intersectionPoint);
        Vector3D reflectionDirection = (r.getDirection() - normal * (r.getDirection() * normal) * 2).normalize();
        Ray reflectionRay(intersectionPoint + reflectionDirection, reflectionDirection);
        double t, tMin;
        Color dummyColor = Color(0, 0, 0);
        Color colorReflected = Color(0, 0, 0);
        Object *o_n;
        t = INF;
        bool updated = false;
        for (auto o : objects)
        {
            double tTemp = o->intersect(reflectionRay, dummyColor, 0);
            if (tTemp > 0 && tTemp < t)
            {
                t = tTemp;
                o_n = o;
                updated = true;
            }
        }
        if (updated)
        {
            tMin = o_n->intersect(reflectionRay, colorReflected, level + 1);
            color += colorReflected * coEfficients.reflection;
        }
        color.clip();
    }
    double intersect(Ray r, Color &color, int level)
    {
        /// cout << "In sphere intersect" << endl;
        Vector3D R_0 = r.getStart() - center;
        Vector3D R_d = r.getDirection();
        double a = 1.0;
        double b = 2 * (R_0 * R_d);
        double c = (R_0 * R_0) - (radius * radius);
        double delta = (b * b) - (4 * a * c);
        double t_min = -1;
        if (delta >= 0)
        {
            double t_pos = (-b + sqrt(delta)) / (2 * a);
            double t_neg = (-b - sqrt(delta)) / (2 * a);
            if (t_neg > 0)
            {
                t_min = t_neg;
            }
            else if (t_pos > 0)
            {
                t_min = t_pos;
            }
        }
        if (level == 0)
        {
            return t_min;
        }
        illuminateWithPhong(r, color, t_min, this->color);
        if (level >= level_of_recursion)
        {
            return t_min;
        }
        recursiveReflection(r, color, t_min, level);
        return t_min;
    }
    Vector3D getNormal(Ray r, Vector3D intersectionPoint)
    {
        Vector3D normal = (intersectionPoint - center).normalize();
        normal = (normal * r.getDirection() <= 0) ? normal : normal * (-1);
        return normal;
    }
    ~Sphere()
    {
    }
};

class Triangle : public Object
{
private:
    Vector3D p1, p2, p3; // 3 points of the triangle
    bool checkInside(Vector3D p)
    {
        double maxX = max(p1.getX(), max(p2.getX(), p3.getX()));
        double minX = min(p1.getX(), min(p2.getX(), p3.getX()));
        double maxY = max(p1.getY(), max(p2.getY(), p3.getY()));
        double minY = min(p1.getY(), min(p2.getY(), p3.getY()));
        double maxZ = max(p1.getZ(), max(p2.getZ(), p3.getZ()));
        double minZ = min(p1.getZ(), min(p2.getZ(), p3.getZ()));
        return (p.getX() > minX && p.getX() < maxX && p.getY() > minY && p.getY() < maxY && p.getZ() > minZ && p.getZ() < maxZ);
    }

public:
    Triangle(Vector3D p1, Vector3D p2, Vector3D p3)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
    void setPoint1(Vector3D p1)
    {
        this->p1 = p1;
    }
    void setPoint2(Vector3D p2)
    {
        this->p2 = p2;
    }
    void setPoint3(Vector3D p3)
    {
        this->p3 = p3;
    }
    Vector3D getPoint1()
    {
        return p1;
    }
    Vector3D getPoint2()
    {
        return p2;
    }
    Vector3D getPoint3()
    {
        return p3;
    }
    void draw()
    {
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(p1.getX(), p1.getY(), p1.getZ());
            glVertex3f(p2.getX(), p2.getY(), p2.getZ());
            glVertex3f(p3.getX(), p3.getY(), p3.getZ());
        }
        glEnd();
    }
    void illuminateWithPhong(Ray r, Color &color, double t_min, Color intersectionPointColor)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        color = intersectionPointColor * coEfficients.ambient;
        // Vector3D normal = getNormal(r, intersectionPoint);
        for (auto pl : pointLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1.00)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
            
        }
        for (auto pl : spotLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double angle = acos(r1.getDirection() * (pl.getLightDir())) * (180 / PI);
            //angle %= 360;
            if (!(angle >= -pl.getAngle() && angle <= pl.getAngle()))
            {
                continue;
            }
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
        }
    }
    void recursiveReflection(Ray r, Color &color, double t_min, int level)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        Vector3D normal = getNormal(r, intersectionPoint);
        Vector3D reflectionDirection = (r.getDirection() - normal * (r.getDirection() * normal) * 2).normalize();
        Ray reflectionRay(intersectionPoint + reflectionDirection, reflectionDirection);
        double t, tMin;
        Color dummyColor = Color(0, 0, 0);
        Color colorReflected = Color(0, 0, 0);
        Object *o_n;
        t = INF;
        bool updated = false;
        for (auto o : objects)
        {
            double tTemp = o->intersect(reflectionRay, dummyColor, 0);
            if (tTemp > 0 && tTemp < t)
            {
                t = tTemp;
                o_n = o;
                updated = true;
            }
        }
        if (updated)
        {
            tMin = o_n->intersect(reflectionRay, colorReflected, level + 1);
            color += colorReflected * coEfficients.reflection;
        }
        color.clip();
    }
    double intersect(Ray r, Color &color, int level)
    {
        double determinantBase, determinantBeta, determinantGamma, determinantT, t_min;

        determinantBase = (p1.getX() - p2.getX()) * ((p1.getY() - p3.getY()) * r.getDirection().getZ() - (p1.getZ() - p3.getZ()) * r.getDirection().getY());
        determinantBase += (p1.getX() - p3.getX()) * ((p1.getZ() - p2.getZ()) * r.getDirection().getY() - (p1.getY() - p2.getY()) * r.getDirection().getZ());
        determinantBase +=r.getDirection().getX() * ((p1.getY() - p2.getY()) * (p1.getZ() - p3.getZ()) - (p1.getZ() - p2.getZ()) * (p1.getY() - p3.getY()));

        determinantBeta = (p1.getX() - r.getStart().getX()) * ((p1.getY() - p3.getY()) * r.getDirection().getZ() - (p1.getZ() - p3.getZ()) * r.getDirection().getY());
        determinantBeta += (p1.getX() - p3.getX()) * ((p1.getZ() - r.getStart().getZ()) * r.getDirection().getY() - (p1.getY() - r.getStart().getY()) * r.getDirection().getZ());
        determinantBeta += r.getDirection().getX() * ((p1.getY() - r.getStart().getY()) * (p1.getZ() - p3.getZ()) - (p1.getZ() - r.getStart().getZ()) * (p1.getY() - p3.getY()));

        determinantGamma = (p1.getX() - p2.getX()) * ((p1.getY() - r.getStart().getY()) * r.getDirection().getZ() - (p1.getZ() - r.getStart().getZ()) * r.getDirection().getY());
        determinantGamma += (p1.getX() - r.getStart().getX()) * ((p1.getZ() - p2.getZ()) * r.getDirection().getY() - (p1.getY() - p2.getY()) * r.getDirection().getZ());
        determinantGamma += r.getDirection().getX() * ((p1.getY() - p2.getY()) * (p1.getZ() - r.getStart().getZ()) - (p1.getZ() - p2.getZ()) * (p1.getY() - r.getStart().getY()));

        determinantT = (p1.getX() - p2.getX()) * ((p1.getY() - p3.getY()) * (p1.getZ() - r.getStart().getZ()) - (p1.getZ() - p3.getZ()) * (p1.getY() - r.getStart().getY()));
        determinantT += (p1.getX() - p3.getX()) * ((p1.getZ() - p2.getZ()) * (p1.getY() - r.getStart().getY()) - (p1.getY() - p2.getY()) * (p1.getZ() - r.getStart().getZ()));
        determinantT += (p1.getX() - r.getStart().getX()) * ((p1.getY() - p2.getY()) * (p1.getZ() - p3.getZ()) - (p1.getZ() - p2.getZ()) * (p1.getY() - p3.getY()));

        if (determinantBase == 0.0)
        {
            t_min = INF;
        }
        else
        {
            if (determinantBeta / determinantBase > 0.0 && determinantGamma / determinantBase > 0.0 && determinantBeta / determinantBase + determinantGamma / determinantBase < 1.0)
            {
                t_min = determinantT / determinantBase;
            }
            else
            {
                t_min = INF;
            }
        }
        if (level == 0)
        {
            return t_min;
        }
        illuminateWithPhong(r, color, t_min, this->color);
        if (level >= level_of_recursion)
        {
            return t_min;
        }
        recursiveReflection(r, color, t_min, level);
        return t_min;
    }
    Vector3D getNormal(Ray r, Vector3D intersectionPoint)
    {
        Vector3D normal = ((p2 - p1) ^ (p3 - p1)).normalize();
        normal = (normal * r.getDirection() < 0) ? normal : normal * (-1);
        return normal;
    }
    ~Triangle()
    {
    }
};

class GeneralQuadraticEquation : public Object
{
private:
    double A, B, C, D, E, F, G, H, I, J;
    bool checkInside(Vector3D p)
    {
        return (length == 0 || (p.getX() >= reference_point.getX() && p.getX() <= reference_point.getX() + length)) && (width == 0 || (p.getY() >= reference_point.getY() && p.getY() <= reference_point.getY() + width)) && (height == 0.0 || (p.getZ() >= reference_point.getZ() && p.getZ() <= reference_point.getZ() + height));
    }

public:
    GeneralQuadraticEquation(double A, double B, double C, double D, double E, double F,
                             double G, double H, double I, double J)
    {
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
        this->E = E;
        this->F = F;
        this->G = G;
        this->H = H;
        this->I = I;
        this->J = J;
    }
    void setA(double A)
    {
        this->A = A;
    }
    void setB(double B)
    {
        this->B = B;
    }
    void setC(double C)
    {
        this->C = C;
    }
    void setD(double D)
    {
        this->D = D;
    }
    void setE(double E)
    {
        this->E = E;
    }
    void setF(double F)
    {
        this->F = F;
    }
    void setG(double G)
    {
        this->G = G;
    }
    void setH(double H)
    {
        this->H = H;
    }
    void setI(double I)
    {
        this->I = I;
    }
    void setJ(double J)
    {
        this->J = J;
    }
    double getA()
    {
        return A;
    }
    double getB()
    {
        return B;
    }
    double getC()
    {
        return C;
    }
    double getD()
    {
        return D;
    }
    double getE()
    {
        return E;
    }
    double getF()
    {
        return F;
    }
    double getG()
    {
        return G;
    }
    double getH()
    {
        return H;
    }
    double getI()
    {
        return I;
    }
    double getJ()
    {
        return J;
    }
    void draw()
    {
    }
    void illuminateWithPhong(Ray r, Color &color, double t_min, Color intersectionPointColor)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        color = intersectionPointColor * coEfficients.ambient;
        // Vector3D normal = getNormal(r, intersectionPoint);
        for (auto pl : pointLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1.00)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
        }
        for (auto pl : spotLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double angle = acos(r1.getDirection() * (pl.getLightDir())) * (180 / PI);
            //angle %= 360;
            if (!(angle >= -pl.getAngle() && angle <= pl.getAngle()))
            {
                continue;
            }
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), shine) * intersectionPointColor;
        }
    }
    void recursiveReflection(Ray r, Color &color, double t_min, int level)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        Vector3D normal = getNormal(r, intersectionPoint);
        Vector3D reflectionDirection = (r.getDirection() - normal * (r.getDirection() * normal) * 2).normalize();
        Ray reflectionRay(intersectionPoint + reflectionDirection, reflectionDirection);
        double t, tMin;
        Color dummyColor = Color(0, 0, 0);
        Color colorReflected = Color(0, 0, 0);
        Object *o_n;
        t = INF;
        bool updated = false;
        for (auto o : objects)
        {
            double tTemp = o->intersect(reflectionRay, dummyColor, 0);
            if (tTemp > 0 && tTemp < t)
            {
                t = tTemp;
                o_n = o;
                updated = true;
            }
        }
        if (updated)
        {
            tMin = o_n->intersect(reflectionRay, colorReflected, level + 1);
            color += colorReflected * coEfficients.reflection;
        }
        color.clip();
    }
    double intersect(Ray r, Color &color, int level)
    {
        double r_origin_x = r.getStart().getX();
        double r_origin_y = r.getStart().getY();
        double r_origin_z = r.getStart().getZ();
        double r_direction_x = r.getDirection().getX();
        double r_direction_y = r.getDirection().getY();
        double r_direction_z = r.getDirection().getZ();
        double a = A * r_direction_x * r_direction_x + B * r_direction_y * r_direction_y + C * r_direction_z * r_direction_z + D * r_direction_x * r_direction_y + E * r_direction_x * r_direction_z + F * r_direction_y * r_direction_z;
        double b = 2 * A * r_origin_x * r_direction_x + 2 * B * r_origin_y * r_direction_y + 2 * C * r_origin_z * r_direction_z + D * (r_origin_x * r_direction_y + r_origin_y * r_direction_x) + E * (r_origin_x * r_direction_z + r_origin_z * r_direction_x) + F * (r_origin_y * r_direction_z + r_origin_z * r_direction_y) + G * r_direction_x + H * r_direction_y + I * r_direction_z;
        double c = A * r_origin_x * r_origin_x + B * r_origin_y * r_origin_y + C * r_origin_z * r_origin_z + D * r_origin_x * r_origin_y + E * r_origin_x * r_origin_z + F * r_origin_y * r_origin_z + G * r_origin_x + H * r_origin_y + I * r_origin_z + J;
        double delta = b * b - 4 * a * c;
        double t_min;
        if (delta >= 0)
        {
            double t1 = (-b - sqrt(delta)) / (2 * a);
            double t2 = (-b + sqrt(delta)) / (2 * a);
            Vector3D intersectPoint1 = r.getStart() + r.getDirection() * t1;
            Vector3D intersectPoint2 = r.getStart() + r.getDirection() * t2;
            bool check1 = checkInside(intersectPoint1);
            bool check2 = checkInside(intersectPoint2);
            if (check2 && t2 > 0)
            {
                t_min = t2;
            }
            else if (check1 && t1 > 0)
            {
                t_min = t1;
            }
            else
            {
                t_min = INF;
            }
        }
        else
        {
            t_min = INF;
        }
        if (level == 0)
        {
            return t_min;
        }
        illuminateWithPhong(r, color, t_min, this->color);
        if (level >= level_of_recursion)
        {
            return t_min;
        }
        recursiveReflection(r, color, t_min, level);
        return t_min;
    }
    Vector3D getNormal(Ray r, Vector3D intersectionPoint)
    {
        double x = intersectionPoint.getX();
        double y = intersectionPoint.getY();
        double z = intersectionPoint.getZ();
        double a = 2 * A * x + D * y + E * z + G;
        double b = 2 * B * y + D * x + F * z + H;
        double c = 2 * C * z + E * x + F * y + I;
        Vector3D normal = Vector3D(a, b, c).normalize();
        normal = (normal * r.getDirection() <= 0) ? normal : normal * (-1);
        return normal;
    }
    ~GeneralQuadraticEquation()
    {

    }
};

class Floor : public Object
{
private:
    double floorWidth;
    double tileWidth;
    Vector3D floorCenter;
    Vector3D floorNormal;

public:
    Floor(double floorWidth, double tileWidth)
    {
        this->floorWidth = floorWidth;
        this->tileWidth = tileWidth;
        this->floorCenter = Vector3D(0.0, 0.0, 0.0);
        this->floorNormal = Vector3D(0.0, 0.0, 1.0);
    }
    void draw()
    {
        for (double row = floorWidth / 2; row > -floorWidth / 2; row -= tileWidth)
        {
            for (double col = floorWidth / 2; col > -floorWidth / 2; col -= tileWidth)
            {
                glColor3f(color.r, color.g, color.b);
                glBegin(GL_QUADS);
                {
                    glVertex3f(row, col, 0);
                    glVertex3f(row - tileWidth, col, 0);
                    glVertex3f(row - tileWidth, col - tileWidth, 0);
                    glVertex3f(row, col - tileWidth, 0);
                }
                glEnd();
                color.r = 1 - color.r;
                color.g = 1 - color.g;
                color.b = 1 - color.b;
            }
            color.r = 1 - color.r;
            color.g = 1 - color.g;
            color.b = 1 - color.b;
        }
    }
    void illuminateWithPhong(Ray r, Color &color, double t_min, Color intersectionPointColor)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        color = intersectionPointColor * coEfficients.ambient;
        // Vector3D normal = getNormal(r, intersectionPoint);
        for (auto pl : pointLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1.00)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), getShine()) * intersectionPointColor;
        }
        for (auto pl : spotLights)
        {
            Ray r1(pl.getLightPos(), (intersectionPoint - pl.getLightPos()).normalize());
            double angle = acos(r1.getDirection() * (pl.getLightDir())) * (180 / PI);
            ///angle %= 360;
            if (!(angle >= -pl.getAngle() && angle <= pl.getAngle()))
            {
                continue;
            }
            double t, t_min1 = INF;
            bool updated = false;
            Color dummyColor;
            for (auto object : objects)
            {
                t = object->intersect(r1, dummyColor, 0);
                if (t > 0 && t < t_min1)
                {
                    t_min1 = t;
                    updated = true;
                }
            }
            /*if (updated && t_min1 < t_min)
            {
                continue;
            }*/
            Vector3D shadowIntersectionPoint = r1.getStart() + r1.getDirection()*t_min1;
            double epsilon = 0.0000001;  

            if(intersectionPoint.getDistance(r1.getStart()) - epsilon > shadowIntersectionPoint.getDistance(r1.getStart())) {
                continue;
            }
            Vector3D normal = getNormal(r, intersectionPoint);
            double lambertValue = (normal * (r1.getDirection() * (-1)));
            Ray rr(intersectionPoint, (r1.getDirection() - normal * (r1.getDirection() * normal) * 2).normalize());
            double phongValue = (r.getDirection() * (-1)) * rr.getDirection();
            color += pl.getColor() * coEfficients.diffuse * max(lambertValue, 0.0) * intersectionPointColor;
            color += pl.getColor() * coEfficients.specular * pow(max(phongValue, 0.0), getShine()) * intersectionPointColor;
        }
    }
    void recursiveReflection(Ray r, Color &color, double t_min, int level)
    {
        Vector3D intersectionPoint = r.getStart() + r.getDirection() * t_min;
        Vector3D normal = getNormal(r, intersectionPoint);
        Vector3D reflectionDirection = (r.getDirection() - normal * (r.getDirection() * normal) * 2).normalize();
        Ray reflectionRay(intersectionPoint + reflectionDirection, reflectionDirection);
        double t, tMin;
        Color dummyColor = Color(0, 0, 0);
        Color colorReflected = Color(0, 0, 0);
        Object *o_n;
        t = INF;
        bool updated = false;
        for (auto o : objects)
        {
            double tTemp = o->intersect(reflectionRay, dummyColor, 0);
            if (tTemp > 0 && tTemp < t)
            {
                t = tTemp;
                o_n = o;
                updated = true;
            }
        }
        if (updated)
        {
            tMin = o_n->intersect(reflectionRay, colorReflected, level + 1);
            color += colorReflected * coEfficients.reflection;
        }
        color.clip();
    }
    double intersect(Ray r, Color &color, int level)
    {
        double t_min;
        if (floorNormal * r.getDirection() != 0)
            t_min = -(floorNormal * r.getStart()) / (floorNormal * r.getDirection());
        else
            t_min = INF;
        Vector3D intersectPoint = r.getStart() + r.getDirection() * t_min;
        // double distance = (intersectPoint - r.getOrigin()).getLength();
        int i = ((floorWidth / 2) - intersectPoint.getX()) / tileWidth;
        int j = ((floorWidth / 2) - intersectPoint.getY()) / tileWidth;
        Color tempColor;
        int limit_x = floorWidth / tileWidth;
        int limit_y = floorWidth / tileWidth;
        if (i < 0 || i >= limit_x || j < 0 || j >= limit_y)
        {
            tempColor = Color(0, 0, 0);
        }
        else
        {
            if (i % 2 == j % 2)
            {
                tempColor = Color(0, 0, 0);
            }
            else
            {
                tempColor = Color(1, 1, 1);
            }
        }

        /// return t;
        if (level == 0)
        {
            return t_min;
        }
        illuminateWithPhong(r, color, t_min, tempColor);
        if (level > level_of_recursion)
        {
            return t_min;
        }
        recursiveReflection(r, color, t_min, level);
        return t_min;
    }
    Vector3D getNormal(Ray r, Vector3D intersectPoint)
    {
        Vector3D normal = floorNormal;
        normal = (normal * r.getDirection() <= 0) ? normal : normal * (-1);
        return normal;
    }
    ~Floor()
    {
    }
};
