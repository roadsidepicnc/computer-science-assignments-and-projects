#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void menu();
void decrypt_and_print (char*);
void deep_decrypt_and_print (char*);
char decrypt_numbers (int);
void track_machine ();
void refresh_position(int*x,int*y,double* D1,double* R1);
int encrypt_chars(char);
void encrypt_messages (char* file_path);

int main ()
{

menu();

return 0;
}

// Functions :

void menu ()
{

char* loc;

int choice = 0;						// Integer for choice for menu.

	while (1)
	{
	printf("1-) Decrypt and print encrypted_p1.img\n");
	printf("2-) Decrypt and print encrypted_p2.img\n");
	printf("3-) Switch on the tracking machine\n");
	printf("4-) Encrypt the message\n");
	printf("5-) Switch off\n\n");
	printf("Please make your choice :\n");
	scanf("%d",&choice);

		switch (choice)
		{
			case 1:										// First part
			loc = "encrypted_p1.img";			
			decrypt_and_print(loc);
			break;
				case 2:									// Second part
				loc = "encrypted_p2.img";
				deep_decrypt_and_print (loc);
				break;
					case 3:								// Third part
					track_machine();
					break;		
						case 4:							// Forth part
						loc = "decrypted_p4.img";
						encrypt_messages (loc);
						break;
							case 5:
							exit(0);
								default:
								printf("Invalid Operation! Please try again :\n");
								break;
		}
	}	
}


void decrypt_and_print(char* file_path)
{
	int y; 								
	char x,z;							 
	FILE* ptr;
	
	ptr = fopen(("%s",file_path), "r");				
	
	
	while (!feof(ptr))					// Continues until it reached to end of the file.	
	{

		
		fscanf(ptr,"%c",&x);			// It gets a char at every step.
		y  = x - 48;					// ASCII value of '0' is 48.So, i substract 48 from x.I get a value from 0 to 6.
		z = decrypt_numbers(y);			// It prints the char that wanted to print.
		printf("%c",z);
	}
	
	fclose(ptr);
}	

void deep_decrypt_and_print ( char* file_path)
{

	int i,mod;
	int y1=0,y2=0,y3=0;
	int total;
	char x1,x2,x3;
	char result;
	FILE* ptr;
	
	
	ptr = fopen(("%s",file_path),"r");

	fscanf(ptr,"%c",&x1);					// It gets 3 chars.
	fscanf(ptr,"%c",&x2);
	fscanf(ptr,"%c",&x3);
	
	y1  = x1 - 48;							// ASCII value of '0' is 48.So, i substract 48 from x.I get a value from 0 to 6.
	y2  = x2 - 48;
	y3  = x3 - 48;
	total = y1 + y2 + y3;					// It adds those 3 values each other.
	mod = total % 7;						// It takes mod of the result of the summation.
	result = decrypt_numbers(mod);			// It prints the char that wanted to print.
	printf("%c",result);

	while (!feof(ptr))						// Continues until it reached to end of the file.	
	{
		
		fscanf(ptr,"%c",&x1);
		if (x1 == '\n')						// Checks if current character is next line.
		{									
											// If it is true sets beginning of the line.
			printf("\n");
			fscanf(ptr,"%c",&x1);
			fscanf(ptr,"%c",&x2);
			fscanf(ptr,"%c",&x3);
			y1  = x1 - 48;
			y2  = x2 - 48;
			y3  = x3 - 48;
			total = y1 + y2 + y3;
			mod = total % 7;
			result = decrypt_numbers(mod);
			printf("%c",result);
			
		}
		else {							
		y1  = x1 - 48;
		total = y2 + y3 + y1;
		mod = total  % 7;	
		result = decrypt_numbers(mod);
		printf("%c",result);
			

		fscanf(ptr,"%c",&x2);
		y2  = x2 - 48;
		total = y3 + y2 + y1;
		mod = total  % 7;
		result = decrypt_numbers(mod);
		printf("%c",result);
			

		fscanf(ptr,"%c",&x3);
		y3  = x3 - 48;
		total = y1 + y2 + y3;
		mod = total  % 7;
		result = decrypt_numbers(mod);
		printf("%c",result);
		
		}
	}

fclose(ptr);
}


char decrypt_numbers(int number)					// It takes an integer and returns a char.
{

switch (number)
{
	case 0:
	return ' ';
	break; 		
		case 1:
		return '-';
		break; 
			case 2:
			return '_';
			break; 
				case 3:
				return '|';
				break; 
					case 4:
					return '/';
					break; 
						case 5:
						return '\\';
						break; 
							case 6:
							return 'O';
							break; 
									default:
									return '\n';
									break;

}

}

