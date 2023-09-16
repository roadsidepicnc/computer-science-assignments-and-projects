#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>




struct point
{
char name1[100];
float cordinates[2];
};

struct point P[100];

struct line
{
char name2[100];
struct point lines[2];

};

struct line L[100];

struct polygon
{
char name3[100];
int p;
int l;
int size;
struct point polygons[20];
struct line polylines[20];
};

struct polygon PG[20];

void commands(struct point P[100], struct line L[100], struct polygon PG[20]);
void actions (struct point P[100], struct line L[100], struct polygon PG[20]);
float distance_point (float x1, float y1, float x2, float y2);
float distance_line (float x0, float y0, float x1, float y1, float x2, float y2);
float angle (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
float area (int num,struct point P[100], struct line L[100], struct polygon PG[20] );

int main()
{
int counter = 0;
int i, k,m;
commands(P, L, PG);

actions(P, L, PG);


return 0;
}


void commands(struct point P[100], struct line L[100], struct polygon PG[20])
{
int check = 0;
int counter_main = 0;
int counter_axis = 0;
int counter_element = 0;
int counter_point = 0;
int counter_line = 0;
int counter_polygon = 0;
int x0,x2;
float  x1;
int i;
char z[100];
char* token; 
char* rest;
FILE* ptr;
ptr = fopen ("commands.txt","r");

fgets (z, 100, ptr);
fgets (z, 100, ptr);
x0 = atoi (z);
	
		while (counter_main < x0)
		{
		fgets (z, 100, ptr);
		rest = z;
		check = 0;
			if (z[0] != 'P')			
			{
			counter_axis = 0;	
				 while ((token = strtok_r(rest, " ", &rest)))
				 {
				 	if (counter_axis < 2)
				 	{
					x1 = atof (token);
					P[counter_point].cordinates[counter_axis] = x1;
					}
						
				
							else if (token[0] == 'P' && !check)
							{
							check = 1;
							counter_main++;
							strcpy(P[counter_point].name1 , token);
							P[counter_point].name1[3] = '\0'; 
							counter_point++;
							}

				 counter_axis++;
				 }

			}
				else if (z[6] == 'L')
				{
				counter_element = 0;
			
					while ((token = strtok_r(rest, " ", &rest)))
				 	{

				 		if (token[0] == 'L'  && check == 2)
				 		{
				 		counter_main++;
						strcpy (L[counter_line].name2 , token);
						L[counter_line].name2[3] = '\0';
						check++;
				 		}
				 				else if (token[0] == 'P' && check < 2)
				 				{
				 				x2 = token[1] - 49;	 				
				 				L[counter_line].lines[counter_element].cordinates[0] = P[x2].cordinates[0];
				 				L[counter_line].lines[counter_element].cordinates[1] = P[x2].cordinates[1];
				 				check++;
				 				}
				 				
				 	counter_element++;

				 	}

				counter_line++;
				}
					

					else
					{
					counter_element = 0;
						while ((token = strtok_r(rest, " ", &rest)))
				 		{
				 			if (token[1] == 'G' && !check)
				 			{
				 			counter_main++;
				 			strcpy(PG[counter_polygon].name3 , token);
				 			PG[counter_polygon].name3[3] = '\0';
				 			check = 1;
				 			}
				 				else if (token[0] == 'P' && !check)
				 				{
				 				PG[counter_polygon].p = 1;
				 				PG[counter_polygon].l = 0;
				 				x2 = token[1] - 49;
				 				PG[counter_polygon].polygons[counter_element].cordinates[0] = P[x2].cordinates[0];
				 				PG[counter_polygon].polygons[counter_element].cordinates[1] = P[x2].cordinates[1];
				 				PG[counter_polygon].size = counter_element;
				 				
				 				}
				 					else if (token[0] == 'L' )
				 					{
				 					PG[counter_polygon].p = 0;
				 					PG[counter_polygon].l = 1;
				 					}

				 		counter_element++;
				 		}

					counter_polygon++;
					}
		}
fclose (ptr);
}


void actions (struct point P[100], struct line L[100], struct polygon PG[20])
{
char test_string[20] = "actions";
int test_int = 1;
int i;
char z[100];
int check = 0;
int x0,x1,x2;
char output[50];
int counter_step = 0;
float result;

char* token; 
char* rest;

FILE* ptr_w;


FILE* ptr2;
ptr2 = fopen ("commands.txt","r");

fgets (z, 100, ptr2);	
	
	while (!feof(ptr2))
	{	
	test_int = 1;
	fgets (z, 100, ptr2);


		for (i = 0; i < 7; i++)
		{
			if (z[i] != test_string[i])		test_int = 0;
		}
		
		if (test_int)		break;

	}

		if (test_int)
		{

		fgets (z, 100, ptr2);
		strcpy(output, z);
		output[strlen(output) - 2] = '\0';
		fgets (z, 100, ptr2);
		x0 = atoi (z);
		ptr_w = fopen (("%s",output),"w");


			while (counter_step < x0)
			{
			fgets (z, 100, ptr2);
			rest = z;
			
					if (z[0] == 'D' && z[9] == 'P' && z[12] == 'P')
					{
					x1 = z[10] - 49;
					x2 = z[13] - 49;	
					result = distance_point (P[x1].cordinates[0], P[x1].cordinates[1], P[x2].cordinates[0], P[x2].cordinates[1]);
					
					fprintf( ptr_w , "Distance(P%d,P%d) = %f \n",x1 , x2, result);		
					}

						else if (z[0] == 'D' && (z[9] == 'P' || z[9] == 'L') && (z[12] == 'P' || z[12] == 'L'))
						{
						x1 = z[10] - 49;

							for (i = 0; i < 100; i++)
							{
								if ((z[13] == L[i].name2[1] && z[14] == L[i].name2[2]) || (z[10] == L[i].name2[1] && z[11] == L[i].name2[2]))
								{
								result = distance_line (P[x1].cordinates[0], P[x1].cordinates[1], L[i].lines[1].cordinates[0], L[i].lines[1].cordinates[1], L[i].lines[0].cordinates[0], L[i].lines[0].cordinates[1]);

														
								
									if (z[9] == 'P')	fprintf( ptr_w , "Distance(P%d,L%c%c) = %f \n",x1 , z[13],z[14], result);
        								else 	fprintf( ptr_w , "Distance(L%c%c,P%d) = %f \n",z[10],z[11] , x2, result);
								
								}

							}	

						}
							else if (z[0] == 'A' && z[1] == 'n')
							{
							int num1, num2;
								for (i = 0; i < 100; i++)
								{
									if (z[7] == L[i].name2[1] && z[8] == L[i].name2[2])		num1 = i;
										else if (z[11] == L[i].name2[1] && z[12] == L[i].name2[2])		num2 = i;
								}

							result = angle (L[num1].lines[1].cordinates[0], L[num1].lines[1].cordinates[1], L[num1].lines[0].cordinates[0], L[num1].lines[0].cordinates[1],
											L[num2].lines[1].cordinates[0], L[num2].lines[1].cordinates[1], L[num2].lines[0].cordinates[0], L[num2].lines[0].cordinates[1]);	
							
							fprintf( ptr_w , "Angle(L%c%c,L%c%c) = %f \n",z[7], z[8],z[11],z[12], result);	
							}
								else if (z[0] == 'L' && z[7] == 'L')
								{
									for (i = 0; i < 100; i++)
									{
										if (z[8] == L[i].name2[1] && z[9] == L[i].name2[2])
										{
										result = distance_point(L[i].lines[1].cordinates[0], L[i].lines[1].cordinates[1], L[i].lines[0].cordinates[0], L[i].lines[0].cordinates[1]);
										
										fprintf( ptr_w , "Length(L%c%c) = %f \n",z[8],z[9] ,result);	
										}

									}	

								}

									else if (z[0] == 'L' && z[8] == 'G')
									{
				
									fprintf(ptr_w, "NOT IMPLEMENTED\n" );

									}

										else if (z[0] == 'A' && z[1] == 'r')
										{
										x1 = z[7] - 49;
											if (PG[x1].p)
											{
											result = area (x1,P,L,PG);
											fprintf(ptr_w, "Area (PG%d) = %f\n",x1 + 1,result);
											}
												else if (PG[x1].l)		fprintf(ptr_w, "NOT IMPLEMENTED\n" );
										}

			counter_step++;
			}

		}
fclose(ptr2);
fclose(ptr_w);
}


float distance_point(float x1, float y1, float x2, float y2)
{
float z_y;
float z_x;
float result;
z_y = pow(y2 - y1, 2);
z_x = pow(x2 - x1, 2);
result = sqrt (z_y + z_x);
return result;
}

float distance_line (float x0, float y0, float x1, float y1, float x2, float y2)
{
float z_y = y2 - y1;
float z_x = x2 - x1;
float m = z_y / z_x;

float n = abs (y2 - (m * x2));
float result = abs ((m * x0 - y0 + n)) / sqrt (1 + m * m);
return result;
}

float angle (float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
float z_y1;
float z_x1;
float z_y2;
float z_x2;
float m1,m2;
float result;
float i, k;
z_y1 = y2 - y1;
z_x1 = x2 - x1;
z_y2 = y4 - y3;
z_x2 = x4 - x3;
m1 = z_y1 / z_x1;
m2 = z_y2 / z_x2;
result = atan ((m2 - m1) / (1 - m1 * m2))* 180 / 3.14;

		if (m1 == 0 && m2 == 0)		result = 0; 
		 else if (m1 == m2)		result = 0;
			else if (m1 * m2 == -1)		result = 90;
				else if (!isfinite(result))		result = 90;
return result ;
}

float area (int num,struct point P[100], struct line L[100], struct polygon PG[20] )
{
int size = PG[num].size;
int i;
float sum1 = 0;
float sum2 = 0;
float result;

	for (i = 0; i < size; i++)		sum1 = sum1 + PG[num].polygons[i].cordinates[0] * PG[num].polygons[i + 1].cordinates[1];
		for (i = size; i > 0; i--)		sum2 = sum2 + PG[num].polygons[i].cordinates[0] * PG[num].polygons[i - 1].cordinates[1];

result = abs (sum1 - sum2) / 2;

return result;

}

