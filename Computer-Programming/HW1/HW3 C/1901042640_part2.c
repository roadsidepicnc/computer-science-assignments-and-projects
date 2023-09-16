#include<stdio.h>
#include<math.h>

int isprime(int a);

int main()
{
	int k, num;
	printf("Please enter an integer :\n");
	scanf("%d", &num);

	for (k = 2; k < num; k++)		// Loop for every number between 2 and entered number.
	{
		if (isprime(k) == 1)					// Checks if function returned 1.If it is true prints it is a prime number.
		{
			printf("%d is a prime number.", k);		
		}
			else	
			{						// Checks if function returned different number than 1.If it is true prints it is not a prime number and it's least divider.		{
				printf("%d is not a prime number.It is divided by %d .", k,isprime(k));
			}
		
		printf("\n");
	}

	return 0;
}

int isprime(int a)		// Prime Number Function
{

int i, check;
check = 1;

	for (i = 2; i <= sqrt(a); i++)			// Checks every number from 2 to sqrt(a)
	{
		if (a % i == 0)
		{
		check = 0;
		break;
			
		}
	}
	if (check == 1)		// If number is not prime returns least divider.
	{
	return 1;
	}
		else
		{
		return i;	// If number is prime returns itself.
		}

}