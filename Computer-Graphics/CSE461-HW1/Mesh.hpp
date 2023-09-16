#ifndef MESH_HPP
#define MESH_HPP

#include <iostream>
#include "Vector3.hpp"
#include "Object.hpp"
#include "Triangle.hpp"

using namespace std;

class Mesh : public Object
{
	public:
		
	vector<Triangle> faces;
		
	Mesh(Material ObjectMaterial, ObjectType Type, vector<Vector3> Faces) : Object(ObjectMaterial, Type)	
	{
		faces.clear();
 		for(int i = 0; i < Faces.size(); i++)
		{
			faces.push_back(Triangle(ObjectMaterial, triangle, Faces[i]));
		}
	}
};

#endif