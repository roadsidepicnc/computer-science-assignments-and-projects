#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector3.hpp"

using namespace std;

namespace cse461_hw1
{
	class Camera
	{
		public:
		
		Vector3 position, gaze, up;
		float nearPlane[4];
		float nearDistance;
		float imageResolution[2];
		
		Camera(Vector3 Pos, Vector3 Gaze, Vector3 Up, float NearPlane[4], float NearDistance, float ImageResolution[2]) : position(Pos), gaze(Gaze), up(Up), nearDistance(NearDistance)
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

		Camera()
		{
		}	
	};

	std::ostream& operator<<(std::ostream &out, const Camera &c)
	{
    	return out 
    	<< "Position: " << c.position << "\n"
    	<< "Gaze: " << c.gaze << "\n"
    	<< "Up: " << c.up << "\n"
    	<< "Up: " << c.nearPlane[0] << " " << c.nearPlane[1] << " " << c.nearPlane[2] << " " << c.nearPlane[3] << "\n"
    	<< "Near Distance: " << c.nearDistance << "\n"
    	<< "Up: " << c.imageResolution[0] << " " << c.imageResolution[1] << "\n";
	}
}

#endif