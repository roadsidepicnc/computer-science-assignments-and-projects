#include<stdio.h>


int sum(int n, int flag);
int mult(int n, int flag);

int main()
{
	int num, flag1, flag2;		// integers for deciding operations.
	int i;
	int result;
	int result2;
	printf("Enter an integer :\n ");	// (Positive) Integer to be performed on.
	scanf("%d", &num);

	printf("Please enter '0' for summation , '1' for multiplication:\n ");
	scanf("%d", &flag1);

	switch (flag1)
	{
	case 0:
		printf("Please enter '0' for work on even numbers, '1' to work on odd numbers.\n ");		
		scanf("%d", &flag2);

		switch (flag2)
		{
		case 0:							// Summation for even numbers
			i = 2;
			for (i; i <= num; i += 2)
			{
				printf("%d", i);
				if (num % 2 == 0)
				{
					if (i != num)			//  This part is for not print more symbol than needed.
					{
						printf(" + ");
					}

				}
				else
				{
					if (i != num - 1)		//  This part is for not print more symbol than needed.
					{
						printf(" + ");
					}
				}
			}

			result = sum(num, flag2);
			printf(" = %d", result);
			break;

		case 1:						// Summation for odd numbers
			i = 1;
			for (i; i <= num; i += 2)
			{
				printf("%d", i);
				if (num % 2 == 0)
				{
					if (i != num -1)		//  This part is for not print more symbol than needed.
					{
						printf(" + ");		
					}

				}
				else
				{
					if (i != num)			//  This part is for not print more symbol than needed.
					{
						printf(" + ");
					}
				}
			}

			result = sum(num, flag2);
			printf(" = %d", result);
			break;
	
			default :							// Prints error message if user entered invalid number.
			printf("Invalid value for even/odd selection.");

		}
		break;

	case 1:
		printf("Please enter '0' for work on even numbers, '1' to work on odd numbers.\n ");
		scanf("%d", &flag2);

		switch (flag2)
		{
		case 1:
			i = 1;
			for (i; i <= num; i += 2)
			{
				printf("%d", i);
				if (num % 2 == 0)
				{
					if (i != num - 1)		//  This part is for not print more symbol than needed.
					{
						printf(" * ");
					}
					
				}
				else
				{
					if (i != num)			//  This part is for not print more symbol than needed.
					{
						printf(" * ");
					}
				}
			}
				result2 = mult(num, flag2);
				printf(" = %d\n", result2);
				mult (num,flag2);
				break;
			
		case 0:
			i = 2;
			for (i; i <= num; i += 2)
			{
				printf("%d", i);

				if (num % 2 == 0)
				{
					if (i != num)				//  This part is for not print more symbol than needed.
					{
						printf(" * ");
					}

				}
				else
				{
					if (i != num -1)			//  This part is for not print more symbol than needed.
					{
						printf(" * ");
					}
				}
			}

			result2 = mult(num, flag2);
			printf(" = %d", result2);
			break;
			default :
			printf("Invalid value for even/odd selection.");		// Prints error message if user entered invalid number.

		}
		
		break;
	default :
		printf("Unsupported operation.");					// Prints error message if user entered invalid number.
	}
	return 0;
}






int sum(int n, int flag)		// Summation function
{
	switch (flag)
	{
		int result_sum;
	case 1:
		result_sum = 0;
		int i = 1;			// Summation for odd numbers
		for (i; i <= n; i += 2)
		{
			result_sum += i;
		}
		
		return result_sum;
		break;

	case 0:					// Summation for even numbers
		result_sum = 0;
		i = 2;
		for (i; i <= n; i += 2)
		{
			result_sum += i;
		}

		return result_sum;
		break;

	
	}
	
}

int mult(int n, int flag)		// Multiplication function
{
	switch (flag)
	{
		int result_mult;
	case 1:				// Multiplication for odd numbers
		result_mult = 1;
		int i = 1;

		for (i; i <= n; i += 2)
		{
			result_mult *= i;
		}
		
		return result_mult;
		break;

	case 0:				// Multiplication for even numbers
		result_mult = 1;
		i = 2;
		for (i; i <= n; i += 2)
		{
			result_mult *= i;
		}
		
		return result_mult;

		break;

	
	}
	
}