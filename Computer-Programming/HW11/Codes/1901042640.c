#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct node 
{
int value;
struct node * next;
};
typedef struct node node_t;
node_t * head;
node_t * tmp;

struct new
{
int n1, n2;
};

int * read_array(char * filename, int *x);
void read_linkedlist (node_t * tmp, char * filename);
float * stats_array (int * array, int limit);
float * stats_linkedlist (node_t * x, int limit);
float mean_value_array (int * array, int limit);
float mean_value_linkedlist (node_t * x, int limit);
int sizeof_array (int * array, int limit);
int sizeof_linkedlist (node_t * x);
struct new * compare ( int * array, node_t * x1, int * counter);

int main ()			
{
int * dynamic_array;
float * array_stats, * linkedlist_stats;
char * filename = "list.txt";
int i, x, size;
int size_array, size_linkedlist;
double time1, time2;
double sum_time1 = 0, sum_time2 = 0;
double average_time1, average_time2;

time_t start, end;

start = clock();
dynamic_array = read_array (filename, &x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time1 += time1;	

start = clock ();
read_linkedlist (tmp, filename);
end = clock ();
time2 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time2 += time2;

start = clock();
dynamic_array = read_array (filename, &x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time1 += time1;

start = clock ();
read_linkedlist (tmp, filename);
end = clock ();
time2 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time2 += time2;

start = clock();
dynamic_array = read_array (filename, &x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time1 += time1;	

start = clock ();
read_linkedlist (tmp, filename);
end = clock ();
time2 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time2 += time2;

start = clock();
dynamic_array = read_array (filename, &x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time1 += time1;	

start = clock ();
read_linkedlist (tmp, filename);
end = clock ();
time2 = (double)(end - start) / CLOCKS_PER_SEC;
sum_time2 += time2;

average_time1 = sum_time1 / 4;
average_time2 = sum_time2 / 4;

printf("PART 1______________________\n");
printf("Time taken for read_array function is %lf seconds.\n", average_time1);
printf("Time taken for read_linkedlist function is %lf seconds.\n", average_time2);

size = sizeof_array (dynamic_array, x);
printf("Total memory in bytes required for read_array is %d .\n", size);
size = sizeof_linkedlist (head);
printf("Total memory in bytes required for read_linkedlist is %d .\n", size);

start = clock ();
array_stats = stats_array (dynamic_array, x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
printf("Time taken for stats_array function is %lf seconds.\n", time1);

start = clock();
linkedlist_stats = stats_linkedlist (head, x);
end = clock ();
time1 = (double)(end - start) / CLOCKS_PER_SEC;
printf("Time taken for stats_linkedlist function is %lf seconds.\n\n", time1);
printf("For Dynamic Array :\n" );
printf("Minimum Value : %f\n",array_stats[0] );
printf("Maximum value : %f\n",array_stats[1] );
printf("Mean Value : %f\n",array_stats[2] );
printf("Standard Deviation : %f\n\n",array_stats[3] );
printf("Fot Linked List :\n");
printf("Minimum Value : %f\n",linkedlist_stats[0] );
printf("Maximum value : %f\n",linkedlist_stats[1] );
printf("Mean Value : %f\n",linkedlist_stats[2] );
printf("Standard Deviation : %f\n\n",linkedlist_stats[3] );

printf("PART2____________________\n");

node_t * y;
struct new * ptr1;
int counter;

int * array = (int *) calloc (10, sizeof (int));
array[0] = 11;	array[1] = 2;	array[2] = 35;	array[3] = 14;	array[4] = 17;	array[5] = 67;
array[6] = 55;	array[7] = 98;	array[8] = 32;	array[9] = 22;	

y = (node_t *) malloc (sizeof (node_t));
y-> value = 11;
y->next  = (node_t *) malloc (sizeof (node_t));
y->next->value = 2;
y->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->value = 35;
y->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->value = 13;
y->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->value = 17;
y->next->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->next->value = 67;
y->next->next->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->next->next->value = 55;
y->next->next->next->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->next->next->next->value = 99;
y->next->next->next->next->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->next->next->next->next->value = 32;
y->next->next->next->next->next->next->next->next->next = (node_t *) malloc (sizeof (node_t));
y->next->next->next->next->next->next->next->next->next->value = 22;
y->next->next->next->next->next->next->next->next->next->next = NULL;

ptr1 = compare (array, y, &counter);
	
	for (i = 0; i < counter; i++)
	{
	printf("%d-", ptr1[i].n1);
	printf("%d\n", ptr1[i].n2);
	}
return 0;	
}


int * read_array(char * filename, int *x)
{
int i, counter = 0, num, lim;
int * array;
FILE* ptr;
ptr = fopen (("%s", filename), "r");
fseek (ptr, 0, SEEK_END);
lim = ftell (ptr);
fseek (ptr, 0, SEEK_SET);
array =(int*)malloc(sizeof (int));

	while (ftell (ptr) != lim)
	{	
	fscanf (ptr, "%d", &num);
		if (ftell (ptr) != lim)		fseek (ptr, 1, SEEK_CUR);
	array [counter] = num;
	counter++;
	array = (int *)realloc (array, (counter + 1)* sizeof (int) );
	}

fclose (ptr);
*x = counter;
return array;
}



void read_linkedlist (node_t * tmp, char * filename)
{

int num, lim, counter = 0;
int i;
FILE* ptr;
ptr = fopen (("%s", filename), "r");

fseek (ptr, 0, SEEK_END);
lim = ftell (ptr);
fseek (ptr, 0, SEEK_SET);

fscanf (ptr, "%d", &num);
fseek (ptr, 1, SEEK_CUR);
head = (node_t *) malloc (sizeof (node_t));
head-> value = num;
tmp = head;

	while (ftell (ptr) != lim)
	{
	fscanf (ptr, "%d", &num);
		if (ftell (ptr) != lim)		fseek (ptr, 1, SEEK_CUR);
	counter++;
	tmp -> next = (node_t*)malloc (sizeof(node_t));
	tmp = tmp -> next;
	tmp -> value = num;
	tmp -> next = NULL;
	}

fclose (ptr);
}

float * stats_array (int * array, int limit)
{
float * result = (float*)calloc (4, sizeof(float)); 
int i;	
float max = array[0];
float min = array[0];
float mean = mean_value_array (array, limit);
float x;
float sum = 0;
	
	for (i = 0; i < limit; i++)
	{
		if (array[i] > max)		max = array[i];
			if (array[i] < min)		min = array[i];
		x = (array[i] - mean) * (array[i] - mean);
		sum += x;
	}

result[0] = min;
result[1] = max;
result[2] = mean;
result[3] = sqrt (sum / (limit - 1));
return result;
}

float * stats_linkedlist (node_t * x, int limit)
{
node_t * tmp;
tmp = x;
float * result = (float*)calloc (4, sizeof(float)); 
int i;
float sum = 0;
int num;
float y;
int max = tmp-> value;
int min = tmp-> value;
float mean = mean_value_linkedlist (x, limit);

	while (tmp != NULL)
	{
	num = tmp-> value;
	y = (num - mean) * (num - mean);
	sum += y;

		if (num > max)		max = num;
			if (num < min)	min = num;
	tmp = tmp -> next;
	}

result[0] = min;
result[1] = max;
result[2] = mean;
result[3] = sqrt (sum / (limit - 1));
return result;
}

float mean_value_array (int * array, int limit)
{
int i;
int sum = 0;
float result;

	for (i = 0; i < limit; i++)		sum += array[i];

result = (float)(sum / limit); 	
return result;
}

float mean_value_linkedlist (node_t * x, int limit)
{
node_t * y = x;
int i, sum = 0;
float result;
	for (i = 0; i < limit; i++)
	{
	sum += y-> value;
	y = y-> next;
	}
result = (float)(sum / limit);
return result;	
}

int sizeof_array (int * array, int limit)
{
int i, sum = 0;
	
	for (i = 0; i < limit; i++)		sum += sizeof (array[i]);
return sum;
}

int sizeof_linkedlist (node_t * x)
{
int num, sum = 0;
node_t * tmp = x;
	
	while (tmp != NULL)
	{
	num = sizeof (tmp);
	sum += num;
	tmp = tmp-> next;
	}
return sum;
}

struct new * compare ( int * array, node_t * x1, int * counter)
{
struct new * ptr2;
int counter1 = 0, counter2 = 0, num1, num2;
node_t * tmp; 
tmp = x1;
ptr2 =(struct new *) malloc(sizeof (struct new));

	while (tmp != NULL)
	{		
	num2 = tmp-> value;
	num1 = array[counter1];

		if (num1 != num2)
		{
		ptr2[counter2].n1 = num1;
		ptr2[counter2].n2 = num2;
		counter2++;
		ptr2 =(struct new *)realloc (ptr2, (counter2 + 1) * sizeof (struct new ));
		}	

	tmp = tmp->next;
	counter1++;	
	}
* counter = counter2;
return ptr2;
}