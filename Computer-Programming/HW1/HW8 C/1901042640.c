#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void menu();
void part1();
void generate_hofstadters_sequence (int *arr, int n);
int find_max (int arr[], int index, int max_value);
int sum_array (int arr[]);
double std_array (int arr[], double *mean, int n, int index);
void part2();
void install_pipes (int visited[8][8], int x, int y, int orientation);
void part3 ();
char* remove_duplicates (char* str);

int global;
int global_array[64];

int main ()
{
menu();
return 0;  
}

void menu()
{

int choice_int; 
char choice_string[10];
    while(1)
    {
printf("Welcome to the Homework 8, Please chose one of the parts to continue\n______________________\n\n");
printf("1) Execute Part 1\n2) Execute Part 2\n3) Execute Part 3\n");
fgets(choice_string,10,stdin);
choice_int = atoi(choice_string);       

    switch (choice_int)
    {
        case 1:
        part1();
        break;
            case 2:
            part2();
            break;
                case 3:
                part3();
                break;
    }

    }
}

// Part 1 Functions :

void part1 ()
{
int choice2;
int array[500];     // It stores numbers of the sequence
int num;            
int max;
int sum;
int num_backup;
double mean_value;  
double std_deviasion;

while(1)
{

printf("Please make your choice :\n_______________\n");
printf("1) Fill Array\n2) Find Biggest Number\n3) Calculate Sum\n4) Calculate Standard Devitation\n5) Exit\n");
scanf("%d",&choice2);

    switch(choice2)
    {
        case 1: 
        printf("Please enter a number\n");
        scanf("%d",&num);
        num_backup = num;
            if (num <= 500)  generate_hofstadters_sequence(array,num);
                else printf("ERROR !\n");
        break;
            case 2:
            max = array[0];
            printf("%d\n",*array +10);
            printf("%d\n",find_max(array,num_backup - 1,max));
            break;
                case 3:
                sum = 0;
                global = 0;
                sum = sum_array(array);
                printf("%d\n",sum);
                break;
                    case 4:
                    global = 0;
                    num = 0;
                    sum = num_backup - 1;
                    std_deviasion = std_array(array,&mean_value,num,sum);
                    printf("Standard Deviasion :%lf\n",std_deviasion);
                    printf("Mean Value :%lf\n",mean_value );
                    break;
                        case 5:
                        exit(0);
                        break;
                            default:
                            printf("Invalid Operation!\n");
                            break;
    }
}

}

void generate_hofstadters_sequence (int *arr, int n)
{
int i;

arr[0] = 1;
arr[1] = 1;

    for (i = 2; i <= n; i++)
    {
    arr[i] = arr[i - arr[i - 1]] + arr[i - arr[i - 2]]; 
    }

        for ( i = n; i < 500; i++)    arr[i] = 0;
}

int find_max (int arr[], int index, int max_value)
{
    if ( index == 0)    return max_value;

        if (index > 0)
        {   
            if (arr[index] > max_value)   max_value = arr[index];

        return find_max (arr, index - 1,max_value );                
        }   
}


int sum_array (int arr[])
{   
    if (*arr == 0 || global == 499)     return *arr;
            else 
            {
            global++;       
            return sum_array(arr + 1) + *arr;
            }
}


double std_array (int arr[], double *mean, int n, int index)
{

double result;

if (n == 0)
{
*mean = (double)sum_array(arr) / (index + 1);
}

    if (n < index + 1) 
    {    
    global = global + pow(arr[n] - *mean, 2);           // It summates every element inside the square root.
    }
        else 
        {
        result = sqrt(global / index);                  // Calculates standard deviation.
        return result;
        } 


std_array(arr,mean,n + 1,index);

}


// Part 2 Fucntions :

void part2()
{

int x,y;
int orientation = 0;
x = 0;
y = 0;
global = 0;
int array[8][8];
int i,k;

    for (i = 0; i < 8; i++)
    {
        for (k = 0; k < 8; k++)     array[i][k] = 0;        // Puts 0 to every slot on array.
    }


install_pipes(array, 0, 0, orientation);                  


}

void install_pipes (int visited[8][8], int x, int y, int orientation)       // It takes a while to complete (It finds first six one immediately then it takes 30 seconds to calculate rest of them)
{

int new_x;
int new_y;
int i,j;
int is_valid;

int row[] = { 2, 1, -1, -2, -2, -1,  1, 2};
int col[] = { 1, 2,  2,  1, -1, -2, -2, -1};

visited[x][y] = orientation;

   if (orientation >= 64)
    {
        for (i = 0; i < 64; i++)
        {
        printf("O%d ",global_array[i]+1);       // Prints the operation list for each setup.
             
        }
    printf("\n");
    global++;
    visited[x][y] = 0;
    if (global == 10)  exit(0);                 // Exits after 10 setups.
    return;
    }


        for (i = 0; i < 8; i++)
        {
        new_x = x + row[i];
        new_y = y + col[i];
            if (x < 0 || y < 0 || x >= 8 || y >= 8)    is_valid = 0;    // Checks if cordinate got out of bounds.
                else is_valid = 1;

                if (is_valid && !visited[new_x][new_y])         // if cordinate is in bounds and there is no other bnumber is attached at that location,proceeds.
                {
                global_array[orientation] = i;          
                install_pipes(visited, new_x, new_y, orientation + 1);
                }
        }


visited[x][y] = 0;
      
}

// part 3  Functions :

void part3 ()
{
char string[100]; 

printf("Enter a string :\n");
fgets(string,100,stdin);
remove_duplicates(string);
printf("%s\n",string);
}


char* remove_duplicates (char* str)
{

    if (str[0] == '\0') return str;

        if (str[0] == str[1])
        {
        int i = 0;
            while (str[i] != '\0')          // If there is same characters next to each other,it moves that part of the string by one character to the left.
            {
            str[i] = str[i + 1];
            i++; 
            }

        remove_duplicates(str);
        }
    
        remove_duplicates(str + 1);
}
