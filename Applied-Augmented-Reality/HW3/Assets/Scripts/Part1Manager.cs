using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class Part1Manager : MonoBehaviour
{
    [SerializeField] private HomographyManager homographyManager;
    
    private double[,] s0;
    private double[,] d0;
    private double[,] h0;
    public double[,] s1;
    public double[,] d1;
    public double[,] d2;
    public double[,] d3;
    private double[,] h1;
    private double[,] h2;
    private double[,] h3;
    private List<double[,]> points;
    private List<double[,]> realPoints;
    
    private void Start()
    {
        s0 = new double[4, 2] { { 3, 4 }, { 6, 3 }, { 4, 12 }, { 1, 7 } };
        d0 = new double[4, 2] { { 3, 4 }, { 6, 3 }, { 4, 12 }, { 1, 7 } };
        
        s1 = new double[5, 2] { { 100, 200 }, { 400, 100 }, { 200, 300 }, { 400, 400 }, { 500, 300 } };
        d1 = new double[5, 2] { { 688, 1119 }, { 1378, 885 }, { 924, 1350 }, { 1381, 1576 } , { 1610, 1345 } };
        d2 = new double[5, 2] { { 488, 1121 }, { 1183, 860 }, { 720, 1352 }, { 1190, 1581 }, { 1428, 1338 } };
        d3 = new double[5, 2] { { 721, 1008 }, { 1360, 823 }, { 952, 1224 }, { 1397, 1436 }, { 1593, 1229 } };
        
        points = new List<double[,]>();
        realPoints = new List<double[,]>();
        
        Part1_1();
        Part1_3();
        Part1_4();
        Part1_5();
        Part1_6();
        Part1_7();
    }

    private void Part1_1()
    {
        Debug.Log("Part 1.1");
        h0 = homographyManager.CalculateHomographyMatrix(s0, d0);
        PrintHomographyMatrix(h0);
    }
    
    private void Part1_3()
    {
        Debug.Log("Part 1.3");
        double[,] x_y = new double[3, 1] { { s0[2,0] }, { s0[2,1] } , { 1 } };
        homographyManager.CalculateProjection(h0, x_y);
    }
    
    private void Part1_4()
    {
        Debug.Log("Part 1.4");
        double[,] u_v = new double[3, 1] { { d0[1, 0] }, { d0[1, 1] }, { 1 } };
        homographyManager.CalculateInverseProjection(h0, u_v);
    }

    private void Part1_5()
    {
        Debug.Log("Part 1.5");
        h1 = homographyManager.CalculateHomographyMatrix(s1, d1);
        h2 = homographyManager.CalculateHomographyMatrix(s1, d2);
        h3 = homographyManager.CalculateHomographyMatrix(s1, d3);
        points.Add(new double[3,1]{{300},{200},{1}});
        points.Add(new double[3,1]{{400},{200},{1}});
        points.Add(new double[3,1]{{300},{400},{1}});
        Debug.Log("Image 1:");
        realPoints.Add(new double[3, 1] { { 1152 }, { 1117 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 1377 }, { 1115 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 1155 }, { 1579 }, { 1 } });
        Part1_5Helper(h1);
        Debug.Log("Image 2:");
        realPoints = new List<double[,]>();
        realPoints.Add(new double[3, 1] { { 949 }, { 1108 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 1185 }, { 1101 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 956 }, { 1585 }, { 1 } });
        Part1_5Helper(h2);
        Debug.Log("Image 3:");
        realPoints = new List<double[,]>();
        realPoints.Add(new double[3, 1] { { 1161 }, { 1018 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 1372 }, { 1022 }, { 1 } });
        realPoints.Add(new double[3, 1] { { 1183 }, { 1438 }, { 1 } });
        Part1_5Helper(h3);
    }

    private void Part1_6()
    {
        Debug.Log("Part 1.6");
        double[,] p1 = new double[3, 1] { { 7.5f }, { 5.5f }, { 1 } };
        double[,] p2 = new double[3, 1] { { 6.3f }, { 3.3f }, { 1 } };
        double[,] p3 = new double[3, 1] { { 0.1f }, { 0.1f }, { 1 } };
            
        Debug.Log("Image 1:");
        homographyManager.CalculateProjection(h1, p1);
        homographyManager.CalculateProjection(h1, p2);
        homographyManager.CalculateProjection(h1, p3);
        Debug.Log("Image 2:");
        homographyManager.CalculateProjection(h2, p1);
        homographyManager.CalculateProjection(h2, p2);
        homographyManager.CalculateProjection(h2, p3);
        Debug.Log("Image 3:");
        homographyManager.CalculateProjection(h3, p1);
        homographyManager.CalculateProjection(h3, p2);
        homographyManager.CalculateProjection(h3, p3);
    }
    
    private void Part1_7()
    {
        Debug.Log("Part 1.7");
        double[,] i1 = new double[3, 1] { { 500 }, { 400 }, { 1 } };
        double[,] i2 = new double[3, 1] { { 86 }, { 167 }, { 1 } };
        double[,] i3 = new double[3, 1] { { 10 }, { 10 }, { 1 } };

        Debug.Log("Image 1:");
        homographyManager.CalculateInverseProjection(h1, i1);
        homographyManager.CalculateInverseProjection(h1, i2);
        homographyManager.CalculateInverseProjection(h1, i3);
        Debug.Log("Image 2:");
        homographyManager.CalculateInverseProjection(h2, i1);
        homographyManager.CalculateInverseProjection(h2, i2);
        homographyManager.CalculateInverseProjection(h2, i3);
        Debug.Log("Image 3:");
        homographyManager.CalculateInverseProjection(h3, i1);
        homographyManager.CalculateInverseProjection(h3, i2);
        homographyManager.CalculateInverseProjection(h3, i3);
    }
    
    private void Part1_5Helper(double[,] h)
    {
        int k = 0;
        PrintHomographyMatrix(h);
        foreach (double[,] x_y in points)
        {
            double[,] u_v = realPoints[k++];
            double[,] result = homographyManager.CalculateProjection(h, x_y);
            Debug.Log("Error percentage: % " + CalculateErrorAmount(result, u_v));
        }
    }
    
    private float CalculateErrorAmount(double[,] points, double[,] realPoints)
    {
        float resDif = Mathf.Sqrt(Mathf.Pow((float)(points[0, 0]), 2) + Mathf.Pow((float)(points[1, 0]), 2));
        float actualDif = Mathf.Sqrt(Mathf.Pow((float)(realPoints[0, 0]), 2) + Mathf.Pow((float)(realPoints[1, 0]), 2));
        return Mathf.Abs((actualDif - resDif) / actualDif * 100);
    }

    private void PrintHomographyMatrix(double[,] h)
    {
        int horizontalLimit = h.GetLength(0);
        int verticalLimit = h.GetLength(1);
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.AppendLine("Calculated Homography Matrix:\n");
        for (int i = 0; i < horizontalLimit; i++)
        {
            for (int j = 0; j < verticalLimit; j++)
            {
                stringBuilder.Append(h[i, j] + "   ");
            }

            stringBuilder.AppendLine();
        }

        Debug.Log(stringBuilder);
    }
    
}