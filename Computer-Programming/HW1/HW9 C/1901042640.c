#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define DESTINATION_X 4		// Home Cordinates for first part
#define DESTINATION_Y 0

struct Hospital 		// For second part
{
char* name;
char citiesServed[3];
};


struct Hospital results[4];
#define NUM 4		// Number of elements on structure 



struct card 		// For third part
{
const char *face;
const char *suit;
};

struct card picked[52];

int numPathsHome(int street, int avenue);
int if_found (char string1[],struct Hospital results[4]);
int factorial (int n);
void combination(int n, int r, int arr1[], int size, int arr2[10][10]);
int canOfferCovidCoverage(char _cities[6], char _locations[4][10], int _numHospitals, struct Hospital results[4]);
void random_number_generator (int random_number_generator[], int num);


int main()
{
int i,k;
int choice;		// menu choice

// For first part
int street, avenue, result;		

// For second part
int max_hospital;				
char cities[6] = {'A','B','C','D','E','F'};
char locations[4][10];
strcpy(locations[0] ,"ABC");
strcpy(locations[1] ,"ACD");
strcpy(locations[2] ,"BF");
strcpy(locations[3] ,"CEF");

results[0].name = "Hospital- 1";
results[1].name = "Hospital- 2";
results[2].name = "Hospital- 3";
results[3].name = "Hospital- 4";

// For third part
srand(time(NULL));
int num = 0;
int random_numbers[52];
int random;
int found;
char face[4][10]  = {{"Hearths"},{"Diamonds"},{"Clubs"},{"Spades"}};
char suit[13][10] = {{"Ace"}, {"Deuce"}, {"Three"}, {"Four"}, {"Five"}, {"Six"}, {"Seven"}, {"Eight"}, {"Nine"}, {"Ten"}, {"Jack"}, {"Queen"}, {"King"}};


printf("-----------------------------------\n");
printf("0)First Part\n1)Second Part\n2)Third Part\n");
printf("Please Select an Operation :\n");
scanf("%d",&choice);

	switch (choice)
	{
		case 0:
		printf("Enter the street number :\n");
		scanf("%d",&street);
		printf("Enter the avenue number :\n");
		scanf("%d",&avenue);
		result = numPathsHome(street - 1, avenue - 1);
		printf("Numbers of optimal paths to take back home : %d\n",result);
		break;

			case 1:
			printf("Enter the maximum numbers of hospitals that can be constructed:\n");
			scanf("%d",&max_hospital);
				
				if (canOfferCovidCoverage(cities, locations, max_hospital, results))
				{
					for (i = 0; i < max_hospital; i++)
					{
					printf("%s\n",results[i].name );
					printf("Hospital Locations :%s\n",results[i].citiesServed);
					}
				}
					else	printf("No, some cities are not covered.\n");	

			break;

				case 2:
				random_number_generator (random_numbers,52);

					for (i = 0; i < 4; i++)
					{
						for ( k = 0; k < 13; k++)
						{
						picked[num].face = face[i];
						picked[num].suit = suit[k];
						num++;
						}
					}
						for (i = 0; i < 52; i++)
						{							
						random = random_numbers[i];
						printf("%s of %s\n",picked[random].face, picked[random].suit );
						}

				break;
					default:
					printf("Inavalid Operation.\n");
					break;
	}

return 0;
}

// Functions :

int numPathsHome(int street, int avenue)	
{

	if (street == DESTINATION_X || avenue == DESTINATION_Y)		return 1;		// Stops when reaches any of the destination cordinates.		

return numPathsHome(street + 1, avenue) + numPathsHome(street, avenue - 1);
}

int canOfferCovidCoverage(char _cities[6], char _locations[4][10], int _numHospitals, struct Hospital results[4])
{

int array1[10] = {0};
int array2[10][10] = {0};

int result = 0;
int size = _numHospitals;
int i,k,m,x;
int num = NUM;

int limit = factorial (num) / (factorial (num - size) * factorial(size));		// number of combinations

combination (num, size, array1, _numHospitals, array2);


	for (i = 0; i < limit; i++)
	{ 	
		for (m = 0; m < 4; m++)		strcpy (results[m].citiesServed , "0000");
			for (k = 0; k < size; k++)
			{
			x = array2[i][k];
			strcpy(results[k].citiesServed, _locations[x - 1]);
			}
				if (if_found(_cities, results) == 1)
				{
				result = 1;	
				break;
				}
	}

		if (result == 1)		return 1;				// If elements on structure match with cities string returns 1.
			else if (result == 0)		return 0;		// If not returns 0.


}
void combination(int n, int r, int arr1[], int size, int arr2[10][10])			// Lists every combination from 1 to input number. (Recursively)
{
int count;

    for (int i = n; i >= r; i--) 
    {
    arr1[r - 1] = i;
        if (r > 1) 
        { 
        combination(i - 1, r - 1, arr1, size,arr2);
        } 
       		else
       		{
        		for (int i = 0; i < size; i++)
        		{
        		arr2[count][i] = arr1[i];
        		}
        	count++;	
       		}
    
    }   
}



int if_found (char string1[],struct Hospital results[4])				// If the structure contains every element on the first string.Returns 1.Else returns 0.
{
int found;
int i, k;	
char string2[15];

	for (i = 0; i < 4; i++)		strcat(string2, results[i].citiesServed); 


	for (i = 0; i < strlen(string1) - 1; i++)
	{
		for (k = 0; k < strlen(string2); k++)
		{
		found = 0;
			if (string1[i] == string2[k])
			{	
			found = 1;
			break;
			}
		}
			if (found == 0)		break;
	}

				if (found == 1)		return 1;
					else return 0;
}

int factorial (int n)
{
	if ( n == 0)	return 1;

return n * factorial (n - 1); 
}

void random_number_generator(int random_numbers[], int num)					// Fills an array with random numbers.Numbers are different from each other.
{
int found;
int random;
int k;
int i = 0;
					while (i < num)
					{
					random = rand() % num;
					found = 0;

						for (k = 0; k < i; k++)
						{
							if (random_numbers[k] == random)
							{
							found = 1;
							break;
							}
						}

					
							if (!found)
							{
							random_numbers[i] = random;			
							i++;
							}
					}

}