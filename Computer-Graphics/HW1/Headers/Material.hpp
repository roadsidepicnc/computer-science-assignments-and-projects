#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vector3.hpp"

using namespace std;

namespace cse461_hw1
{
	class Material
	{
		public:
		
		int id;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		Vector3 mirrorReflectance;
		float phongExponent;
		
		Material(int Id, Vector3 Ambient, Vector3 Diffuse, Vector3 Specular, Vector3 MirrorReflectance, float PhongExponent) : id(Id), ambient(Ambient), diffuse(Diffuse), specular(Specular), mirrorReflectance(MirrorReflectance), phongExponent(PhongExponent)
 		{	
		}	
	};

	std::ostream& operator<<(std::ostream &out, const Material &m)
	{
    	return out << "Material:" << m.id << " \n"
    	<< "Ambient: " << m.ambient << "\n"
    	<< "Diffuse: " << m.diffuse << "\n"
    	<< "Specular: " <<m.specular << "\n"
    	<< "Mirror Refelectance: " << m.mirrorReflectance << "\n"
    	<< "Phong Exponent: " << m.phongExponent;
	}
}

#endif