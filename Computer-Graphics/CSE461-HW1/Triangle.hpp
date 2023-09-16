#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include "Vector3.hpp"
#include "Object.hpp"

using namespace std;

class Triangle : public Object
{
	public:
		
	Vector3 indices;
		
	Triangle(Material ObjectMaterial, ObjectType Type, Vector3 Indices) : Object(ObjectMaterial, Type), indices(Indices)
	{
	}
};

#endif