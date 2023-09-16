using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using Accord.Math;

using Vector3 = Accord.Math.Vector3;

public class RandomManager : MonoBehaviour
{
    [SerializeField] private string firstFileName;
    [SerializeField] private string secondFileName;
    [SerializeField] private Transform inputParent;
    [SerializeField] private Transform outputParent;
    [SerializeField] private Transform rotatedParent;
    [SerializeField] private Transform transformedParent;
    [SerializeField] private GameObject sphere;
    [SerializeField] private Material blueMaterial;
    [SerializeField] private Material redMaterial;
    [SerializeField] private Material greenMaterial;


    private int N;
    private List<Vector3> setA;
    private List<Vector3> setB;
    private List<Vector3> transformedSetA;
    private List<Vector3> finalRotatedSetA;
    private List<Vector3> finalTransformedSetA;
    private List<Vector3> tempPointListA;
    private List<Vector3> tempPointListB;

    private Matrix3x3 R;
    private Vector3 T;

    #region Ransac Variables

    [SerializeField] private int maxIterations;
    [SerializeField] private float ransacThreshold;

    #endregion

    private void Start()
    {
        tempPointListA = new List<Vector3>();
        tempPointListB = new List<Vector3>();
        transformedSetA = new List<Vector3>();
        finalTransformedSetA = new List<Vector3>();
        finalRotatedSetA = new List<Vector3>();
        ReadInputFiles(firstFileName, secondFileName);
        Main();
        InstantiateObjects();
        inputParent.gameObject.SetActive(false);
        outputParent.gameObject.SetActive(false);
        transformedParent.gameObject.SetActive(false);
        rotatedParent.gameObject.SetActive(false);
        DrawLines();
    }

    
    private void Main()
    {
        for(int i = 0; i < maxIterations; i++)
        {
            CalculateRigidTransformations();
            ApplyRigidTransformation();
            int ransac = CalculateRansac();

            if (ransac >= setA.Count / 2)
            {
                Debug.Log("Succesfully found the result in " + i + " tries");
                return;
            }
        }

        Debug.Log("Failed to find the result");
    }
    

    private void PickRandomPoints(List<Vector3> pointList, List<Vector3> tempPointList)
    {
        tempPointList.Clear();
        int rand = Random.Range(0, pointList.Count);
        while (tempPointList.Count < 3)
        {
            if (!tempPointList.Contains(pointList[rand]))
            {
                tempPointList.Add(pointList[rand]);
            }

            rand = Random.Range(0, pointList.Count);
        }
    }

    private void ReadInputFiles(string firstFileName, string secondFileName)
    {
        setA = LoadPoints(firstFileName);
        setB = LoadPoints(secondFileName);
    }

    private List<Vector3> LoadPoints(string fileName)
    {
        TextAsset textAsset = Resources.Load<TextAsset>(fileName);
        StringReader reader = new StringReader(textAsset.text);
        List<Vector3> pointList = new List<Vector3>();
        int counter = 0;
        while (reader.Peek() != -1)
        {
            string line = reader.ReadLine();

            string[] items = line.Split(' ');
            if (counter == 0)
            {
                N = int.Parse(items[0]);
            }
            else
            {
                Vector3 point = new Vector3(float.Parse(items[0]), float.Parse(items[1]), float.Parse(items[2]));
                pointList.Add(point);
            }

            counter++;
        }

        return pointList;
    }

    private Vector3 CalculateCentroid(List<Vector3> list)
    {
        Vector3 result = new Vector3(0, 0, 0);
        foreach (Vector3 vec in list)
        {
            result += vec;
        }

        return result / list.Count;
    }

    private async void CalculateRigidTransformations()
    {
        PickRandomPoints(setA, tempPointListA);
        PickRandomPoints(setB, tempPointListB);

        Vector3 centroidA = CalculateCentroid(tempPointListA);
        Vector3 centroidB = CalculateCentroid(tempPointListB);
        for (int i = 0; i < 3; i++)
        {
            tempPointListA[i] -= centroidA;
            tempPointListB[i] -= centroidB;
        }

        Matrix3x3 matrixA;
        Matrix3x3 matrixB;
        Matrix3x3 H;
        matrixA = Matrix3x3.CreateFromColumns(tempPointListA[0], tempPointListA[1], tempPointListA[2]);
        matrixB = Matrix3x3.CreateFromColumns(tempPointListB[0], tempPointListB[1], tempPointListB[2]);
        H = matrixA * (matrixB.Transpose());
        Matrix3x3 u;
        Matrix3x3 v;
        Vector3 e;
        H.SVD(out u, out e, out v);
        R = v * (u.Transpose());
        if (R.Determinant < 0)
        {
            v.V20 = -1;
            v.V21 = -1;
            v.V22 = -1;
            R = v * u.Transpose();
        }

        T = centroidB - (R * centroidA);
    }

