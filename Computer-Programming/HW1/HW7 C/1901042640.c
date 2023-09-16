#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void menu ();
void divide_into_parts_string(char string[],char string2[],char string3[]);
void divide_into_parts_float (float array[2000][7],char string1[], char string2[]);
void genres_platforms (char string1[],char string2[]);
float word_number (char* string1, char string2[]);
void list_due_to_date (float array1[2000][7], char string1[] ,int choice, int date);
void game_finder (char string1[],char string2[],char string3[],char string4[],float array1[2000][7]);
void georogphical_popularity_checker (char string1[],char string2[],float array1[2000][7]);
void frequency_calculator (char string1[], float array1[2000][7], int choice);

int main()
{
	menu();
	return 0;
}

// Functions :

void menu()
{

int i;
float total = 0;
char x1[1000];		// includes genres without overlap
char x2[1000];		// includes platorms without overlap
x1[0] = '\0';
x2[0] = '\0';
char backup[100000];
char name[100000];			// includes all names
char genre[100000];			// includes all genres
char platform[100000];		// includes all platforms
float all_info[2000][7];	

float earliest;
float latest;
int choice;					// menu choice
int year;					// year selection for part 2nd operation
int year_choice;			// until or since selection for 2nd operation
char name_of_the_game[100];		// name of the game for 3rd operation

int p = 0;

divide_into_parts_string(name,genre,platform);
genres_platforms(platform,x2);
genres_platforms(genre,x1);
divide_into_parts_float(all_info,x1,x2);
int k;


while(1)
{
printf("0: List of the Genres:\n");
printf("1: List of Platforms:\n");
printf("2: List of the Games Through the Years:\n");
printf("3: All Informations of a Single Game:\n");
printf("4: Average of the User Scores:\n");
printf("5: Georogphical Information of a Single Game:\n");
printf("6: Frequence of the Genres:\n");
printf("7: Frequence of the Platforms:\n");
printf("8: Exit:\n");
printf("Please Select an operation :\n");
scanf("%d",&choice);

	switch (choice)
	{
		case 0:	
		printf("%s\n",x1);			// prints all ganres
		break;
			case 1:
			printf("%s\n",x2 );		// prints all platforms
			break;
				case 2:
				printf("Enter a year:\n");
				scanf("%d",&year);
				printf("Until (0) or Since (1) %d\n",year);
				scanf("%d",&year_choice);
				if (year < 1985 && year_choice == 0)	printf("Invalid date\n");
					else if (year > 2005 && year_choice == 1)	printf("Invalid date\n");
						else
						{
						strcpy(backup,name);
						list_due_to_date(all_info,backup,year_choice,year);			// Lists game names
						}
				break;
					case 3:
					strcpy(backup,name);
					printf("Please enter the name of the game: \n");
					scanf("%s",name_of_the_game);
					game_finder(backup,x1,x2,name_of_the_game,all_info);	// Prints all of the information about the selected game
					break;
						case 4:
							for (i = 0; i < 2000; i++)
							{
							total += all_info[i][6];			// Takes average of the user scores.
							}
							printf("%f\n",total / 2000 );
							break;
							case 5:
							printf("Please enter the name of the game: \n");
							scanf("%s",name_of_the_game);
							strcpy(backup,name);
							georogphical_popularity_checker(backup,name_of_the_game,all_info);		// Compares NA Sales and EU Sales and decides which region game was more popular at
							break;
								case 6:
								strcpy(backup,x1);
								frequency_calculator(x1,all_info,0);				// Prints how many games were at each genre
								break;
									case 7: 
									frequency_calculator(x2,all_info,1);			// Prints how many games were on each platform
									break;
										case 8:										// Exit
										exit(0);
											default: 
											printf("TRY AGAIN !\n");			// Prints errror message
											break;
	}	

}

}

