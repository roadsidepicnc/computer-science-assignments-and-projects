#ifndef MESH_HPP
#define MESH_HPP

#include "Vector3.hpp"
#include "Triangle.hpp"
#include "Material.hpp"

namespace cse461_hw1
{
	class Mesh
	{
		public:
		
		int id;
		vector<Triangle> faces;
		int materialId;
		
		Mesh(int Id, int MaterialId, vector<Triangle> Faces) : id(Id), materialId(MaterialId)	
		{
			faces.clear();
 			for(int i = 0; i < Faces.size(); i++)
			{
				faces.push_back(Faces[i]);
			}
		}
	};
}

#endif