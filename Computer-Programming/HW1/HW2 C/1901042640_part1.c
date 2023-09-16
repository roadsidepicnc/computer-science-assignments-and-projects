#include<stdio.h>
#include<math.h>

int draw_triangle(int side1, int side2, int side3);
void type_triangle(int side1, int side2, int side3);
int perimeter_triangle(int side1, int side2, int side3);
double area_triangle(int side1, int side2, int side3, int perimeter);

int main()
{
			int side1, side2, side3;
			printf("Length of the first side :\n");		// Length of the sides.
			scanf("%d", &side1);
			printf("Length of the first side :\n");
			scanf("%d", &side2);
			printf("Length of the first side :\n");
			scanf("%d", &side3);

			if (draw_triangle(side1,side2,side3) == 1)
			{
				printf("According to the triangle inequality theorem  , this triangle can be drawn.\n");
				type_triangle(side1, side2, side3);
				perimeter_triangle(side1, side2, side3);
				printf("The perimeter of the triangle is : %d\n", perimeter_triangle(side1, side2, side3));
				printf("The area of the triangle is : %lf\n", area_triangle(side1,side2,side3, perimeter_triangle(side1, side2, side3)));
			}
			if (draw_triangle(side1, side2, side3) == 0)
			{
				printf("According to the triangle inequality theorem  , this triangle can not be drawn.\n");	// Prints error if entered numbers are not appropriate to draw a triangle.
			}

	return 0;
}

int draw_triangle(int side1, int side2, int side3)
{
	if (side1 + side2 > side3 && side2 + side3 > side1 && side1 + side3 > side2)		// Checks if entered numbers are appropriate to draw a triangle.
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void type_triangle(int side1, int side2, int side3)
{
	if (side1 == side2 && side1 == side3)				// If all side's length is equal to each other.Then triangle is equileteral.
	{
		printf("It is an equileteral triangle\n");
	}
	else if (side1 != side2 && side1 != side3 && side2 != side3)	// If any of the side's length is not equal to any other length.Then triangle is scaelene.
	{
		printf("It is a scaelene triangle\n");
	}
	else
	{
		printf("It is an isosceles triangle\n");		// Only option left is only 2 lengths are equal to each other.So,triangle is isosceles.
	}
}

int perimeter_triangle(int side1, int side2, int side3)
{
	return side1 + side2 + side3;					// Calculates perimeter of the triangle.
}

double area_triangle(int side1, int side2, int side3, int perimeter)
{
	double area;
	double per = perimeter;
	double p = per/ 2;						// Calculates area of the triangle.
	area = sqrt(p * (p - side1) * (p - side2) * (p - side3));
		return area;
}