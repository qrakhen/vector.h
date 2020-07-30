#pragma once

#define PI 3.14159265358979323846

using namespace std;

struct t_vec2 {
    float x;
    float y;
};

struct t_vec3 {
    float x;
    float y;
    float z;
};

struct t_vec4 {
    float x;
    float y;
    float z;
    float w;
};

class Vector {
public:
    float x, y, z, w;

    const Vector* __X = new Vector(1, 0, 0, 0);
    const Vector* __Y = new Vector(1, 0, 0, 0);
    const Vector* __Z = new Vector(1, 0, 0, 0);
    const Vector* __W = new Vector(1, 0, 0, 0);
    const Vector* NUL = new Vector(0, 0, 0, 0);
    const Vector* ONE = new Vector(1, 0, 0, 0);

    Vector(
            float x = 0.f, 
            float y = 0.f, 
            float z = 0.f, 
            float w = 0.f) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    ~Vector() {
        free(this);
    }

    //Vector(Vector&& v) : Vector(v.x, v.y, v.z, v.w) { }
    //Vector(const Vector& v) : Vector(v.x, v.y, v.z, v.w) { }
    Vector operator =(Vector v) { return v.clone(); }

    Vector(t_vec2 v) : Vector(v.x, v.y) { }
    Vector(t_vec3 v) : Vector(v.x, v.y, v.z) { }
    Vector(t_vec4 v) : Vector(v.x, v.y, v.z, v.w) { }
    t_vec2 asStruct2() { return t_vec2 { x, y }; }
    t_vec3 asStruct3() { return t_vec3 { x, y, z }; }
    t_vec4 asStruct4() { return t_vec4 { x, y, z, w }; }

    /// <summary>
    /// distance to given vector 
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    float distance(Vector v) {
        return delta(v).length();
    }

    /// <summary>
    /// since we live in the normal world, this is euclidian and ignores W.
    /// </summary>
    /// <returns></returns>
    float length() {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    /// <summary>
    /// delta between this and the given vector (v2 - v1)
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector delta(Vector v) {
        return Vector(v.x - x, v.y - y, v.z - z, v.w - w);
    }

    /// <summary>
    /// v1 + v2
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector add(Vector v) {
        return Vector(
            x + v.x,
            y + v.y,
            z + v.z,
            w + v.w);
    }
    Vector operator +(Vector v) { return add(v); }

    /// <summary>
    /// v1 - v2
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector subtract(Vector v) {
        return Vector(
            x - v.x,
            y - v.y,
            z - v.z,
            w - v.w);
    }
    Vector operator -(Vector v) { return subtract(v); }

    /// <summary>
    /// v1 / v
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector divide(float v) {
        return Vector(
            x / v,
            y / v,
            z / v,
            w / v);
    }
    Vector operator /(float v) { return divide(v); }

    /// <summary>
    /// v1 / v2 ... i honestly have no idea what you'd need this for
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector divide(Vector v) {
        return Vector(
            x / v.x,
            y / v.y,
            z / v.z,
            w / v.w);
    }
    Vector operator /(Vector v) { return divide(v); }

    /// <summary>
    /// v1 * f
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector scale(float f) {
        return Vector(x * f, y * f, z * f);
    }
    Vector operator *(float v) { return scale(v); }

    /// <summary>
    /// returns the product of two vectors v1 * v2
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Vector product(Vector v) {
        return Vector(
            x * v.x,
            y * v.y,
            z * v.z,
            w * v.w
        );
    }
    Vector operator *(Vector v) { return product(v); }

    /// <summary>
    /// compares v1 == v2. 
    /// </summary>
    /// <param name="v"></param>
    /// <param name="__fpea">floating point error accuracy. will automatically round to the decimal point to prevent floating point bullshit</param>
    /// <returns></returns>
    bool equals(Vector v, float __fpea = .00001) {
        return (
            round(x - v.x, __fpea) == 0.f,
            round(y - v.y, __fpea) == 0.f,
            round(z - v.z, __fpea) == 0.f,
            round(w - v.w, __fpea) == 0.f);
    }
    //bool operator ==(Vector v) { return equals(v); }

    /// <summary>
    /// rounds floating point value.
    /// </summary>
    /// <param name="v"></param>
    /// <param name="d">precision, 1 means will round to 1.f, 100 means 100, .01f means 1.00f. 0.f is forbidden by law</param>
    /// <returns></returns>
    static float round(float v, float d = 1) {
        if (d == 0)
            d = 1;
        return (float)((int)(v * (1 / d) + .5f)) / (1 / d);
    }

    /// <summary>
    /// clones this vector
    /// </summary>
    /// <returns></returns>
    Vector clone() {
        return Vector(x, y, z, w);
    }

    /// <summary>
    /// normalizes this vector with a given factor.
    /// default 1 is, making the power (length) of it 1 and adjusting the sides
    /// </summary>
    /// <param name="factor"></param>
    /// <returns></returns>
    Vector normalize(float factor = 1.f) {
        float len = length();
        return Vector(
            x *= (factor / len),
            y *= (factor / len),
            z *= (factor / len),
            w *= (factor / len));
    }

    /// <summary>
    /// calculates the angle on every axis towards v.
    /// example: Vector(30, 90, 0).angle(Vector(0, 180, 0), Vector(1, 0, 0), Vector(0, 1, 0)) 
    /// would result in Vector(-30, 90, 0), as those are the relative rotations on every axis.
    /// keep in mind that the direction vectors right and up must never have a power greater than 1. 
    /// you can not have "right" into two direction at the same time, and right also cant be "two times as right as one".
    /// 
    /// note: forward rotation is intentionally ignored to prevent gimbal lock. will implement quaternions soon.
    /// </summary>
    /// <param name="v">vector to rotate to</param>
    /// <param name="right">vector that defines where 'right' is. can be (1, 0, 0), or for some engines even Vector(0, -1, 0)</param>
    /// <param name="up">vector that defines where 'up' is. can be (0, 1, 0) or anything else</param>
    /// <param name="fwd">vector that defines where 'foward' is. can be (0, 0, 1) or anything else</param>
    /// <returns></returns>
    Vector angle(
            Vector v, 
            Vector right =   Vector(0, 0, 1),
            Vector up =      Vector(0, 1, 0),
            Vector fwd =     Vector(1, 0, 0)) {
        Vector d = delta(v);
        float
            rUp = asinf(d.getAxis(right) / d.length()),
            rRight = atanf(d.getAxis(up) / d.getAxis(fwd)),
            aFwd = 0.f,
            ax = (right.getAxis(*__X) * rRight) + (up.getAxis(*__X) * rUp),
            ay = (right.getAxis(*__Y) * rRight) + (up.getAxis(*__Y) * rUp),
            az = (right.getAxis(*__Z) * rRight) + (up.getAxis(*__Z) * rUp);
        return Vector(ax, ay, az);
    }

    /// <summary>
    /// returns the value of the according direction of this vector.
    /// example: (12, 32, 72).getAxis((0, -1, 0)) would result in -32.
    /// this is essentially the length of the product of v1 * v2
    /// this is needed for most dynamic calculations due to every engine having its own preferences.
    /// </summary>
    /// <param name="direction"></param>
    /// <returns></returns>
    float getAxis(Vector v) {
        return product(v).length();
    }

    string toString() {
        return "(" +
            to_string(round(x)) + "|" + 
            to_string(round(y)) + "|" + 
            to_string(round(z)) + ":" + 
            to_string(round(w)) + ")";
    }

    bool isNull() { return x == y == z == w == 0.f; }
    operator bool() { return !isNull(); }
    void nullify() { x = y = z = w = 0.f;  }
};
