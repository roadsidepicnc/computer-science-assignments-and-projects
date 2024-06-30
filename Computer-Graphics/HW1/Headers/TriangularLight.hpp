#ifndef TRIANGULAR_LIGHT_HPP
#define TRIANGULAR_LIGHT_HPP

namespace cse461_hw1
{
	class TriangularLight
	{
		public:
		
		LightType lightType;
		int id;
		Vector3 intensity;

		Vector3 vertex1;
		Vector3 vertex2;
		Vector3 vertex3;
		
		Vector3 direction;

		TriangularLight(LightType Type, int Id, Vector3 Vertex1, Vector3 Vertex2, Vector3 Vertex3, Vector3 Intensity) : lightType(Type), id(Id), intensity(Intensity), vertex1(Vertex1), vertex2(Vertex2), vertex3(Vertex3)
 		{
 			direction = CalculateDirection();
		}

		Vector3 CalculateDirection()
		{
			return cross(vertex1 - vertex2, vertex1 - vertex3);
		}	
	};

	std::ostream& operator<<(std::ostream &out, const TriangularLight &l)
	{
    	return out 
    	<< "Triangular Light id: " << l.id << "\n"
    	<< "Vertex1: " << l.vertex1 << "\n"
    	<< "Vertex2: " << l.vertex2 << "\n"
    	<< "Vertex3: " << l.vertex3 << "\n"
    	<< "Intensity: " << l.intensity << "\n"
    	<< "Direction: " << l.direction << "\n";
	}
}

#endif