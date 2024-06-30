#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream>

using std::sqrt;
using std::ostream;

namespace cse461_hw1
{
	enum LightType
	{   
		point_light = 0,
		triangular_light = 1
	};
	
	class Vector3 
	{
		public:

			double values[3];

			Vector3() 
			{ 
				values[0] = 0; 
				values[1] = 0; 
				values[2] = 0; 
			}

			Vector3(double value1, double value2, double value3)
        	{
            	values[0] = value1;
            	values[1] = value2;
            	values[2] = value3;
        	}

        	double& operator[](int index) 
        	{ 
        		return values[index]; 
        	}

        	Vector3& operator+=(const Vector3 &secondVector) 
			{
            	values[0] += secondVector.values[0];
            	values[1] += secondVector.values[1];
            	values[2] += secondVector.values[2];
            	return *this;
        	}

        	Vector3& operator-=(const Vector3 &secondVector) 
			{
            	values[0] -= secondVector.values[0];
            	values[1] -= secondVector.values[1];
            	values[2] -= secondVector.values[2];
            	return *this;
        	}

        	Vector3 operator-() const 
        	{ 
        		return Vector3(-values[0], -values[1], -values[2]); 
        	}

        	Vector3& operator*=(const double multiplier) 
			{
            	values[0] *= multiplier;
            	values[1] *= multiplier;
            	values[2] *= multiplier;
            	return *this;
        	}

        	Vector3& operator/=(const double multiplier) 
			{
            	values[0] /= multiplier;
            	values[1] /= multiplier;
            	values[2] /= multiplier;
            	return *this;
        	}

        	double length() const 
			{
            	return sqrt(length_squared());
        	}

        	double length_squared() const 
			{
            	return values[0] * values[0] + values[1] * values[1] + values[2] * values[2];
        	}
	};

	ostream& operator<<(ostream &out, const Vector3 &vec)
	{
    	return out << vec.values[0] << ' ' << vec.values[1] << ' ' << vec.values[2];
	}

	Vector3 operator+(const Vector3 &vec1, const Vector3 &vec2) 
	{
    	return Vector3(vec1.values[0] + vec2.values[0], vec1.values[1] + vec2.values[1], vec1.values[2] + vec2.values[2]);
	}

	Vector3 operator-(const Vector3 &vec1, const Vector3 &vec2) 
	{
    	return Vector3(vec1.values[0] - vec2.values[0], vec1.values[1] - vec2.values[1], vec1.values[2] - vec2.values[2]);
	}

	Vector3 operator*(const Vector3 &vec1, const Vector3 &vec2) 
	{
	    return Vector3(vec1.values[0] * vec2.values[0], vec1.values[1] * vec2.values[1], vec1.values[2] * vec2.values[2]);
	}

	Vector3 operator*(double multiplier, const Vector3 &vec)
	{
	    return Vector3(multiplier * vec.values[0], multiplier * vec.values[1], multiplier * vec.values[2]);
	}
	
	Vector3 operator*(const Vector3 &vec, double multiplier) 
	{
	    return multiplier * vec;
	}

	Vector3 operator/(Vector3 vec, double multiplier) 
	{
	    return (1 / multiplier) * vec;
	}

	/////////////////////////////////////////

	Vector3 unit_vector(Vector3 vec) 
	{
    	return vec / vec.length();
	}

	Vector3 cross(const Vector3 &vec1, const Vector3 &vec2)
	{
    	return Vector3(vec1.values[1] * vec2.values[2] - vec1.values[2] * vec2.values[1], vec1.values[2] * vec2.values[0] - vec1.values[0] * vec2.values[2], vec1.values[0] * vec2.values[1] - vec1.values[1] * vec2.values[0]);
	}

	double dot(const Vector3 &vec1, const Vector3 &vec2)
	{
    	return vec1.values[0] * vec2.values[0] + vec1.values[1] * vec2.values[1] + vec1.values[2] * vec2.values[2];
	}
}

#endif