#include <iostream>
#include <sstream>
#include <ostream>
#include <string.h>
#include <thread>
#include <chrono>
#include "xml_library/rapidxml.hpp"
#include "xml_library/rapidxml_utils.hpp"
#include "xml_library/rapidxml_print.hpp"
#include "Headers/Camera.hpp"
#include "Headers/PointLight.hpp"
#include "Headers/AmbientLight.hpp"
#include "Headers/TriangularLight.hpp"
#include "Headers/Material.hpp"
#include "Headers/Mesh.hpp"
#include "Headers/Scene.hpp"

typedef unsigned char Byte;

#define FILE_NAME "Result.ppm"

using namespace std;
using namespace rapidxml;
using namespace cse461_hw1;

Scene scene;
stringstream stream;
Byte * imagePixels; 

Vector3 normalizedUpVector;
Vector3 rightVector;
Vector3 leftBottomPoint;
float horizontalDif;
float verticalDif;

float timeThreshold = 0.001;

// Functions for creating scene 

struct RayCastResult
{
    Vector3 hitPoint;
    int hitMaterialId;
    float t;
};

Camera CreateCamera(xml_node<> * scene_node)
{
    string x;
    string y;
    string z;
    string v;

    xml_node<> * camera_node = scene_node -> first_node("camera");

    stream.clear();
    stream.str(camera_node -> first_node("position") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 position(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(camera_node->first_node("gaze")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 gaze(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(camera_node->first_node("up")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 up(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(camera_node->first_node("nearPlane")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    getline(stream, v, ' ');
    float nearPlane[] = {stof(x), stof(y), stof(z), stof(v)};
    
    stream.clear();
    stream.str(camera_node->first_node("neardistance")->value());
    getline(stream, x, ' ');
    float nearDistance = stof(x);
    
    stream.clear();
    stream.str(camera_node->first_node("imageresolution")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    float imageResolution[] = {stof(x), stof(y)};
    
    
    Camera camera(position, gaze, up, nearPlane, nearDistance, imageResolution);
    return camera;
}

AmbientLight CreateAmbientLight(xml_node<> * lights_node)
{
    string x;
    string y;
    string z;

    stream.clear();
    stream.str(lights_node -> first_node("ambientlight") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    AmbientLight ambientLight(Vector3(stof(x), stof(y), stof(z)));

    return ambientLight;
}

PointLight CreatePointLight(xml_node<> * light_node)
{
    string x;
    string y;
    string z;

    stream.clear();
    int id = stoi(light_node-> first_attribute("id") -> value());

    stream.clear();
    stream.str(light_node->first_node("position")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 lightPosition(Vector3(stof(x), stof(y), stof(z)));

    stream.clear();
    stream.str(light_node->first_node("intensity")->value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 lightIntensity(Vector3(stof(x), stof(y), stof(z)));
        
    PointLight pointLight(point_light, id, lightPosition, lightIntensity);

    return pointLight;
}

vector<PointLight> CreatePointLights(xml_node<> * lights_node)
{
    vector<PointLight> pointLights;
    xml_node<> * light_node = lights_node -> first_node("pointlight");

    if(light_node == NULL)
    {
        return pointLights;
    }

    pointLights.push_back(CreatePointLight(light_node));

    while(light_node != lights_node -> last_node("pointlight"))
    {
        light_node = light_node -> next_sibling();

        if(strcmp(light_node -> name(), "pointlight") == 0)
        {
            pointLights.push_back(CreatePointLight(light_node));
        }
    }

    return pointLights;
}

TriangularLight CreateTriangularLight(xml_node<> * light_node)
{
    string x;
    string y;
    string z;
    stream.clear();
    int id = stoi(light_node-> first_attribute("id") -> value());
    stream.clear();
    stream.str(light_node -> first_node("vertex1") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 vertex1(Vector3(stof(x), stof(y), stof(z)));

    stream.clear();
    stream.str(light_node->first_node("vertex2") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 vertex2(Vector3(stof(x), stof(y), stof(z)));

    stream.clear();
    stream.str(light_node->first_node("vertex3") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 vertex3(Vector3(stof(x), stof(y), stof(z)));

    stream.clear();
    stream.str(light_node->first_node("intensity") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 lightIntensity(Vector3(stof(x), stof(y), stof(z)));
        
    TriangularLight triangularLight(triangular_light, id, vertex1, vertex2, vertex3, lightIntensity);
    return triangularLight;
}

vector<TriangularLight> CreateTriangularLights(xml_node<> * lights_node)
{
    vector<TriangularLight> triangularLights;

    xml_node<> * light_node = lights_node -> first_node("triangularlight");

    if(light_node == NULL)
    {
        return triangularLights;
    }

    if(strcmp(light_node -> name(), "triangularlight") == 0)
    {
        triangularLights.push_back(CreateTriangularLight(light_node));
    }

    while(light_node != lights_node -> last_node("triangularlight"))
    {
        light_node = light_node -> next_sibling();

        if(strcmp(light_node -> name(), "triangularlight") == 0)
        {
            triangularLights.push_back(CreateTriangularLight(light_node));
        }
    }

    return triangularLights;
}

Material CreateMaterial(xml_node<> * material_node)
{   
    string x;
    string y;
    string z;

    int ID = stoi(material_node-> first_attribute("id") -> value());

    stream.clear();
    stream.str(material_node->first_node("ambient") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 materialAmbientReflectance(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(material_node->first_node("diffuse") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 materialDiffuseReflectance(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(material_node->first_node("specular") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 materialSpecularReflectance(Vector3(stof(x), stof(y), stof(z)));
    
    stream.clear();
    stream.str(material_node->first_node("mirrorreflactance") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 materialMirrorReflectance(Vector3(stof(x), stof(y), stof(z)));
    
    float materialPhongExponent = stof(material_node -> first_node("phongexponent") -> value());
        
    Material material(ID, materialAmbientReflectance, materialDiffuseReflectance, materialSpecularReflectance, materialMirrorReflectance, materialPhongExponent);
    return material;
}

vector<Material> CreateMaterials(xml_node<> * scene_node)
{ 
    vector<Material> materials;

    xml_node<> * material_node = scene_node -> first_node("materials") -> first_node("material");
    materials.push_back(CreateMaterial(material_node));

    while(material_node != scene_node -> first_node("materials") -> last_node("material"))
    {
        material_node = material_node -> next_sibling();
        materials.push_back(CreateMaterial(material_node));
    }

    return materials;
}

Mesh CreateMesh(xml_node<> * mesh_node)
{
    string x;
    string y;
    string z;

    stream.clear();
    int id = stoi(mesh_node-> first_attribute("id") -> value());
    stream.clear();
    int materialId = stoi(mesh_node->first_node("materialid")->value());
    stream.clear();
    stream.str(mesh_node -> first_node("faces") -> value());

    vector<Triangle> triangles;

    while(getline(stream, x, ' '))
    {
        vector<int> indices;
        getline(stream, y, ' ');
        getline(stream, z, '\n');
        indices.push_back(stoi(x));
        indices.push_back(stoi(y));
        indices.push_back(stoi(z));
        triangles.push_back(Triangle(indices));
    }   
    
    Mesh mesh(id, materialId, triangles);
    return mesh;   
}

vector<Mesh> CreateMeshes(xml_node<> * scene_node)
{
    xml_node<> * object_node = scene_node -> first_node("objects");
    xml_node<> * mesh_node = object_node -> first_node("mesh");
    vector<Mesh> meshes;
    meshes.push_back(CreateMesh(mesh_node));

    while(mesh_node != object_node -> last_node("mesh"))
    {
        mesh_node = mesh_node -> next_sibling();;
        meshes.push_back(CreateMesh(mesh_node));
    }

    return meshes;
}

vector<Vector3> CreateVertexData(xml_node<> * scene_node)
{
    string x;
    string y;
    string z;

    stream.clear();
    stream.str(scene_node -> first_node("vertexdata") -> value());
    
    vector<Vector3> vertexData;

    while(getline(stream, x, ' '))
    {
        getline(stream, y, ' ');
        getline(stream, z, '\n');
        vertexData.push_back(Vector3(stof(x), stof(y), stof(z)));
    }

    return vertexData;
}

Scene CreateScene(xml_node<> * scene_node)
{
    string x;
    string y;
    string z;

    int maxRayTraceDepth = stof(scene_node -> first_node("maxraytracedepth") -> value());

    stream.str(scene_node -> first_node("backgroundColor") -> value());
    getline(stream, x, ' ');
    getline(stream, y, ' ');
    getline(stream, z, ' ');
    Vector3 backgroundColor(stof(x), stof(y), stof(z));
    Camera camera = CreateCamera(scene_node);
    vector<Vector3> vertexData = CreateVertexData(scene_node);
    vector<Material> materials = CreateMaterials(scene_node);
    vector<Mesh> meshes = CreateMeshes(scene_node);

    AmbientLight ambientLight = CreateAmbientLight(scene_node -> first_node("lights"));
    vector<PointLight> pointLights = CreatePointLights(scene_node -> first_node("lights"));
    vector<TriangularLight> triangularLights = CreateTriangularLights(scene_node -> first_node("lights"));

    Scene scene(maxRayTraceDepth, backgroundColor, camera, materials, vertexData, meshes, ambientLight, pointLights, triangularLights);
    return scene;
}

///////////////////////////////////


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

float CalculateDiffuse(Vector3 lightRayVector, int lightIndex, int rgb, Vector3 hitNormalVector, int hitMaterialId, bool isPointLight)
{
    float r = lightRayVector.length();
    if(isPointLight)
    {
        return scene.materials[hitMaterialId - 1].diffuse[rgb] * cos(dot(unit_vector(hitNormalVector), unit_vector(lightRayVector))) * scene.pointLights[lightIndex].intensity[rgb] / (r * r);
    }
    else
    {
        return scene.materials[hitMaterialId - 1].diffuse[rgb] * abs(dot(unit_vector(hitNormalVector), unit_vector(lightRayVector))) * scene.triangularLights[lightIndex].intensity[rgb] / (r * r);
    }
}

float CalculateSpecular(Vector3 lightRayVector, int lightIndex, int rgb, Vector3 hitNormalVector, Vector3 hitPoint, int hitMaterialId)
{
    float r = lightRayVector.length();
    Vector3 h = unit_vector(lightRayVector + scene.mainCamera.position - hitPoint);
    return pow(dot(hitNormalVector, h), scene.materials[hitMaterialId - 1].phongExponent) * scene.

    pointLights[lightIndex].intensity[rgb] / (r * r);
}

void TryToHitTriangle(Vector3 rayDirection, Triangle triangle, int materialId, Vector3 origin, float &hitT, Vector3 &hitNormalVector, Vector3 &hitPoint, int &hitMaterialId)
{
    Vector3 a = scene.vertexData[triangle.indices[0] - 1], b = scene.vertexData[triangle.indices[1] - 1], c = scene.vertexData[triangle.indices[2] - 1];
    Vector3 n = cross(c - b, a - b);
    float dn = dot(rayDirection, n);

    if(dn == 0)
    {
        return;
    }
    
    float t = dot((a - origin), n) / dot(rayDirection, n);
    if(t > 0 && t < hitT && (rayDirection * t).length() <= scene.maxRayTraceDepth)
    {
        Vector3 p = origin + rayDirection * t;
        if(IsPointInTriangle(p, a, b, c))
        {
            hitNormalVector = n;
            hitT = t;
            hitPoint = p;
            hitMaterialId = materialId;
        }
    }
}

void TryToHitTriangle(Vector3 rayDirection, vector<Vector3> triangleVertexes, Vector3 origin, float &hitT, Vector3 &lightPoint)
{
    Vector3 a = triangleVertexes[0], b = triangleVertexes[1], c = triangleVertexes[2];
    Vector3 n = cross(c - b, a - b);
    float dn = dot(rayDirection, n);

    if(dn == 0)
    {
        return;
    }
    
    float t = dot((a - origin), n) / dot(rayDirection, n);
    if(t > 0 && t < hitT && (rayDirection * t).length() <= 100)
    {
        Vector3 p = origin + rayDirection * t;
        if(IsPointInTriangle(p, a, b, c))
        {
            hitT = t;
            lightPoint = p;
        }
    }
}

bool TryToHitTriangle(Vector3 rayDirection, Triangle triangle, Vector3 origin)
{
    Vector3 a = scene.vertexData[triangle.indices[0] - 1], b = scene.vertexData[triangle.indices[1] - 1], c = scene.vertexData[triangle.indices[2] - 1];
    Vector3 n = cross(c - b, a - b);
    float dn = dot(rayDirection, n);

    if(dn == 0)
    {
        return false;
    }
    
    float t = dot((a - origin), n) / dot(rayDirection, n);
    if(t > timeThreshold && (rayDirection * t).length() <= scene.maxRayTraceDepth)
    {
        Vector3 p = origin + rayDirection * t;
        if(IsPointInTriangle(p, a, b, c))
        {
            return true;
        }
    }

    return false;
}

bool IsHittingAnyObject(Vector3 rayDirection, Vector3 origin, Vector3 &hitNormalVector, Vector3 &hitPoint, int &hitMaterialId)
{
    float hitT = numeric_limits<float>::max();

    for(int i = 0; i < scene.meshes.size(); i++)
    {
        for(int j = 0; j < scene.meshes[i].faces.size(); j++)
        {
            TryToHitTriangle(rayDirection, scene.meshes[i].faces[j], scene.meshes[i].materialId, origin, hitT, hitNormalVector, hitPoint, hitMaterialId);
        }
    }
    
    return hitT != numeric_limits<float>::max();
}

bool IsHittingAnyObject(Vector3 rayDirection, Vector3 origin)
{
    bool result = false;
    for(int i = 0; i < scene.meshes.size(); i++)
    {
        for(int j = 0; j < scene.meshes[i].faces.size(); j++)
        {
            result = TryToHitTriangle(rayDirection, scene.meshes[i].faces[j], origin);
            if(result)
            {
                return result;
            }
        }
    }
    
    return result;
}

bool CheckIfInsideShadow(Vector3 normalizedDirectionVector, Vector3 origin)
{
    if(IsHittingAnyObject(normalizedDirectionVector, origin))
    {
        return true;
    }

    return false;
}

bool CheckIfTriangularLightHits(TriangularLight triangularLight, Vector3 origin, Vector3 &lightPoint)
{
    float hitT = numeric_limits<float>::max();
    vector<Vector3> vertexes;
    vertexes.push_back(triangularLight.vertex1);
    vertexes.push_back(triangularLight.vertex2);
    vertexes.push_back(triangularLight.vertex3);
    Vector3 direction = -triangularLight.direction;
    TryToHitTriangle(direction, vertexes, origin, hitT, lightPoint);
    return hitT != numeric_limits<float>::max();
}

void CalculatePixelValues(int threadCounter, int pixelsPerThreadCount)
{   
    Vector3 hitNormalVector;
    Vector3 hitPoint;
    float hitT;
    int hitMaterialId;

    float r;
    float b;
    float g;

    int startPixel = threadCounter * pixelsPerThreadCount;

    for(int i = 0; i < pixelsPerThreadCount; i++)
    {
        int currentPixel = startPixel + i;
        int row = currentPixel / (int)scene.mainCamera.imageResolution[0];
        int column = currentPixel % (int)scene.mainCamera.imageResolution[1];

        Vector3 directionVector = leftBottomPoint + (rightVector * (float) column * horizontalDif) + (normalizedUpVector * (float) row * verticalDif);
        Vector3 normalizedDirectionVector = unit_vector(directionVector - scene.mainCamera.position);

        if(IsHittingAnyObject(normalizedDirectionVector, scene.mainCamera.position, hitNormalVector, hitPoint, hitMaterialId))
        {
            Vector3 shadingHitPoint = hitPoint;
            Vector3 shadingHitNormalVector = hitNormalVector;
            int shadingHitMaterialId = hitMaterialId;
            r = scene.materials[hitMaterialId - 1].ambient[0] * scene.ambientLight.rgb[0];
            g = scene.materials[hitMaterialId - 1].ambient[1] * scene.ambientLight.rgb[1];
            b = scene.materials[hitMaterialId - 1].ambient[2] * scene.ambientLight.rgb[2];

            for(int k = 0; k < scene.pointLights.size(); k++)
            {
                Vector3 normalizedDirectionVector = unit_vector(scene.pointLights[k].position - shadingHitPoint);
                if(!CheckIfInsideShadow(normalizedDirectionVector, shadingHitPoint))
                {
                    r += CalculateDiffuse(scene.pointLights[k].position - shadingHitPoint, k, 0, shadingHitNormalVector, shadingHitMaterialId, true) + CalculateSpecular(scene.pointLights[k].position - shadingHitPoint, k, 0, shadingHitNormalVector, shadingHitPoint, shadingHitMaterialId);
                    g += CalculateDiffuse(scene.pointLights[k].position - shadingHitPoint, k, 1, shadingHitNormalVector, shadingHitMaterialId, true) + CalculateSpecular(scene.pointLights[k].position - shadingHitPoint, k, 1, shadingHitNormalVector, shadingHitPoint, shadingHitMaterialId);
                    b += CalculateDiffuse(scene.pointLights[k].position - shadingHitPoint, k, 2, shadingHitNormalVector, shadingHitMaterialId, true) + CalculateSpecular(scene.pointLights[k].position - shadingHitPoint, k, 2, shadingHitNormalVector, shadingHitPoint, shadingHitMaterialId);
                }
            }
    
            for(int k = 0; k < scene.triangularLights.size(); k++)
            { 
                Vector3 lightPoint;
                if(CheckIfTriangularLightHits(scene.triangularLights[k], shadingHitPoint, lightPoint))
                {
                    r += CalculateDiffuse(lightPoint - shadingHitPoint, k, 0, shadingHitNormalVector, shadingHitMaterialId, false);
                    g += CalculateDiffuse(lightPoint - shadingHitPoint, k, 1, shadingHitNormalVector, shadingHitMaterialId, false);
                    b += CalculateDiffuse(lightPoint - shadingHitPoint, k, 2, shadingHitNormalVector, shadingHitMaterialId, false);
                }
            } 
        }
        else
        {
            r = scene.background[0];
            g = scene.background[1];
            b = scene.background[2];
        }

        r = min(r, (float) 255);
        g = min(g, (float) 255);
        b = min(b, (float) 255);

        imagePixels[currentPixel * 3] = (int) r;
        imagePixels[currentPixel * 3 + 1] = (int) g;
        imagePixels[currentPixel * 3 + 2] = (int) b;

        //cout << "Row: " << row << " Column: " << column << "\n";
    }

    return;
}

// Main Function

int main() 
{
    xml_document<> doc;
    ifstream theFile ("Data.xml");
    
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> * scene_node = doc.first_node("scene");
    scene = CreateScene(scene_node);

    cout << scene;
    cout << "VERTEX DATA: \n";
    for (int i = 0; i < scene.vertexData.size(); ++i)
    {
       cout << scene.vertexData[i] << "\n";
    }

    cout << "MATERIALS: \n";
    for (int i = 0; i < scene.materials.size(); ++i)
    {
       cout << scene.materials[i] << "\n";
    }

    cout << "MESHES: \n";
    for (int i = 0; i < scene.meshes.size(); ++i)
    {
        cout << "Mesh Id: " << scene.meshes[i].id << "\n";
        cout << "Material Id: " << scene.meshes[i].materialId << "\n";
        for (int j = 0; j < scene.meshes[i].faces.size(); ++j)
        {
            cout << scene.meshes[i].faces[j] << "\n";
        }
    }

    cout << "POINT LIGHTS: \n";
    for (int i = 0; i < scene.pointLights.size(); ++i)
    {
        cout << scene.pointLights[i] << "\n";
    }

    cout << "TRIANGULAR LIGHTS: \n";
    for (int i = 0; i < scene.triangularLights.size(); ++i)
    {
        cout << scene.triangularLights[i] << "\n";
    }

    ////////////////////////////////////

    size_t SIZE = scene.mainCamera.imageResolution[0] * scene.mainCamera.imageResolution[1] * 3;
    imagePixels = (Byte*) malloc(SIZE);
    memset(imagePixels, 255, SIZE);
    ofstream myImage;
    myImage.open(FILE_NAME);

    if (myImage.fail())
    {
        cout << "Unable to create " << FILE_NAME << endl;
        return 0;
    }

    myImage << "P3" << endl;
    myImage << scene.mainCamera.imageResolution[0] << " " << scene.mainCamera.imageResolution[1] << endl;
    myImage << "255" << endl;

    float r;
    float b;
    float g;
    int pixel = 0;

    Vector3 normalizedGazeVector = unit_vector(scene.mainCamera.gaze);
    normalizedUpVector = unit_vector(scene.mainCamera.up);
    Vector3 w = -1 * normalizedGazeVector;
    Vector3 planeCenter = scene.mainCamera.position + normalizedGazeVector * scene.mainCamera.nearDistance;
    rightVector = cross(normalizedUpVector, w);
    normalizedUpVector = unit_vector(-scene.mainCamera.up);
    leftBottomPoint = scene.mainCamera.nearPlane[0] * rightVector + scene.mainCamera.nearPlane[2] * normalizedUpVector + planeCenter;
    
    horizontalDif = (float)(scene.mainCamera.nearPlane[1] - scene.mainCamera.nearPlane[0]) / (float)scene.mainCamera.imageResolution[0];
    verticalDif = (float)(scene.mainCamera.nearPlane[3] - scene.mainCamera.nearPlane[2]) / (float)scene.mainCamera.imageResolution[1];
    vector<thread> threads;

    int maxThreadCount = 8;
    int pixelsPerThreadCount = scene.mainCamera.imageResolution[0] * scene.mainCamera.imageResolution[1] / maxThreadCount;

    cout << "\nStarted rendering\n";
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < maxThreadCount; i++)
    {
        threads.push_back(thread(CalculatePixelValues, i, pixelsPerThreadCount));     
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\nRendering finished, Render time: " << duration.count() << endl;


    cout << "\nCreating " << FILE_NAME << endl;

    for (int i = 0; i < SIZE; i++) 
    {
        int value = imagePixels[i];
        myImage << value << " " << endl;
    }



    cout << "\nDONE creating " << FILE_NAME << endl;

    myImage.close();
    free(imagePixels);
    imagePixels = NULL;

    return 0;
}