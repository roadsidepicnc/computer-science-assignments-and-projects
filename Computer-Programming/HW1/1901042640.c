#include<stdio.h>
#include<math.h>

void find_root();
void find_newtonian_root();
int find_multiple_closest(int a, int b);


int main()
{
	int num1, num2, max, min;	// Integers for Part 3
	// Part 1
	find_root();
	// Part 2
	find_newtonian_root();
	// Part 3
	printf("Please enter the first integer :");
	scanf("%d", &num1);
	printf("Please enter the second integer :");
	scanf("%d", &num2);
	if (num1 < num2)		// I find max and min of the entered numbers so i can find if answer is an entered number.
	{						// Also, i need that for printing.
		max = num2;				
		min = num1;				
	}
	else
	{
		max = num1;
		min = num2;
	}
	
	if (find_multiple_closest(num1,num2) == max)
	{
		printf("Closest number to %d that is multiple of %d is itself\n", max, min);
	}
	else if (min % 2 == 0 && abs(find_multiple_closest(num1, num2)-max) == min / 2)		// Checks if there are 2 answers with same distance to max number.
	{
		printf("Closest numbers to %d that is multiple of %d are %d and %d.Their distance to %d is equal.\n", max, min, find_multiple_closest(num1, num2), find_multiple_closest(num1, num2) + min,max); 
	}
	
	else

	{
		printf("Closest number to %d that is multiple of %d is %d\n", max, min, find_multiple_closest(num1, num2));
	}

	return 0;
}

void find_root()
{
	int a, b, c;			// Integers for formula
	float delta,x1, x2;
	printf("Please enter the first coefficent :\n");
	scanf("%d", &a);
	printf("Please enter the second coefficent :\n");
	scanf("%d", &b);
	printf("Please enter the third coefficent :\n");
	scanf("%d", &c);
	delta = b * b - 4 * a * c;   // I calculate delta in order to decide if equation has real root or not.
	if (delta < 0)
	{
		printf("Your equation (%d)x^2 + (%d)x +(%d) does not have any real roots.\n", a,b,c);
	}
	
	else
	{
		x1 = (-b + sqrt(delta)) / (2 * a);
		x2 = (-b - sqrt(delta)) / (2 * a);
		printf("Your equation (%d)x^2 + (%d)x +(%d) have real roots {%f,%f}\n", a,b,c,x1,x2);
		
	}

}

void find_newtonian_root()
{
	int i;
	float a, b, c,initial,x;
	printf("Please enter the first coefficent :\n");
	scanf("%f", &a);
	printf("Please enter the second coefficent :\n");
	scanf("%f", &b);
	printf("Please enter the third coefficent :\n");
	scanf("%f", &c);
	printf("Please enter the initial :\n");
	scanf("%f", &initial);

	// Real Root
	float delta = b * b - 4 * a * c;
	float x1 = (-b + sqrt(delta)) / 2 * a;		// There may be 2 roots . I calculate one of them and proceed.I don't calculate the other one.
	

	x = initial;
	float parabola = a * x * x + b * x + c;
	float derivative = 2 * a * x + b;

		printf("Step        x             f(x)       Difference\n");
		x = x - (parabola / derivative);
		parabola = a * x * x + b * x + c;
		derivative = 2 * a * x + b;
		
		printf("x1      %f      %f      %f\n", x, parabola, x - x1);
		x = x - (parabola / derivative);
		parabola = a * x * x + b * x + c;
		derivative = 2 * a * x + b;
		
		printf("x2      %f      %f      %f\n", x, parabola, x - x1);
		x = x - (parabola / derivative);
		parabola = a * x * x + b * x + c;
		derivative = 2 * a * x + b;
		
		printf("x3      %f      %f      %f\n", x, parabola, x - x1);
		x = x - (parabola / derivative);
		parabola = a * x * x + b * x + c;
		derivative = 2 * a * x + b;
		
		printf("x4      %f      %f      %f\n", x, parabola, x - x1);
		x = x - (parabola / derivative);
		parabola = a * x * x + b * x + c;
		derivative = 2 * a * x + b;
	
		printf("x5      %f      %f      %f\n", x, parabola, x - x1);
		printf("The root is %f\n", x);

}

int find_multiple_closest(int a, int b)
{
	int max;
	int min;
	int div;
	int check1;
	int check2;

	if (a < b)
	{
		max = b;
		min = a;
	}
	else
	{
		max = a;
		min = b;
	}
	if (max % min == 0)
	{
		return max;
		}
	else
	{
		div = max / min;
		check1 = min * div;
		check2 = check1 + min;
		if (max - check1 > check2 - max)
		{
			return check2;
		}
		else		
		{
			return check1;
		}

	}

}
