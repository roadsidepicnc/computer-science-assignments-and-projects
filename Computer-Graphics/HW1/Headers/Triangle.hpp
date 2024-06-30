#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

namespace cse461_hw1
{
	class Triangle
	{
		public:
		
		vector<int> indices;
		
		Triangle(vector<int> Indices)
		{
			indices.clear();
 			for(int i = 0; i < Indices.size(); i++)
			{
				indices.push_back(Indices[i]);
			}
		}
	};

	std::ostream& operator<<(std::ostream &out, const Triangle &t)
	{
    	return out << t.indices[0] << " " << t.indices[1] << " " << t.indices[2]; 
	}
}

#endif