void divide_into_parts_string(char string1[],char string2[],char string3[])
{

FILE* ptr;
ptr = fopen ("Video_Games.txt","r");
char* token; 
char* rest;
char first_step[200];		
int counter1;
int counter2;
int x = 0;
char idk[20];

fgets(first_step,200,ptr);
	while (!feof(ptr))
	{
	fgets(first_step,200,ptr);			// Reads the file string by string
	rest = first_step;	
	counter1 = 0;
		while ((token = strtok_r(rest, ",", &rest))) 
		{
			
				if (counter1 == 0)
				{		
				strcat(string1,token);					// Stores names of the games
				sprintf(string1,"%s,",string1);
				break;
				}														
		}
	
	}

rewind(ptr);
fgets(first_step,200,ptr);

				while (!feof(ptr))
				{
				fgets(first_step,200,ptr);
				rest = first_step;	
				counter1 = 0;
					while ((token = strtok_r(rest, ",", &rest))) 
					{
						if (counter1 == 1)
						{		
						strcat(string2,token);						// Stores genres of the games
						sprintf(string2,"%s,",string2);
						break;
						}
							
					counter1++;	
					}							
				}


rewind(ptr);
fgets(first_step,200,ptr);

					while (!feof(ptr))
					{
					fgets(first_step,200,ptr);
					rest = first_step;	
					counter1 = 0;
						while ((token = strtok_r(rest, ",", &rest))) 
						{
							if (counter1 == 2)
							{		
							strcat(string3,token);					// Stores platforms of the games
							sprintf(string3,"%s,",string3);
							break;
							}
							
						counter1++;	
						}							
					}


										

										
fclose(ptr);
}

void divide_into_parts_float (float array[2000][7],char string1[], char string2[])
{
FILE* ptr;
ptr = fopen ("Video_Games.txt","r");
char* token; 
char* rest;
char first_step[200];
int counter1;
int counter2;
int x = 0;
char idk[20];


counter1 = 0;
fgets(first_step,200,ptr);

										while (!feof(ptr))
										{
										fgets(first_step,200,ptr);			// Reads the file string by string
										rest = first_step;	
										counter2 = 0;
											
												while ((token = strtok_r(rest, ",", &rest))) 
												{
													
														if (counter2 == 1)
														{
														array[counter1][counter2 -1] = word_number(token,string1);		// Puts a number for each genre type.Stores genre number for each game.
														}
															else if (counter2 == 2)
															{
															array[counter1][counter2 - 1] = word_number(token,string2);		// Puts a number for each platform type.Stores platform number for each game.
															}
																else if (counter2 != 0)
																{
																	if (strcmp(token,"not_available") == 0)				// Checks if  there are any "not available" data
																	{
																	array[counter1][counter2 -1] = -31;					// If there are puts a negative number to that cell.Since,there can't be a negative number at the list. 		
																	}													// It can pick the negative number and print "not available".
																	else
																	array[counter1][counter2 - 1] = atof(token);		// Converts number at string to a float.Stores data at the array.
																}

													

												counter2++;
												}
											
										counter1++;
										}



fclose(ptr);

}

void genres_platforms (char string1[],char string2[])
{
char check[20];
char* token;
char* rest;
rest = string1;

		while ((token = strtok_r(rest, ",", &rest))) 
		{
		
			strcpy(check,token);
			sprintf(check,"%s,",check);	
				if (strstr(rest,check) == NULL)
				{
				strcat(string2,check);						// Stores genres of the games without any overlap.
				}
	}
}

float word_number (char* string1, char string2[])
{
float num;
char* token;
char* rest;
char string2_copy[1000];
strcpy(string2_copy,string2);
rest = string2_copy;

		while ((token = strtok_r(rest, ",", &rest))) 
		{
			if (strcmp(string1,token) == 0)	break;		// Gives a number to each type . Gives one of them to input string depending it's content.


		num++;
		}

return num;
}


