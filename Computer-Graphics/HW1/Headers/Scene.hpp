#ifndef SCENE_HPP
#define SCENE_HPP

#include "Vector3.hpp"
#include "Camera.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace cse461_hw1
{
	class Scene
	{
		public:
		
		int maxRayTraceDepth;
		Vector3 background;
		Camera mainCamera;

		vector<Material> materials;
		vector<Vector3> vertexData;
		vector<Mesh> meshes;
		AmbientLight ambientLight;
		vector<PointLight> pointLights;
		vector<TriangularLight> triangularLights;

		Scene(int MaxRayTraceDepth, Vector3 Background, Camera MainCamera, vector<Material> Materials, vector<Vector3> VertexData, vector<Mesh> Meshes, AmbientLight AmbientLight, vector<PointLight> PointLights, vector<TriangularLight> TriangularLights) : maxRayTraceDepth(MaxRayTraceDepth), background(Background), mainCamera(MainCamera), ambientLight(AmbientLight)
		{
			for(int i = 0; i < Materials.size(); i++)
			{
				materials.push_back(Materials[i]);
			}

			for(int i = 0; i < VertexData.size(); i++)
			{
				vertexData.push_back(VertexData[i]);
			}

			for(int i = 0; i < Meshes.size(); i++)
			{
				meshes.push_back(Meshes[i]);
			}

			for(int i = 0; i < PointLights.size(); i++)
			{
				pointLights.push_back(PointLights[i]);
			}

			for(int i = 0; i < TriangularLights.size(); i++)
			{
				triangularLights.push_back(TriangularLights[i]);
			}
		}
		
		Scene()
		{
		}
	};

	std::ostream& operator<<(std::ostream &out, const Scene &s)
	{
    	return out << "Max Ray Trace Depth: " << s.maxRayTraceDepth << "\n"
    	<< "Background: " << s.background << "\n"
    	<< "Camera:\n" << s.mainCamera << "\n";
	}
}

#endif