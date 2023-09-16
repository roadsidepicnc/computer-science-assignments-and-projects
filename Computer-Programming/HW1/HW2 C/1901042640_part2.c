#include<stdio.h>
#include<math.h>

int number_length(int number);
int find_digit(int number, int index);

int main()
{
	int num1, num2;
	printf("Enter a number (maximum 6 digits) :\n");
	scanf("%d", &num1);
	if (number_length(num1) != 0 && num1 > 0 && number_length(num1) <= 6)
	{
		printf("Your number has %d digits.\n", number_length(num1));
		printf("When your number is written 100 times next to each other , which digit you would like to see ?  : \n");
		scanf("%d", &num2);
		if (num2 <= 600)			// Max allowed number is 6 * 100 = 600 ( 6 from max digit , 100 from maximum numbers put next to each other.)
		{
			printf("%d.th digit of the big number sequence :%d\n", num2, find_digit(num1, num2));
		}
		else
		{
			printf("You entered bigger number than allowed maximum number.");		// Prints error message.
		}
	}

	else
	{
		printf("Entered number doesn't fulfill the requirements ");				// Prints error message.
	}
	return 0;
}

int number_length(int number)
{
	int digits = log10(number) + 1;				// I used base 10 logarithm function in order to calculate digits of the number.

	return digits;
}
	

int find_digit(int number, int index)
{
	int x, y, z,result,exp;
	x = number_length(number);				//  I call previous function to find number length.
	index = index % x;						//  I divide index to number length.Then, extract result from number length.
	exp = x - index;

	y = pow(10, exp);
	if (index != 0)							// I check if index is not multiple of number length.
	{
		number = number / y;

		result = number % 10;
		result = result / 10;

		return index;
	}
	else
	{
		return (number % 10);			// If index is multiple of number length.I directly return the rightmost digit of the number.
	}


}
