using System.Text;
using MathNet.Numerics.LinearAlgebra;
using UnityEngine;
using MathNet.Numerics.LinearAlgebra.Double;
using MathNet.Numerics.LinearAlgebra.Factorization;

public class HomographyManager : MonoBehaviour
{
    public double[,] CalculateHomographyMatrix(double[,] s, double[,] d)
    {
        double[,] h = new double[3, 3];
        int row = 0;
        double[] temp = CalculateHomographyMatrixHelper(s, d);
        for (int i = 0; i < temp.Length; i++)
        {
            if (i != 0 && i % 3 == 0)
            {
                row++;
            }
                
            h[row, i % 3] = temp[i];
        }
        
        return h;
    }
    
    private double[] CalculateHomographyMatrixHelper(double[,] s, double[,] d)
    {
        int currentPoint = 0;
        int horizontalLimit = 9;
        int verticalLimit = s.GetLength(0);
        double[,] a= new double[2 * verticalLimit, horizontalLimit];
        for (int i = 0; i < 2 * verticalLimit; i++)
        {
            if (i % 2 == 0)
            {
                a[i, 0] = s[currentPoint, 0] * (-1);
                a[i, 1] = s[currentPoint, 1] * (-1);
                a[i, 2] = -1;
                a[i, 3] = 0;
                a[i, 4] = 0;
                a[i, 5] = 0;
                a[i, 6] = s[currentPoint, 0] * d[currentPoint, 0];
                a[i, 7] = d[currentPoint, 0] * s[currentPoint, 1];
                a[i, 8] = d[currentPoint, 0];
            }
            else
            {
                a[i, 0] = 0;
                a[i, 1] = 0;
                a[i, 2] = 0;
                a[i, 3] = s[currentPoint, 0] * (-1);
                a[i, 4] = s[currentPoint, 1] * (-1);
                a[i, 5] = -1;
                a[i, 6] = s[currentPoint, 0] * d[currentPoint, 1];
                a[i, 7] = d[currentPoint, 1] * s[currentPoint, 1];
                a[i, 8] = d[currentPoint, 1];
                currentPoint++;
            }
        }
        
        return CalculateHomographyMatrixHelperHelper(a);
    }

    private double[] CalculateHomographyMatrixHelperHelper(double[,] a)
    {
        DenseMatrix matrix = DenseMatrix.OfArray(a);
        Svd<double> svd = matrix.Svd(true);
        double[] result = svd.VT.Row(svd.VT.RowCount - 1).ToArray();
        return result;
    }
    
    public double[,] CalculateProjection(double[,] h, double[,] x_y)
    {
        double[,] result = ImplementHomography(h, x_y);
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.AppendLine("Homography is calculated:");
        stringBuilder.AppendLine("(x,y): " + x_y[0, 0] + "  " + x_y[1, 0] + "  " + x_y[2, 0]);
        stringBuilder.AppendLine("(u,v): " + result[0, 0] + "  " + result[1, 0] + "  " + result[2, 0]);
        Debug.Log(stringBuilder);
        return result;
    }
    
    public double[,] CalculateInverseProjection(double[,] h, double[,] u_v)
    {
        double[,] result = ImplementInverseHomography(h, u_v);
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.AppendLine("Inverse homography is calculated:");
        stringBuilder.AppendLine("(x,y): " + result[0, 0] + "  " + result[1, 0] + "  " + result[2, 0]);
        stringBuilder.AppendLine("(u,v): " + u_v[0, 0] + "  " + u_v[1, 0] + "  " + u_v[2, 0]);
        Debug.Log(stringBuilder);
        return result;
    }
    
    private double[,] ImplementHomography(double[,] h, double[,] a)
    {
        double[,] temp = CalculateMatrixMultiplication(h, a);
        double[,] result = new double[,] { { temp[0, 0] / temp[2, 0] }, { temp[1, 0] / temp[2, 0] }, { 1 } };
        return result;
    }
    
    private double[,] ImplementInverseHomography(double[,] h, double[,] a)
    {
        Matrix<double> m = DenseMatrix.OfArray(h).Inverse();
        double[,] temp = m.ToArray();
        return ImplementHomography(temp, a);
    }
    
    public double[,] CalculateMatrixMultiplication(double[,] matrix1, double[,] matrix2)
    {
        int horizontalLimit = matrix1.GetLength(0);
        int verticalLimit = matrix2.GetLength(1);
        int limit = matrix1.GetLength(1);
        double[,] result = new double[horizontalLimit, verticalLimit];
        for (int i = 0; i < horizontalLimit; i++)
        {
            for (int j = 0; j < verticalLimit; j++)
            {
                result[i, j] = 0;
                for (int k = 0; k < limit; k++)
                {
                    result[i, j] += matrix1[i, k] * matrix2[k, j];
                }
            }
        }
        
        return result;
    }
}