void track_machine ()
{

int X1 = 0;						// x cordinate of enemy base's location.
int Y1 = 0;						// y cordinate of enemy base's location.
double R1;						// Distance between previous enemy base and current enemy base.
double D1; 						// Distance between enemy base and our base.
int i,k;
char c = '.';
char choice = 'R';				// For checking if user want to continue or exit.
int check2 = 1;

	while (choice != 'E')		// 'R' for continue and 'E' for exit. 
	{
	
	
									if (choice == 'R')
									{
									check2 = 1;
									}
										else if (choice == 'E')
										{
											check2 = 0;
										}
											else
											{
											check2 = 2;
											}	

					scanf("%c", &choice);	
											
switch (check2)
{
	case 1:

			refresh_position(&X1,&Y1,&D1,&R1);

				for (i = 1;i <= 11;i++)
				{
		
					for (k = 1;k <= 11;k++)
					{
						if (i == 6 && k == 6)
						{
						c = 'O';					// For printing our base's location (6 ,6)
						}
							else if (i == X1 && k == Y1)
							{	
							c = 'E';				// For printing enemy base's location
							}
								else if (c != '.')
								{	
								c = '.';			// For printing every other location except enemy and our base.
								}

					printf("%c",c);
					printf("        ");

					}

				printf("\n");
				}

			printf("\nEnemies X Position : %d, Y Position : %d, Displacement : %lf, Distance to our camp : %lf\n",X1,Y1,D1,R1);		
			printf("Command waiting...:\n");
			break;

					case 2:
					printf("You entered an invalid operation!\n");			// Error message
					printf("Command waiting...:\n");
					break;
	}	


		}
	}




void refresh_position(int* X,int* Y,double* D,double* R)
{

int X_prev;				// x cordinate of previous enemy base's location.
int Y_prev;				// y cordinate of previous enemy base's location.

X_prev = *X;
Y_prev = *Y;

*X = rand() % 12;		// x cordinate of current enemy base's location.
*Y = rand() % 12;		// y cordinate of current enemy base's location.
	
	while (*X == *Y || *X == 0 || *Y == 0)		// Avoids enemy and our base's locations are same point.
	{
	*X = rand() % 12;
	*Y = rand() % 12;
	}



*D = sqrt (pow(*Y-6,2) + pow(*X-6,2));						// Calculates distance between enemy base and our base.
*R = sqrt (pow(*Y - Y_prev,2) + pow(*X-X_prev,2));          // Calculates distance between previous enemy base and current enemy base.

} 

int encrypt_chars (char c)									// Function for part4 encrypting 
{

	switch (c)
	{
		case ' ':
		return 0;
		break; 		
			case '-':
			return 1;
			break; 
				case '_':
				return 2;
				break; 
					case '|':
					return 3;
					break; 
						case '/':
						return 4;
						break; 
							case '\\':
							return 5;
							break; 
								case '0':
								return 6;
								break; 
									

	}

}

void encrypt_messages (char* file_path)
{

	int i,mod;
	int y1=0,y2=0,y3=0;
	int total;
	char x1,x2,x3;
	int result;
	FILE* ptr1;			// Decrypted file
	FILE* ptr2;			// File to be encrypted.
	
	
	ptr1 = fopen(("%s",file_path),"r");
	ptr2 = fopen("encrypted_p4.img","w");

	fscanf(ptr1,"%c",&x3);				// It gets 3 chars.
	y3 = encrypt_chars (x3);			// It takes a char and return an integer.
	total = y1 + y2 + y3;				// It adds those 3 values each other.
	mod = total % 7;					// It takes mod of the result of the summation.
	fprintf(ptr2,"%d",mod);				// It writes the integer that wanted to the new file.

	while(!feof(ptr1))					// Continues until it reached to end of the file.	
	{

	fscanf(ptr1,"%c",&x1);

		if (x1 == '\n')			// If current character is next line it sets the beginning of the line.
		{
			
		y1 = 0;
		y2 = 0;
		fscanf(ptr1,"%c",&x3);
		fprintf(ptr2,"\n");
			
			if (x3 != '\n')		
			{
			y3 = encrypt_chars(x3);
			total = y1 + y2 + y3;
			mod = total % 7;
			fprintf(ptr2,"%d",mod);
			}
			else fprintf(ptr2,"\n");

		}
				else 
				{
				y1 = encrypt_chars(x1);
				total = y1 + y2 + y3;
				mod = total % 7;
				fprintf(ptr2,"%d",mod);

				}

	fscanf(ptr1,"%c",&x2);

					if (x2 == '\n')			// If current character is next line it sets the beginning of the line.
					{
					y1 = 0;
					y2 = 0;
					fscanf(ptr1,"%c",&x3);
					fprintf(ptr2,"\n");

						if (x3 != '\n')
						{

						y3 = encrypt_chars(x3);
						total = y1 + y2 + y3;
						mod = total % 7;
						fprintf(ptr2,"%d",mod);
						}
							else fprintf(ptr2,"\n");
							
					}
							else 
							{
							y2 = encrypt_chars(x2);
							total = y1 + y2 + y3;
							mod = total % 7;
							fprintf(ptr2,"%d",mod);
							}

	fscanf(ptr1,"%c",&x3);

								if (x3 == '\n')				// If current character is next line it sets the beginning of the line.
								{
								y1 = 0;
								y2 = 0;
								fscanf(ptr1,"%c",&x3);
								fprintf(ptr2,"\n");

									if (x3 != '\n')
									{
									y3 = encrypt_chars(x3);
									total = y1 + y2 + y3;
									mod = total % 7;
									fprintf(ptr2,"%d",mod);
									}
										else fprintf(ptr2,"\n");

								}
											else 
											{
											y3 = encrypt_chars(x3);
											total = y1 + y2 + y3;
											mod = total % 7;
											fprintf(ptr2,"%d",mod);
											}
		
		}


fclose(ptr1);
fclose(ptr2);
}






