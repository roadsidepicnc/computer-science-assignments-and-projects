#include<stdio.h>

int main()
{
	int num, cur,tl;
	

	printf("***** Welcome to ABC Exchange Office *****\n\n");
	printf("Enter your amount :\n");
	scanf("%d", &num);

if ( num >= 0 )				// Checks if enterd number is zero or positve.
{
	printf("Please select your currency :\n1. Turkish Liras\n2. Euro\n3. Dollar\n");
	scanf("%d", &cur);

	


	switch (cur)
	{
	case 1 :
		printf("You have %d Turkish Liras.\n",num);
		printf("Choose which currency you want to convert\n");
		scanf("%d", &tl);
		switch (tl)
		{
		case 2 :
			printf("You have %f Euro\n", num/6.69);							// 1 Euro is 6.69 TL.
			break;											// 1 Dollar is 6.1106 TL.
		case 3:												// 1 Euro is 1.1188 Dollars.
			printf("You have %f Dollars\n", num / 6.14);		
			break;
		default:
			printf("Your Selection is invalid.");
			break;
		}
		break;
	
	case 2:
		printf("You have %d Euros.\n", num);
		printf("Choose which currency you want to convert\n");
		scanf("%d", &tl);
		switch (tl)
		{
		case 1:
			printf("You have %f Turkish Liras\n", num* 6.69);
			break;
		case 3:
			printf("You have %f Dollars\n", num * 1.1188);
			break;
		default:
			printf("Your Selection is invalid.");
			break;
		}
		break;

	case 3:
		printf("You have %d Dollars.\n", num);
		printf("Choose which currency you want to convert\n");
		scanf("%d", &tl);
		switch (tl)
		{
		case 1:
			printf("You have %f Turkish Liras\n", num *6.14);
			break;
		case 2:
			printf("You have %f Euros\n", num / 1.1188);
			break;
		default:
			printf("Your Selection is invalid.");
			break;
		}
		break;

		default:				// If user enters a wrong number at first section prints an error message.
		printf("Your Selection is invalid.");		
		break;
	
	}

}
else
{
printf("You entered a negative number.");		// If entered number is negative prints an error message.
}


return 0;
}