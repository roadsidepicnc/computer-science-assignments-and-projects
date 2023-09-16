#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

extern char **environ;

int sigEnd = 0;

int varCalculator(int arr [], int avg, int length);
int covCalculator(int arr1 [], int arr2 [], int avg1, int avg2, int length);
int ** covarianceMatrixCalculator(int x_arr [], int y_arr [], int z_arr [], int length);
char * mergeString(int ** arr, char * processNum);
void handler (int sig);

	////////////////////////////////////////////////////////////////////////

/* Returns an 2d caovariant matrix
*/
int ** covarianceMatrixCalculator(int x_arr [], int y_arr [], int z_arr [], int length)
{
	int avg_x, avg_y, avg_z, sum = 0;
	int cov_xy, cov_xz, cov_yz;
	int var_x, var_y, var_z;

	for(int i = 0; i < length; i++)	sum += x_arr[i];
 	avg_x = sum / length;

 	sum = 0;
 	for(int i = 0; i < length; i++)	sum += y_arr[i];
 	avg_y = sum / length;

 	sum = 0;
 	for(int i = 0; i < length; i++)	sum += z_arr[i];
 	avg_z = sum / length;

	var_x = varCalculator(x_arr, avg_x, length);
	var_y = varCalculator(y_arr, avg_y, length);
	var_z = varCalculator(z_arr, avg_z, length);

	cov_xy = covCalculator(x_arr, y_arr, avg_x, avg_y, length);
	cov_xz = covCalculator(x_arr, z_arr, avg_x, avg_z, length);
	cov_yz = covCalculator(y_arr, z_arr, avg_y, avg_z, length);

	int** result = (int**) malloc(3 * sizeof(int*));
    for (int i = 0; i < 3; i++)
        result[i] = (int*) malloc(3 * sizeof(int));

	result[0][0] = var_x; 
	result[0][1] = cov_xy; 
	result[0][2] = cov_xz; 
	result[1][0] = cov_xy; 
	result[1][1] = var_y; 
	result[1][2] = cov_yz; 
	result[2][0] = cov_xz; 
	result[2][1] = cov_yz; 
	result[2][2] = var_z; 

	return result;
}

int covCalculator(int arr1 [], int arr2 [], int avg1, int avg2, int length)
{
	int temp = 0;

	for(int i = 0; i < length; i++) temp += (arr1[i] - avg1) * (arr2[i] - avg2);
	return temp / length;
}

int varCalculator(int arr [], int avg, int length)
{
	int temp = 0;

 	for(int i = 0; i < length; i++) 
 		{
 			int t =pow((arr[i] - avg), 2);
 			temp += t;
 		}
 	return temp / length;
}

char * mergeString(int ** arr, char * processNum)
{
	char * result = (char*) malloc(100 * sizeof(char));
	sprintf(result, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%s\n", arr[0][0], arr[0][1], arr[0][2], arr[1][0] ,arr[1][1], arr[1][2], arr[2][0], arr[2][1], arr[2][2], processNum);
	return result;
}

/* Signal handler
*/
void handler (int sig)
{
	int saved;
	saved = errno;
	sigEnd = 1;
	errno = saved;
}

int main(int argc, char * argv[])
{
	int fdout;
	int x_arr[10];
	int y_arr[10];
	int z_arr[10];
	char * str;
	int ** arr;

	struct sigaction sigAct;
	memset(&sigAct, 0, sizeof(sigAct));
	sigAct.sa_handler = &handler;
	sigAct.sa_flags = 0;

	if((sigemptyset(&sigAct.sa_mask) == -1) || (sigaction(SIGINT, &sigAct, NULL) == -1))
	{
		perror("Couldn't install sa_handler");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < 10; i++)
	{
		x_arr[i] = environ[0][0 + i * 3];
		y_arr[i] = environ[0][1 + i * 3];
		z_arr[i] = environ[0][2 + i * 3];
	}
	
	arr = covarianceMatrixCalculator(x_arr, y_arr, z_arr, 10);
	str = mergeString(arr, environ[1]);

	if(sigEnd)
	{
		free(arr);
		free(str);

		if(close(fdout) == -1)
		{
			perror("Error occured while closing the output file at R_i");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	struct flock lock;
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	fcntl(fdout, F_SETLKW, &lock);	// Locks the file for if any other child process tries the reach. If other child process tries to reach, it waits

	fdout = open(argv[1], O_WRONLY | O_APPEND);

	lseek(fdout, 0, SEEK_END);
	
	write(fdout, str, strlen(str));

	lock.l_type = F_UNLCK;
	fcntl(fdout, F_SETLKW, &lock);

	printf("Created R_%s with ", environ[1]);
	for(int i = 0; i < 10; i++)
	{
		printf("(%d,%d,%d),", x_arr[i], y_arr[i], z_arr[i]);
	}
	printf("\n");

	free(arr);
	free(str);
	int closeID = close(fdout);	

	if(closeID == -1)	
	{
		perror("ERROR: Couldn't Close the File!\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}