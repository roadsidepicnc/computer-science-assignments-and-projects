#include <stdio.h>
#include <string.h>

void menu();
void read_news(char buffer[500], char file_path[20], int is_Only_Title);
void append_file(char* file_path, char c);
double f_func (int x);
double g_func(double f_func (int x),int a);
void read_magic_numbers(char buffer_magic[10], char buffer_news[500]);

int main()
{

menu();

return 0;
}

// Functions:

void menu()
{
char string[500];
int news_num;
char news_path[20];
char magic[10];
int magic_int;
double magic_num = 0;
double magic_result;


FILE* ptr_news_num;
FILE* ptr_read_news_num;
FILE* ptr_check;
ptr_news_num = fopen("all_news_id.txt","r");		// File that stores all news numbers.
ptr_check = fopen("readed_news_id.txt","r");		// File for storing read news.

char* read_news_path;								
read_news_path = "readed_news_id.txt";				
char x;				// Character for checking if the new is already read for case a
int num = 69;
int y;				// Integer for case b.
int check_if_equal = 0;
int i;

int check1;							// Integer for case a.
char is_continue = 'y';				// char for asking user continue or exit.
int text_choice;					
char menu_choice;
char read_news_char;


while (is_continue == 'y')
	{	

rewind(ptr_news_num);
printf("**********Daily Press**********\n\n");
printf("Today's news are listed for you :\n\n");

	while (!feof(ptr_news_num))						
	{
	fscanf(ptr_news_num,"%d",&news_num);
	sprintf(news_path,"news/%d.txt",news_num);		// File address
	printf("Title of %d. new :",news_num);			
	read_news(string,news_path,1);
	printf("%s",string);							// Prints titles of the news.
	}

printf("What do you want to do ?\n");				// Prints options that user can choose.
printf("a. Read a new\n");
printf("b. List the read ones\n");
printf("c. Get the decrypted information from the news\n");
scanf(" %c",&menu_choice);							// Takes users choice.

	
		switch (menu_choice)
		{
			case 'a':
			ptr_read_news_num = fopen("readed_news_id.txt","r");
			printf("Which new text you want to read ?\n");			
			scanf("%d", &text_choice);
			sprintf(news_path, "news/%d.txt",text_choice);			// Files address
			read_news_char = text_choice + 48;
			check1 = 0;

			while(!feof(ptr_read_news_num))
				{
				x = fgetc(ptr_read_news_num);
					if (read_news_char == x)
					{
					check1 = 1;
					}
				}
					fclose(ptr_read_news_num);
					if (check1 == 1)						// Checks if the seleceted new is already read.
					{
					printf("This new is read. Do you want to read it again ? Yes(1) / No(0) :\n");			// Asks to user if he/she still wants to read it again.
					scanf("%d",&num);

							if (num == 1)
							{
							read_news(string,news_path,0);
							printf("%s\n",string);
							}
								else if (num == 0)
								{
								printf("Good Bye!\n");						// If he/she doesn't want to read it prints "GoodBye!"
								}
					}		
						else if (check1 == 0)								// If selected new hasn't read before prints it.
						{
						read_news(string,news_path,0);
						printf("%s\n",string);								// Adds selected new's number to end of the readed_news_id.txt.It doesn't add any number to list more than one. 
						append_file(read_news_path, read_news_char);	
						}	
			
			printf("Do you want to continue? Yes(y)/No(n)\n");				// Asks user if he/she wants to continue or exit.
			scanf(" %c",&is_continue);
			break;						
				
				case 'b':
				ptr_read_news_num = fopen("readed_news_id.txt","r");		// Lists the read news.
				rewind(ptr_read_news_num);

					while (!feof(ptr_read_news_num))
					{
					fscanf(ptr_read_news_num,"%d",&y);
					printf("%d. is read\n",y);							
					}

			fclose(ptr_read_news_num);
			printf("Do you want to continue? Yes(y)/No(n)\n");				// Asks user if he/she wants to continue or exit.
			scanf(" %c",&is_continue);
			break;
					case 'c':
					printf("Which news text you want to read ?\n");			// Decrypts the seleceted new.
					scanf("%d", &text_choice);
					sprintf(news_path, "news/%d.txt",text_choice);			
					read_news(string,news_path,0);							// Prints the new.
					printf("%s",string);
					read_magic_numbers(magic,string);						
					magic_result = 0;

						for (i = 0;i <strlen(magic);i++)
						{
						magic_int = magic[i] - 48;					// It substracts 48 from strings each element so i can get integers i want.
						magic_num = g_func(f_func,magic_int);		// Puts each integer to function.
						magic_result += magic_num; 					// It adds all results to each other.
						}
							if (text_choice == 1)	printf("\nnumber of tests performed = %lf\n",magic_result);					// Prints the decrypted message.
								else if (text_choice == 2)	printf("\nnumber of sick people = %lf\n",magic_result);
									else if (text_choice == 3)	printf("\nnumber of deaths = %lf\n",magic_result);
										else if (text_choice == 4) printf("\nexpected number of sick people = %lf\n",magic_result);
					printf("Do you want to continue? Yes(y)/No(n)\n");			// Asks user if he/she wants to continue or exit.
					scanf(" %c",&is_continue);
					break;

						default :
						printf("Invalid Operator!\n");
						printf("Do you want to continue? Yes(y)/No(n)\n");
						scanf(" %c",&is_continue);
						break;								
		}
	}
fclose(ptr_news_num);
fclose(ptr_check);
}

void read_news(char buffer[500], char file_path[20], int is_Only_Title)
{

FILE* ptr1;

ptr1 = fopen(("%s",file_path),"r");				// Takes the file's address
	
		if (is_Only_Title == 0)					// Reads the title
		{
			fread(buffer,500,1,ptr1);		
		}
				else if (is_Only_Title == 1)	// Reads whole new.
				{
				fgets(buffer,100,ptr1);
				}
fclose(ptr1);
}

void append_file(char* file_path, char c)
{

FILE* ptr2;
ptr2 = fopen(("%s",file_path),"a");				// Takes the file's address
fprintf(ptr2," %c",c);							// Adds c to end of the file.

fclose(ptr2);
}

double f_func (int x)							// f_func for decryption
{
double result;
result = x * x * x -(x * x) + 2;				
return result;
}

double g_func (double f_func (int x),int a)		// g_func for decryption
{
double result;
result = f_func(a) * f_func(a);
return result;
}

void read_magic_numbers(char buffer_magic[10], char buffer_news[500])
{

int i;
int k = 0;
int magic;


	for (i = 0; i < strlen(buffer_news) - 1; i++)
	{
		if (buffer_news[i] == '#')					// When it reads '#' character it adds next character (which is an integer) to another string.
		{
		magic = buffer_news[i + 1];
		buffer_magic[k] = magic;
		k += 1;
		}	
		
	}
	buffer_magic[k] = '\0';
}