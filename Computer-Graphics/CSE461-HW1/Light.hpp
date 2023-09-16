#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <iostream>
#include "Vector3.hpp"

using namespace std;

class Light
{
	public:
		
	Vector3 position;
	Vector3 intensity;
		
	Light(Vector3 Position, Vector3 Intensity) : position(Position), intensity(Intensity) 	
 	{	
	}
};

#endif