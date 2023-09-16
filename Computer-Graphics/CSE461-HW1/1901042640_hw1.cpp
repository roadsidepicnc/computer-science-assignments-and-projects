#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include "Vector3.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"

typedef unsigned char byte;

using namespace rapidxml;
using namespace std;

string x, y, z, v;
stringstream stream;

int hitMaterialId;
Vector3 hitNormalVector;
Vector3 hitPoint;
float hitT;

float maxRayTraceDepth;
Vector3 backgroundColor;
Vector3 ambientLight;

vector<Vector3> vertexData;
vector<Camera> cameras;
vector<Light> lights;
vector<Material> materials;
vector<Mesh> meshes;
vector<Triangle> triangles;
vector<Sphere> spheres;

Material GetMaterialWithId(int id)
{
	for(int i = 0; i < materials.size(); i++)
	{
		if(materials[i].id == id)
		{
			return materials[i];
		}
	}
}

void CreateCamera(xml_node<> * camera_node)
{	
	stream.str(camera_node->first_node("Position")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 camPos(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(camera_node->first_node("Gaze")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 camGaze(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(camera_node->first_node("Up")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 camUp(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(camera_node->first_node("NearPlane")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	getline(stream, v, ' ');
	float camNearPlane[] = {stof(x), stof(y), stof(z), stof(v)};
	
	stream.clear();
	stream.str(camera_node->first_node("NearDistance")->value());
	getline(stream, x, ' ');
	float camNearDistance = stof(x);
	
	stream.clear();
	stream.str(camera_node->first_node("ImageResolution")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	float camImageResolution[] = {stof(x), stof(y)};
	
	stream.clear();
	stream.str(camera_node->first_node("ImageName")->value());
	string camImageName;
	getline(stream, camImageName, ' ');
	
	Camera camera(camPos, camGaze, camUp, camNearPlane, camNearDistance, camImageResolution, camImageName);
	cameras.push_back(camera);
}

void CreateCameras(xml_node<> * root_node)
{
	stream.clear();
	xml_node<> * camera_node = root_node -> first_node("Cameras") -> first_node("Camera");
	CreateCamera(camera_node);
	while(camera_node != root_node -> first_node("Cameras") -> last_node("Camera"))
	{
		xml_node<> * temp = camera_node -> next_sibling();
		camera_node = temp;
		CreateCamera(camera_node);
	}
}

void CreateLight(xml_node<> * light_node)
{	
	stream.clear();
	stream.str(light_node->first_node("Position")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 lightPosition(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(light_node->first_node("Intensity")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 lightIntensity(Vector3(stof(x), stof(y), stof(z)));
		
	Light light(lightPosition, lightIntensity);
	lights.push_back(light);
}

void CreateLights(xml_node<> * root_node)
{
	stream.clear();
	stream.str(root_node -> first_node("Lights") -> first_node("AmbientLight")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	ambientLight = Vector3(stof(x), stof(y), stof(z));
	
	xml_node<> * light_node = root_node -> first_node("Lights") -> first_node("PointLight");
	CreateLight(light_node);
	while(light_node != root_node -> first_node("Lights") -> last_node("PointLight"))
	{
		xml_node<> * temp = light_node -> next_sibling();
		light_node = temp;
		CreateLight(light_node);
	}
}

void CreateMaterial(xml_node<> * material_node)
{	
	int ID = stoi(material_node-> first_attribute("id")->value());

	stream.clear();
	stream.str(material_node->first_node("AmbientReflectance")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 materialAmbientReflectance(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(material_node->first_node("DiffuseReflectance")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 materialDiffuseReflectance(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(material_node->first_node("SpecularReflectance")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 materialSpecularReflectance(Vector3(stof(x), stof(y), stof(z)));
	
	stream.clear();
	stream.str(material_node->first_node("MirrorReflectance")->value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	Vector3 materialMirrorReflectance(Vector3(stof(x), stof(y), stof(z)));
	
	float materialPhongExponent = stof(material_node->first_node("PhongExponent")->value());
		
	Material material(ID, materialAmbientReflectance, materialDiffuseReflectance, materialSpecularReflectance, materialMirrorReflectance, materialPhongExponent);
	materials.push_back(material);
}

void CreateMaterials(xml_node<> * root_node)
{	
	xml_node<> * material_node = root_node -> first_node("Materials") -> first_node("Material");
	CreateMaterial(material_node);
	while(material_node != root_node -> first_node("Materials") -> last_node("Material"))
	{
		xml_node<> * temp = material_node -> next_sibling();
		material_node = temp;
		CreateMaterial(material_node);
	}
}

void CreateVertexData(xml_node<> * root_node)
{
	stream.clear();
	stream.str(root_node -> first_node("VertexData") -> value());
	
	while(getline(stream, x, ' '))
	{
		getline(stream, y, ' ');
		getline(stream, z, '\n');
		vertexData.push_back(Vector3(stof(x), stof(y), stof(z)));
	}
}

void CreateTriangle(xml_node<> * triangle_node)
{
	if(triangle_node != nullptr)
	{
		stream.clear();
		int triangleMaterial = stoi(triangle_node->first_node("Material")->value());
		Material material = GetMaterialWithId(triangleMaterial);
	
		stream.clear();
		stream.str(triangle_node->first_node("Indices")->value());
		getline(stream, x, ' ');
		getline(stream, y, ' ');
		getline(stream, z, ' ');
		Vector3 triangleIndices(Vector3(stof(x), stof(y), stof(z)));
	
		ObjectType type = triangle;
		Triangle triangle(material, type, triangleIndices);
		triangles.push_back(triangle);
	}
}

void CreateTriangles(xml_node<> * object_node)
{
	xml_node<> * triangle_node = object_node -> first_node("Triangle");
	CreateTriangle(triangle_node);
	while(triangle_node != object_node -> last_node("Triangle"))
	{
		xml_node<> * temp = triangle_node -> next_sibling();
		triangle_node = temp;
		CreateTriangle(triangle_node);
	}	
}

void CreateMesh(xml_node<> * mesh_node)
{
	if(mesh_node != nullptr)
	{
		stream.clear();
		int meshMaterial = stoi(mesh_node->first_node("Material")->value());
		Material material = GetMaterialWithId(meshMaterial);
		vector<Vector3> vec;
		stream.clear();
		stream.str(mesh_node -> first_node("Faces") -> value());
		while(getline(stream, x, ' '))
		{
			getline(stream, y, ' ');
			getline(stream, z, '\n');
			vec.push_back(Vector3(stoi(x), stoi(y), stoi(z)));
		}	
	
		ObjectType type = mesh;
		Mesh mesh(material, type, vec);
		meshes.push_back(mesh);
	}		
}

void CreateMeshes(xml_node<> * object_node)
{
	xml_node<> * mesh_node = object_node -> first_node("Mesh");
	CreateMesh(mesh_node);
	while(mesh_node != object_node -> last_node("Mesh"))
	{
		xml_node<> * temp = mesh_node -> next_sibling();
		mesh_node = temp;
		CreateMesh(mesh_node);
	}
}

void CreateSphere(xml_node<> * sphere_node)
{
	if(sphere_node != nullptr)
	{
		stream.clear();
		int sphereMaterial = stoi(sphere_node->first_node("Material")->value());
		Material material = GetMaterialWithId(sphereMaterial);
		int sphereCenter = stof(sphere_node->first_node("Center")->value());
		float sphereRadius = stof(sphere_node->first_node("Radius")->value());
	
		ObjectType type = sphere;
		Sphere sphere(material, type, sphereCenter, sphereRadius);
		spheres.push_back(sphere);
	}
}

void CreateSpheres(xml_node<> * object_node)
{
	xml_node<> * sphere_node = object_node -> first_node("Sphere");
	CreateSphere(sphere_node);
	while(sphere_node != object_node -> last_node("Sphere"))
	{
		xml_node<> * temp = sphere_node -> next_sibling();
		sphere_node = temp;
		CreateSphere(sphere_node);
	}
}

void CreateObjects(xml_node<> * root_node)
{
	xml_node<> * object_node = root_node -> first_node("Objects");
	CreateMeshes(object_node);
	CreateTriangles(object_node);
	CreateSpheres(object_node);
}

Vector3 unit_vector(Vector3 v) 
{
	return v / v.length();
}

Vector3 cross(const Vector3 &u, const Vector3 &v)
{
	return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

double dot(const Vector3 &u, const Vector3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

bool IsOnSameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
{
	Vector3 cp1 = cross(b - a, p1 - a);
	Vector3 cp2 = cross(b - a, p2 - a);
	return dot(cp1, cp2) >= 0;
}

bool IsPointInTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
	return IsOnSameSide(p, a, b, c) && IsOnSameSide(p, b, a, c) && IsOnSameSide(p, c, a, b);
}

void TryToHitTriangle(Vector3 rayDirection, Triangle triangle)
{
	Vector3 a = vertexData[triangle.indices[0] - 1], b = vertexData[triangle.indices[1] - 1], c = vertexData[triangle.indices[2] - 1];
	Vector3 n = cross(c - b, a - b);
	float dn = dot(rayDirection, n);
	if(dn == 0)
	{
		return;
	}
	
	float t = dot((a - cameras[0].position), n) / dot(rayDirection, n);
	if(t > 0 && t < hitT && (rayDirection * t).length() <= maxRayTraceDepth)
	{
		Vector3 p = cameras[0].position + rayDirection * t;
		if(IsPointInTriangle(p, a, b, c))
		{
			hitMaterialId = triangle.material.id;
			hitNormalVector = n;
			hitT = t;
			hitPoint = rayDirection * hitT;
		}
	}
}

void TryToHitSphere(Vector3 rayDirection, Sphere sphere)
{
	Vector3 center = vertexData[sphere.center - 1];
	float variable1 = -1 * dot(rayDirection, cameras[0].position - center);
	float variable2 = sqrt(pow(dot(rayDirection, cameras[0].position - center), 2) - (dot(rayDirection, rayDirection) * (dot(cameras[0].position - center, cameras[0].position - center) - pow(sphere.radius, 2))));
	float variable3 = dot(rayDirection, rayDirection);
	float t1 = (variable1 + variable2) / variable3;
	float t2 = (variable1 - variable2) / variable3;
	bool isResultFound = true;
	if(t2 >= t1 && t1 > 0 && t1 <= hitT && (rayDirection * t1).length() <= maxRayTraceDepth)
	{
		hitT = t1;
	}
	else if(t1 >= t2 && t2 > 0 && t2 <= hitT && (rayDirection * t2).length() <= maxRayTraceDepth)
	{
		hitT = t2;
	}
	else
	{
		isResultFound = false;
	}
	
	if(isResultFound)
	{
		hitMaterialId = sphere.material.id;
		hitPoint = rayDirection * hitT;
		hitNormalVector = unit_vector(hitPoint - center);
	}
}

bool IsHittingAnyObject(Vector3 rayDirection)
{
	hitT = numeric_limits<float>::max();
	for(int i = 0; i < triangles.size();++i)
	{
		TryToHitTriangle(rayDirection, triangles[i]);
	}
	
	for(int i = 0; i < spheres.size();++i)
	{
		TryToHitSphere(rayDirection, spheres[i]);
	}
	
	for(int i = 0; i < meshes.size(); i++)
	{
		for(int j = 0;j < meshes[i].faces.size(); j++)
		{
			TryToHitTriangle(rayDirection, meshes[i].faces[j]);
		}
	}
	
	return hitT != numeric_limits<float>::max();
}

float CalculateDiffuse(Vector3 lightRayVector, int lightIndex, int rgb)
{
	return materials[hitMaterialId - 1].diffuseReflectance[rgb] * cos(dot(unit_vector(lightRayVector), unit_vector(hitNormalVector))) * lights[lightIndex].intensity[rgb];
}

float CalculateSpecular(Vector3 lightRayVector, int rgb)
{
	Vector3 h = (lightRayVector + cameras[0].position - hitPoint) / 2;
	return materials[hitMaterialId - 1].specularReflectance[rgb] * pow(cos(dot(hitNormalVector, h)), materials[hitMaterialId - 1].phongExponent);
}

void PrintXmlData()
{
	cout << "CAMERA DATA:\n";
	cout << "Position: " << cameras[0].position << "\n";
	cout << "Gaze: " << cameras[0].gaze << "\n";
	cout << "Up: " << cameras[0].up << "\n";
	cout << "Near Plane: " << cameras[0].nearPlane[0] << " " << cameras[0].nearPlane[1] << " " << cameras[0].nearPlane[2] << " " << cameras[0].nearPlane[3] <<"\n";
	cout << "Near Distance: " << cameras[0].nearDistance << "\n";	
	cout << "Image Resolution: " << cameras[0].imageResolution[0] << "x" << cameras[0].imageResolution[1] << "\n";
	cout << "Image Name: " << cameras[0].imageName << "\n";
	cout << "\n";
	cout << "LIGHT DATA:\n";
	cout << "Ambient Light: " << ambientLight << "\n";
	for(int i = 0; i < lights.size(); i++)
	{
		cout << "id" << i << "\n";
		cout << "Position: " << lights[i].position << "\n";
		cout << "Intensity: " << lights[i].intensity << "\n";
	}
	
	cout << "\n";
	cout << "MATERIAL DATA:\n";
	for(int i = 0; i < materials.size(); i++)
	{
		cout << "id" << materials[i].id << "\n";
		cout << "Ambient Reflectance: " << materials[i].ambientReflectance << "\n";
		cout << "Diffuse Reflectance: " << materials[i].diffuseReflectance << "\n";
		cout << "Specular Reflectance: " << materials[i].specularReflectance << "\n";
		cout << "Mirror Reflectance: " << materials[i].mirrorReflectance << "\n";
		cout << "Phong Exponent: " << materials[i].phongExponent << "\n";
	}
	
	cout << "\n";
	cout << "VERTEX DATA:\n";
	for(int i = 0; i < vertexData.size(); i++)
	{
		cout << vertexData[i] << "\n";
	}
	
	cout << "\n";
	cout << "MESH DATA:\n";
	for(int i = 0; i < meshes.size(); i++)
	{
		cout << "Material Id" << meshes[i].material.id << "\n";
		for(int j = 0; j < meshes[i].faces.size(); j++)
		{
			cout << "Faces: " << meshes[i].faces[j].indices << "\n";
		}
	}
	
	cout << "\n";
	cout << "TRIANGLE DATA:\n";
	for(int i = 0; i < triangles.size(); i++)
	{
		cout << "Material Id: " << triangles[i].material.id << "\n";
		cout << "Indices: " << triangles[i].indices << "\n";
	}
	
	cout << "\n";
	cout << "SPHERE DATA:\n";
	for(int i = 0; i < spheres.size(); i++)
	{
		cout << "Material Id: " << spheres[i].material.id << "\n";
		cout << "Center: " << spheres[i].center << "\n";
		cout << "Radius: " << spheres[i].radius << "\n";
	}
}

int main()
{
	int vertexCount = 0;
	xml_document<> doc;
	xml_node<> * root_node;
	ifstream theFile ("data.xml");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("Scene");
	
	maxRayTraceDepth = stof(root_node -> first_node("maxraytracedepth") -> value());
		
	stream.str(root_node -> first_node("BackgroundColor") -> value());
	getline(stream, x, ' ');
	getline(stream, y, ' ');
	getline(stream, z, ' ');
	backgroundColor.Set(stof(x), stof(y), stof(z));
	
	CreateCameras(root_node);
	CreateLights(root_node);
	CreateMaterials(root_node);
	CreateVertexData(root_node);
	CreateObjects(root_node);
	PrintXmlData();
	
	size_t SIZE = cameras[0].imageResolution[0] * cameras[0].imageResolution[1] * 3;
	byte * imagePixels = (byte*) malloc(SIZE);
	memset(imagePixels, 255, SIZE);
	ofstream myImage;
	myImage.open(cameras[0].imageName);
	
	if (myImage.fail())
	{
		cout << "Unable to create " << cameras[0].imageName << endl;
		return 0;
	}

	myImage << "P3" << endl;
	myImage << cameras[0].imageResolution[0] << " " << cameras[0].imageResolution[1] << endl;
	myImage << "255" << endl;

	float r;
	float b;
	float g;
	int pixel = 0;
	
	Vector3 normalizedGazeVector = unit_vector(cameras[0].gaze);
	Vector3 normalizedUpVector = unit_vector(cameras[0].up);
	Vector3 w = -1 * normalizedGazeVector;
	Vector3 planeCenter = cameras[0].position + normalizedGazeVector * cameras[0].nearDistance;
	Vector3 rightVector = cross(w, normalizedUpVector);
	Vector3 leftBottomPoint = cameras[0].nearPlane[0] * rightVector + cameras[0].nearPlane[2] * normalizedUpVector + planeCenter;
	
	float horizontalDif = (float)(cameras[0].nearPlane[1] - cameras[0].nearPlane[0]) / (float)cameras[0].imageResolution[0];
	float verticalDif = (float)(cameras[0].nearPlane[3] - cameras[0].nearPlane[2]) / (float)cameras[0].imageResolution[1];
	
	for(int i = 0; i < cameras[0].imageResolution[0]; i++)
	{
		for(int j = 0; j < cameras[0].imageResolution[1]; j++)
		{
			Vector3 directionVector = leftBottomPoint + (rightVector * (float) j * horizontalDif) + (normalizedUpVector * (float) i * verticalDif);
			Vector3 normalizedDirectionVector = unit_vector(directionVector - cameras[0].position);
			if(IsHittingAnyObject(normalizedDirectionVector))
			{
				r = materials[hitMaterialId - 1].ambientReflectance[0] * ambientLight[0] ;
				g = materials[hitMaterialId - 1].ambientReflectance[1] * ambientLight[1];
				b = materials[hitMaterialId - 1].ambientReflectance[2] * ambientLight[2];
				
				for(int k = 0; k < lights.size(); k++)
				{
					r += CalculateDiffuse(lights[k].position - hitPoint, k, 0) + CalculateSpecular(lights[k].position - hitPoint, 0);
					g += CalculateDiffuse(lights[k].position - hitPoint, k, 1) + CalculateSpecular(lights[k].position - hitPoint, 1);
					b += CalculateDiffuse(lights[k].position - hitPoint, k, 2) + CalculateSpecular(lights[k].position - hitPoint, 2); 
				}
			}
			else
			{
				r = backgroundColor[0];
				g = backgroundColor[1];
				b = backgroundColor[2];
			}
			
			r = min(r, (float) 255);
			g = min(g, (float) 255);
			b = min(b, (float) 255);

			imagePixels[pixel * 3] = (int) r;
			imagePixels[pixel * 3 + 1] = (int) g;
			imagePixels[pixel * 3 + 2] = (int) b;
			pixel++;	
		}
	}
	
	cout << "\nCreating " << cameras[0].imageName << endl;

	for (int x = 0; x < SIZE; x++) 
	{
		int value = imagePixels[x];
		myImage << value << " " << endl;
	}

	cout << "\nDONE creating " << cameras[0].imageName << endl;
	
	myImage.close();
	free(imagePixels);
	imagePixels = NULL;

	return 0;
}