void list_due_to_date (float array1[2000][7], char string1[] ,int choice,int date)
{

int array2[2000];
int i;
int counter = 0;

if (choice == 0)					// Until
{
	for (i = 0; i < 2000; i++)
	{
	
		if (array1[i][2] < date)	// Lists the games published before the input date.	
		{
		array2[counter] = i;
		counter++;
		}
	}
}
			else if (choice == 1)		// Since
			{
				for (i = 0; i < 2000; i++)
				{

					if (array1[i][2] >= date)		// Lists the games published after the input date.
					{
					array2[counter] = i;
					counter++;
					}
				}
			 }
			


char* token;
char* rest;
rest = string1;
int counter2 = 0;
int counter1 = 0;

	while ((token = strtok_r(rest, ",", &rest))) 
	{
		if (array2[counter2] == counter1)				// Prints those games
		{
		printf("%s\n",token);
		counter2++;
		}

	counter1++;
	}

}


void game_finder (char string1[],char string2[],char string3[],char string4[],float array1[2000][7])
{
char string2_copy[1000];
char string3_copy[1000];
strcpy(string2_copy,string2);
strcpy(string3_copy,string3);
int num;							// Row number for the array
char* token;
char* rest;
rest = string1;
int counter1 = 0;
int check = 0;

	while ((token = strtok_r(rest, ",", &rest))) 
	{
		if (strcmp(string4,token) == 0)	
		{
		check = 1;	    
		num = counter1;			// Finds the game's name and it's row number for the array.
		break;
		}

	counter1++;
	}

			if (check)
			{
			printf("Name: %s\n",token);	

counter1 = 0;
rest = string2_copy;
			while ((token = strtok_r(rest, ",", &rest))) 
			{
				if (counter1 == array1[num][0])	printf("Genre :%s\n",token );		// Prints the genre of the game 

			counter1++;	
			}

counter1 = 0;
rest = string3_copy;
			while ((token = strtok_r(rest, ",", &rest))) 
			{
				if (counter1 == array1[num][1])	printf("Platform :%s\n",token );	// Prints the platform of the game


			counter1++;	
			}	

printf("Year :%f\n",array1[num][2]);				
printf("Sales in NA :%f\n",array1[num][3]);
printf("Sales in EU :%f\n",array1[num][4]);
	if (array1[num][5] == -31)	printf("Total Sale :Not Available\n");
		else 	printf("Total Sale :%f\n",array1[num][5]);
printf("User Score :%f\n",array1[num][6]);


		}

else 	printf("There is no game called %s.\n",string4 );

}


void georogphical_popularity_checker (char string1[],char string2[],float array1[2000][7])
{

int num;
char* token;
char* rest;
rest = string1;
int counter1 = 0;
int check = 0;

	while ((token = strtok_r(rest, ",", &rest))) 			// Finds the game's name and it's row number for the array.
	{
		if (strcmp(string2,token) == 0)	
		{	
		check = 1;
		num = counter1;
		break;
		}

	counter1++;
	}

if (check)
{

	if (array1[num][3] < array1[num][4])	printf("This game was more popular in Europe.\n" );
		else if (array1[num][3] > array1[num][4])	printf("This game was more popular in North America.\n" );
			else if (array1[num][3] == array1[num][4])	printf("This game was equally popular in North America and Europe.\n" );
}
	
	 else 	printf("There is no game called %s.\n",string2 );
	
			
}


void frequency_calculator (char string1[], float array1[2000][7],int choice)
{

int i,k;
float array2[15];
int counter1 = 0;
float counter2 = 0;
char string1_copy[1000];
strcpy(string1_copy,string1);
char* token;
char* rest;
rest = string1_copy;

for (i = 0; i < 12; i++)	array2[i] = '\0';
	

	for ( i = 0; i < 12; i++)
	{
		for (k = 0; k < 2000; k++)
		{
			if (array1[k][choice] == counter2)	array2[i] = array2[i] + 1;		// It counts each game by it's genre or platform.
		}
	counter2 = counter2 + 1;
	}

				while ((token = strtok_r(rest, ",", &rest))) 					// It prints the genre or platform type and number of games.
				{
				printf("%s: %f\n",token,array2[counter1] );	
				counter1++;
				}


}