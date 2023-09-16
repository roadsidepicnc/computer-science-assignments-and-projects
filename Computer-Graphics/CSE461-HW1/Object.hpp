#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iostream>
#include "Vector3.hpp"
#include "Material.hpp"

using namespace std;

enum ObjectType
{   
	mesh = 0,
	triangle = 1,
	sphere = 2
};

class Object
{
	public:
		
	Material material;
	ObjectType type;
		
	Object(Material ObjectMaterial, ObjectType Type) : material(ObjectMaterial), type(Type) 	
 	{	
	}
};

#endif