    private UnityEngine.Vector3 ConvertToUnityVector3(Vector3 vector)
    {
        return new UnityEngine.Vector3(vector.X, vector.Y, vector.Z);
    }

    private Vector3 ConvertToAccordVector3(UnityEngine.Vector3 vector)
    {
        return new Vector3(vector.x, vector.y, vector.z);
    }

    private void DrawLines()
    {
        for(int i = 0; i < rotatedParent.childCount; i++)
        {
            Debug.DrawLine(rotatedParent.GetChild(i).position, transformedParent.GetChild(i).position, Color.magenta, 100);
        }
    }

    private void ApplyRigidTransformation()
    {
        transformedSetA.Clear();
        foreach(Vector3 vec3 in setA)
        {
            Vector3 tempVec3 = Matrix3x3.Multiply(R, vec3) + T;
            transformedSetA.Add(tempVec3);
        }

    }

    private int CalculateRansac()
    {
        int inlierCount = 0;
        finalRotatedSetA.Clear();
        finalTransformedSetA.Clear();
        for (int i = 0; i < transformedSetA.Count; i++)
        {
            foreach (Vector3 v2 in setB)
            {
                if (CalculateDistanceBetweenPoints(transformedSetA[i], v2) <= ransacThreshold)
                {
                    Vector3 tempVec1 = Matrix3x3.Multiply(R, setA[i]) + T;
                    finalTransformedSetA.Add(tempVec1);
                    Vector3 tempVec2 = Matrix3x3.Multiply(R, setA[i]);
                    finalRotatedSetA.Add(tempVec2);
                    inlierCount++;
                }
            }
        }

        return inlierCount;
    }

    private void InstantiateObjects()
    {
        foreach(Vector3 v in setA)
        {
            GameObject tempObject = Instantiate(sphere, ConvertToUnityVector3(v), Quaternion.identity);
            tempObject.transform.SetParent(inputParent);
        }
    
        foreach (Vector3 v in setB)
        {
            GameObject tempObject = Instantiate(sphere, ConvertToUnityVector3(v), Quaternion.identity);
            tempObject.GetComponent<Renderer>().material = greenMaterial;
            tempObject.transform.SetParent(outputParent);
        }

        foreach (Vector3 v in finalRotatedSetA)
        {
            GameObject tempObject = Instantiate(sphere, ConvertToUnityVector3(v), Quaternion.identity);
            tempObject.GetComponent<Renderer>().material = blueMaterial;
            tempObject.transform.SetParent(rotatedParent);
        }

        foreach (Vector3 v in finalTransformedSetA)
        {
            GameObject tempObject = Instantiate(sphere, ConvertToUnityVector3(v), Quaternion.identity);
            tempObject.GetComponent<Renderer>().material = redMaterial;
            tempObject.transform.SetParent(transformedParent);
        }
    }

    private float CalculateDistanceBetweenPoints(Vector3 vec1, Vector3 vec2)
    {
        float distance = Mathf.Sqrt(Mathf.Pow(vec1.X - vec2.X, 2) + Mathf.Pow(vec1.Y - vec2.Y, 2) + Mathf.Pow(vec1.Z - vec2.Z, 2));
        return distance;
    }

    public void InputOnOff()
    {
        bool temp = inputParent.gameObject.activeSelf;
        inputParent.gameObject.SetActive(!temp);
    }

    public void OutputOnOff()
    {
        bool temp = outputParent.gameObject.activeSelf;
        outputParent.gameObject.SetActive(!temp);
    }

    public void RotateOnOff()
    {
        bool temp = rotatedParent.gameObject.activeSelf;
        rotatedParent.gameObject.SetActive(!temp);
    }

    public void TransformOnOff()
    {
        bool temp = transformedParent.gameObject.activeSelf;
        transformedParent.gameObject.SetActive(!temp);
    }
}