#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <iostream>
#include "Vector3.hpp"

using namespace std;

class Camera
{
	public:
		
	Vector3 position, gaze, up;
	float nearPlane[4];
	float nearDistance;
	float imageResolution[2];
	string imageName;	
		
	Camera(Vector3 Pos, Vector3 Gaze, Vector3 Up, float NearPlane[4], float NearDistance, float ImageResolution[2], string ImageName) : position(Pos), gaze(Gaze), up(Up), nearDistance(NearDistance), imageName(ImageName)
	{
		for(int i = 0; i < 4; i++)
		{
			nearPlane[i] = NearPlane[i];
		}
		for(int i = 0; i < 2; i++)
		{
			imageResolution[i] = ImageResolution[i];
		}	
	}		
};

#endif