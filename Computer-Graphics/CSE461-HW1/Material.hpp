#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <iostream>
#include "Vector3.hpp"

using namespace std;

class Material
{
	public:
		
	int id;
	Vector3 ambientReflectance;
	Vector3 diffuseReflectance;
	Vector3 specularReflectance;
	Vector3 mirrorReflectance;
	float phongExponent;
		
	Material(int Id, Vector3 AmbientReflectance, Vector3 DiffuseReflectance, Vector3 SpecularReflectance, Vector3 MirrorReflectance, float PhongExponent) 
	: id(Id), ambientReflectance(AmbientReflectance), diffuseReflectance(DiffuseReflectance), specularReflectance(SpecularReflectance), mirrorReflectance(MirrorReflectance), phongExponent(PhongExponent)
 	{	
	}		
};

#endif