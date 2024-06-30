#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

namespace cse461_hw1
{
	class PointLight
	{
		public:
		
		LightType lightType;
		int id;
		Vector3 position;
		Vector3 intensity;

		PointLight(LightType Type, int Id, Vector3 Position, Vector3 Intensity) :  lightType(Type), id(Id), position(Position), intensity(Intensity)
 		{	
		}
	};

	std::ostream& operator<<(std::ostream &out, const PointLight &l)
	{
    	return out 
    	<< "Point Light id: " << l.id << "\n"
    	<< "Position: " << l.position << "\n"
    	<< "Intensity: " << l.intensity << "\n";
	}
}

#endif