#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <iostream>
#include "Vector3.hpp"
#include "Object.hpp"

using namespace std;

class Sphere : public Object
{
	public:
		
	int center;
	float radius;
		
	Sphere(Material ObjectMaterial, ObjectType Type, int Center, float Radius) : Object(ObjectMaterial, Type), center(Center), radius(Radius)
	{
	}
};

#endif