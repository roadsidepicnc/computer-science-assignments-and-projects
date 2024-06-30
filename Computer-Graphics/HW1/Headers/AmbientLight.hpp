#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "Vector3.hpp"

namespace cse461_hw1
{
	class AmbientLight
	{
		public:
		
		Vector3 rgb;
		
		AmbientLight(Vector3 Rgb) : rgb(Rgb)
 		{	
		}

		AmbientLight()
		{
		}
	};
}

#endif