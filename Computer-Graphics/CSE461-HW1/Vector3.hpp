#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

using std::sqrt;

class Vector3 
{
    public:
    
        Vector3() { e[0] = 0; e[1] = 0; e[2] = 0; }
    
        Vector3(double e0, double e1, double e2)
        {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
        }
    
        void Set(double e0, double e1, double e2){ Vector3(e0, e1, e2); }

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        Vector3& operator+=(const Vector3 &v) 
		{
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vector3& operator*=(const double t) 
		{
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vector3& operator/=(const double t) 
		{
            return *this *= 1/t;
        }

        double length() const 
		{
            return sqrt(length_squared());
        }

        double length_squared() const 
		{
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        
    public:
        double e[3];
        
};

std::ostream& operator<<(std::ostream &out, const Vector3 &v)
{
    	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
	
Vector3 operator+(const Vector3 &u, const Vector3 &v) 
{
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

Vector3 operator-(const Vector3 &u, const Vector3 &v) 
{
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

Vector3 operator*(const Vector3 &u, const Vector3 &v) 
{
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

Vector3 operator*(double t, const Vector3 &v) 
{
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

Vector3 operator*(const Vector3 &v, double t) 
{
    return t * v;
}

Vector3 operator/(Vector3 v, double t) 
{
    return (1/t) * v;
}

using point3 = Vector3;
using color = Vector3;

#endif