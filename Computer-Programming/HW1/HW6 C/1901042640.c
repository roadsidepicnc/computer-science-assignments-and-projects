#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void game_field();
void pick_word (char buffer[15]);
char random_word_generator();
int converter (char x);
void play(char string2[20][20] ,char string3[10][15], int* num);

int main()
{
srand(time(NULL));
game_field();

return 0;
}

// Functions:

void game_field()
{

int raw;
int column;
int i,k,m,n;
int counter = 0;
int random;


int* score;						// For tracking score
long int length;				// length of the string.
char c;							
int  is_available;				// it is 1 if picked position is available for word ,0 if picked position is unavailable. 
int word_check;

char field[20][20];			// Game field
char word[15];				// For keeping word
int check_array[20][20];	// For checking if picked position for word is already allocated.
char word_list[10][15];		//  For storing picked words
word_list[0][0] = '\0';


	for (i = 0; i < 20; i++)
	{
		for (k = 0; k < 20; k++)
		{
		check_array[i][k] = 0;
		}
	}

		for (i = 0; i < 20; i++)
		{
			for (k = 0; k < 20; k++)
			{
			c = ' ';
			field[i][k] = c;
			}
		}

while (counter < 10)
{
				
random = rand () % 80;			// For picking a random position for the word.
			
	if (random <= 10)							
	{
			pick_word (word);								// Left to Right
			word_check = 0;
			is_available = 1;
			length = strlen(word) - 2;
			column = rand() % (20 - length);	
			raw = rand() % 20;
			
			

		for (m = column; m < column + length; m++)
		{
			if (check_array[raw][m] == 1 && field[raw][m] != word[word_check] )		// Checks if that position is available to put the word there.		
			{
			is_available = 0;
			}
		word_check += 1; 
		}
			if (is_available)														// If it is available puts the word there.
			{
			strcpy(word_list[counter],word);		// Puts word to the list
			word_check = 0;
					for (k = length + column - 1; k >= column; k--)			
					{	
							check_array[raw][k] = 1;						
							field[raw][k] = word[length - word_check -1];
							word_check += 1;		
					}
				
				counter += 1;	
			
			}					
	
	}
		
		else if (random > 10 && random <= 20)
		{
			pick_word (word);
			word_check = 0;								// Right to Left
			is_available = 1;
			length = strlen(word) - 2;
			raw = rand() % 20;
			column = rand() % 20;	
				while (column <= length -1)
				{
				column = rand() % 20;	
				}

									for (m = column; m > column - length; m--)
									{
										if (check_array[raw][m] == 1 && field[raw][m] != word[word_check])			// Checks if that position is available to put the word there.	
										{
										is_available = 0;
										}
									word_check += 1;
									}

											if (is_available)								// If it is available puts the word there.
											{
											word_check = 0;
											strcpy(word_list[counter],word);				// Puts word to the list
												for (k = column ; k > column - length; k--)
												{
												
												check_array[raw][k] = 1;
												field[raw][k] = word[word_check];
												word_check += 1;
												}
											counter += 1;
											}				
		
		}	

				
		 	else if (random > 20 && random <= 30)
		 	{
		 
		 	word_check = 0;											// Top-Down
			is_available = 1;
		 	pick_word (word);
			length = strlen(word) - 2;
			column = rand() % 20;
			raw = rand() % (20 - length);
	
				for (m = raw; m < raw + length; m++)
				{
					if (check_array[m][column] == 1 && field[m][column] != word[word_check])		// Checks if that position is available to put the word there.	
					{
					is_available = 0;
					}
				word_check += 1; 
				}

					if (is_available)								// If it is available puts the word there.
					{
					word_check  = 0;
					strcpy(word_list[counter],word);				// Puts word to the list
						for (k = length + raw - 1; k >= raw; k--)
						{	
							
							check_array[k][column] = 1;
							field[k][column] = word[length - word_check - 1];
							word_check += 1;
						}
						counter += 1;
					}
			
			}		

					else if (random > 30 && random <= 40)
		 			{
		 
		 			word_check = 0;
					is_available = 1;									// Down-Top
		 			pick_word (word);
					length = strlen(word) - 2;
					column = rand() % 20;
					raw = rand() % 20;
						while (raw <= length - 1)
						{
						raw = rand() % 20;	
						}
			
				

						for (m = raw; m > raw - length; m--)
									{
										if (check_array[m][column] == 1 && field[m][column] != word[word_check])		// Checks if that position is available to put the word there.	
										{
										is_available = 0;
										}
									word_check += 1;
									}

											if (is_available)								// If it is available puts the word there.
											{		
											word_check = 0;
											strcpy(word_list[counter],word);				// Puts word to the list
												for (k = raw ; k > raw - length; k--)
												{
												
												check_array[k][column] = 1;
												field[k][column] = word[word_check];
												word_check += 1;
												}
											counter += 1;
											}
					
					}			
						
						else if (random > 40 && random <= 50) 
						{
				
		 				word_check = 0;
						is_available = 1;
		 				pick_word (word);								// Diagonally (Down Left to Right Top)

						length = strlen(word) - 2;
						column = rand() % (20 - length);
						raw = rand() % 20;
							while (raw <= length - 1)
							{
							raw = rand() % 20;
							}
			
								for (m = column; m  < column + length; m++)
								{
									if (check_array[raw - word_check][m] == 1 &&  field[raw - word_check][m] != word[word_check])	// Checks if that position is available to put the word there.	
									{
									is_available = 0;
									}
								word_check += 1;
								}

						if (is_available)							// If it is available puts the word there.
						{
						word_check = 0;				
						strcpy(word_list[counter],word);			// Puts word to the list
							for(k = column; k < column + length; k++)
							{
							
							check_array[raw - word_check][k] = 1;
							field[raw - word_check][k] = word[word_check];
							word_check += 1;
							}
							counter += 1;
						}
							
				
						}		

							else if (random > 50 && random <= 60)
							{
				
		 					word_check = 0;
							is_available = 1;
		 					pick_word (word);							// Diagonally (Right Top to Left Down)

							length = strlen(word) - 2;
							raw = rand() % (20 - length);
							column = rand() % 20;
								while (column <= length - 1)
								{
								column = rand() % 20;
								}

									for (m = raw; m  < raw + length; m++)
									{
										if (check_array[m][column - word_check] == 1 && field[m][column - word_check] != word[word_check])	// Checks if that position is available to put the word there.	
										{
										is_available = 0;
										}
									word_check += 1;
										}

											if (is_available)						// If it is available puts the word there.
											{
											word_check = 0;
											strcpy(word_list[counter],word);		// Puts word to the list
												for(k = raw; k < raw + length; k++)
												{
												
												check_array[k][column - word_check] = 1;
												field[k][column - word_check] = word[word_check];
												word_check += 1;
												}
											counter += 1;
											}
							
				
							}		

							
							else if (random > 60 && random <= 70) 
							{
				
		 					word_check = 0;
							is_available = 1;
		 					pick_word (word);							// Right Down to Left Top

							length = strlen(word) - 2;					
							column = rand() % 20;
							raw = rand() % 20;
								while (raw <= length - 1)
								{
								raw = rand() % 20;
								}
									while (column <= length -1)
									{
									column = rand() % 20;	
									}

									for (m = column; m > column - length; m--)
									{
										if (check_array[raw - word_check][m] == 1 && field[raw - word_check][m] != word[word_check])	// Checks if that position is available to put the word there.	
										{
										is_available = 0;
										}
									word_check += 1;
									}

											if (is_available)							// If it is available puts the word there.
											{
											word_check = 0;
											strcpy(word_list[counter],word);			// Puts word to the list
												for (k = column ; k > column - length; k--)
												{
												
												check_array[raw - word_check][k] = 1;
												field[raw - word_check][k] = word[word_check];
												word_check += 1;
												}
											counter += 1;
											}
									}					

								else if (random > 70 && random <= 80)
								{
				
		 						word_check = 0;
								is_available = 1;
		 						pick_word (word);						// Left Top to Right Down

								length = strlen(word) - 2;
								raw = rand() % (20 - length);
								column = rand() % (20 - length);				

										for (m = raw; m  < raw + length; m++)
										{
											if (check_array[m][column + word_check] == 1 && field[m][column + word_check] != word[word_check])	// Checks if that position is available to put the word there.	
											{
											is_available = 0;
											}
										word_check += 1;
										}

												if (is_available)							// If it is available puts the word there.
												{
												word_check = 0;
												strcpy(word_list[counter],word);			// Puts word to the list
													for(k = raw; k < raw + length; k++)
													{
													
													check_array[k][column + word_check] = 1;
													field[k][column + word_check] = word[word_check];
													word_check += 1;
													}
												counter += 1;
												}
							
				
								}		

		
	


}

printf("\n");

char t[20] = "A31";
int u = atoi(t);
printf("%s-%d\n",t,u );
int score_check;
int x,y;

score_check = 0;
score = &score_check;

			while (score_check < 10)
			{
			printf("0 1 2 3 4 5 6 7 8 9 10111213141516171819\n");		// Column Numbers
				for (x = 0; x < 20; x++)
				{
					for (y = 0; y < 20; y++)
					{
					printf("%c ",field[x][y]);				// Prints field
					}
					printf("%c",'a' + x);					// Row characters
						if (x < 10)
						{	
						length = strlen	(word_list[x]);
						printf("     ");
							for (k = 0; k < length; k++)
							{
							printf("%c",word_list[x][k]);		// Prints word list
							}
						
						}
							else printf("\n");
							

						}
							
				play(field,word_list,score);
				}
			printf("0 1 2 3 4 5 6 7 8 9 10111213141516171819\n");
				for (x = 0; x < 20; x++)
				{
					for (y = 0; y < 20; y++)
					{
					printf("%c ",field[x][y]);
					}
					printf("%c",'a' + x);
						if (x < 10)
						{	
						length = strlen	(word_list[x]);
						printf("     ");
							for (k = 0; k < length; k++)
							{
							printf("%c",word_list[x][k]);
							}	
						}
							else printf("\n");				
				}
	
				if (score_check == 10)	printf("Conngratulations !!\n");
}			

