#include<stdio.h>


int andop(int a, int b);

int main()
{
	int num1, num2; 

	int x1 = 1;		// Integers for checking if entered numbers are binary.
	int x2 = 1;

	int check1 = 0;		
	int check2 = 0;

	int digit1 = 1;
	int digit2 = 0;

	int backup1;
	int backup2;
	
	while ((x1 == 1 || x2 == 1) || digit1 != digit2)	// Ask for new numbers until user enters numbers in correct form.
	{
		printf("First Integer :\n");
		scanf("%d", &num1);
		printf("Second Integer :\n");
		scanf("%d", &num2);

		backup1 = num1;
		backup2 = num2;
		
		check1 = 0;			// Integers for checking if entered numbers are binary.
		check2 = 0;

		digit1 = 0;			// Integers for checking digit numbers.
		digit2 = 0;

		while (num1 > 0)
		{
			
			if (check1 == 0 || check1 == 1)			// Checks if first entered number is in binary form.
			{
				check1 = num1 % 10;
				
				x1 = 0;	
							
			}
				else
				{
				x1 = 1;
				}
		num1 /= 10;
		digit1 += 1;				// Calculates length of the first number.
		
		}
	
		while (num2 > 0)					// Checks if second entered number is in binary form.
		{
			if (check2 == 0 || check2 == 1)		
			{
				check2 = num2 % 10;
				
				x2 = 0;
							
			}
				else
				{
				x2 = 1;
				}
		num2 /= 10;				
		digit2 += 1;				// Calculates length of the second number.
		
		}

		if (x1 == 1 || x2 == 1)
		{
		printf("Integers should be binary,please enter 2 new integers.\n");		// Prints error message
		}
			if (digit1 != digit2)
			{
			printf("Integers should have the same length,please enter 2 new integers.\n");		// Prints error message
			}

	}

	printf("%d AND %d = %d\n", backup1, backup2,andop(backup1, backup2));

	return 0;
	
	}

int andop(int a, int b)			// Function
{
	
	int i = 0;
	int c = 0;
	int length = 0;
	int a_backup = a;
	int d = 1;

	while (a_backup > 0)		// Calculates length of the one of the numbers.	
	{
		a_backup /= 10;
		length += 1;
	}

		while (i <= length)
		{
			if (a % 10 + b % 10 == 2)	// If summation of the numbers on same digit is equal to 2.Then adds 1 to same digit at result.
			{
			c = c + d;		
			}

		a /= 10;			
		b /= 10;
		i++;
		d *= 10;			
		}

return c;
}