void pick_word (char buffer[15])			// Picks a random word from text file.
{

FILE* ptr;
ptr = fopen("words.txt","r");

int i;
int mod;
int x;

mod = rand() % 101;
	
		for (i = 1; i <= mod; i++)
		{
		fgets(buffer,15,ptr);
		}

x = strlen(buffer);		

fclose(ptr);
}

char random_word_generator()
{
return (rand() % 26) + 'a'; 	
}

int converter (char x)		// Convert char to integer	
{
int y;
y = x - 'A';			
return y;
}

void play(char string2[20][20] ,char string3[10][15], int* num)
{
int k,i,y;
char x_char;
int x;
int win; 				// for checking if input matches to field.
char string1[15];		// input word
long int length;		// length of input
int check;				// For checking if score increased.
int is_same;			

printf("Enter your answer :");
scanf(" %c%d",&x_char,&y);
scanf(" %s",string1);						// input word
if (strcmp(string1,"Exit") == 0) exit(0);	// Type 'Exit' to close the program.

x = converter(x_char);


length = strlen(string1);
win = 1;
check = *num;

			for (k = 0; k < length; k++)
			{
				if (string2[x][y + k] != string1[k] && string2[x][y + k] != string1[k] - 32)		// Left to Right
				{																					// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
				win = 0;
				break;
				}					
			}
					if (win) 
					{ 
					(*num)++;
					
						for (k = 0; k < length; k++)
						{
							if (string2[x][y + k]  != string2[x][y + k] - 32)		// If every character from starting point to destination is same with input.Increases score by 1 and converts every to upper cases from lower cases.
							{														// Destination is from startimng point to right
							string2[x][y + k] = string1[k] - 32;
							}
						}
					}
		
win = 1;
				
					for (k = 0; k < length; k++)
					{

						if (string2[x][y - k] != string1[k] && string2[x][y - k] != string1[k] - 32)		// Right to Left                                           
						{																					// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
						win = 0;
						break;
						}								
					}
							if (win) 											// If input matches with the word, proceeds.
							{ 
							(*num)++;
						
								for (k = 0; k < length; k++)
								{
									if (string2[x][y - k] != string1[k] - 32)	// If every character from starting point to destination is same with input.Increases score by 1 and converts every to upper cases from lower cases.
									{
									string2[x][y - k] = string2[x][y - k] - 32;		// Destination is from starting point to left.
									}
								}
							}

	
						
win = 1;
							for (k = 0; k < length; k++)
							{

								if (string2[x + k][y] != string1[k] && string2[x + k][y] != string1[k] - 32)		// Top-Down
								{
								win = 0;																			// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
								break;
								}
							}
									if (win) 									// If input matches with the word, proceeds.
									{ 
									(*num)++;
									
										for (k = 0; k < length; k++)
										{
											if (string2[x + k][y] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
											{
											string2[x + k][y] = string2[x + k][y] - 32;		// Destination is from starting point to down.
											}
										}
									}

	
								
win = 1;								
										for (k = 0; k < length; k++)
										{
											if (string2[x - k][y] != string1[k] && string2[x - k][y] != string1[k] - 32)		// Down-Top
											{																					// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
											win = 0;
											break;
											}				
										
										}	
												if (win) 											// If input matches with the word, proceeds.
												{ 
												(*num)++;
												
													for (k = 0; k < length; k++)
													{
														if (string2[x - k][y] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
														{
														string2[x - k][y] = string2[x - k][y] - 32;			// Destination is from starting point to top.
														}
													}
												}	
								
win = 1;										
												for (k = 0; k < length; k++)
												{
													if (string2[x - k][y + k] != string1[k] && string2[x - k][y + k] != string1[k] - 32)		// Diagonally (Down Left to Right Top)														
													{																							// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
													win = 0;
													break;
													}										
												}
														if (win) 												// If input matches with the word, proceeds.
														{ 
														(*num)++;
												
															for (k = 0; k < length; k++)
															{
																if (string2[x - k][y + k] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
																{
																string2[x - k][y + k] = string2[x - k][y + k] - 32;		// Destination is from starting point to right and top.
																}
															} 
														}

win = 1;												

													for (k = 0; k < length; k++)
													{

														if (string2[x + k][y - k] != string1[k] && string2[x + k][y - k] != string1[k] - 32)		// Diagonally (Right Top to Left Down)
														{																							// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
														win = 0;
														break;
														}
													}
															if (win) 												// If input matches with the word, proceeds.
															{
															(*num)++;
														
																for (k = 0; k < length; k++)
																{
																	if (string2[x + k][y - k] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
																	{
																	string2[x + k][y - k] = string2[x + k][y - k] - 32;		// Destination is from starting point to left and down. 
																	}
																} 
															}


win = 1;		
															for (k = 0; k < length; k++)
															{

																if (string2[x - k][y - k] != string1[k] && string2[x - k][y - k] != string1[k] - 32)		// Right Down to Left Top
																{																							// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .
																win = 0;	
																break;
																}													
															}		
																	if (win) 												// If input matches with the word, proceeds.
																	{ 
																	(*num)++;
																	
																		for (k = 0; k < length; k++)
																		{
																			if (string2[x - k][y - k] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
																			{	
																			string2[x - k][y - k] = string2[x - k][y - k] - 32;		// Destination is from starting point to left and top.
																			}
																		}
																	}															
		


win = 1;																
																	for (k = 0; k < length; k++)					
																	{

																		if (string2[x + k][y + k] != string1[k] && string2[x + k][y + k] != string1[k] - 32)		// Left Top to Right Down
																		{																							// Checks if input is same as the word.Also ,checks for upper case character since words can cross and user would have found the word .										
																		win = 0;
																		break;
																		}																	
						
																	}
																			if (win) 												// If input matches with the word, proceeds.
																			{ 
																			(*num)++;

																				for (k = 0; k < length; k++)
																				{
																					if (string2[x + k][y + k] != string1[k] - 32)	// If every character from starting point to destination is same as input.Increases score by 1 and converts every to upper cases from lower cases.
																					{
																					string2[x + k][y + k] = string2[x + k][y + k] - 32;		// Destination is from starting point to right and dwon.
																					}
																				}
																			}


			if (check < *num)
			{

				for (k = 0; k < 10; k++)
				{
				is_same = 1;
					for (i = 0; i < length; i++)
					{
						if (string3[k][i] != string1[i])				// Checks if input is same with any word on the list.
						{
						is_same = 0;
						break;
						}

					}	
							if (is_same == 1)
							{
								for (i = 0; i < length; i++)			// If it is ,converts lower case characters on the list to upper cases.
								{
								string3[k][i] = string3[k][i] - 32;
								}
							break;
							}

				}

				printf("%d\n",*num );


			}

	}