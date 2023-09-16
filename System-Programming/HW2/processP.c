#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

	////////////////////////////////////////////////////////////////////////

float frobeniusNormCalculator(float arr [], int length);
char * readLine(int * fdin);
float * readLineOutputFile(int * fd);
float * readOutputFileMain(char * fileName, int limit, int idArr []);
void closestDistanceCalculator(float * distanceArr, int * idArr, int limit, int * idFin1, int * idFin2, float * minDistFin);
void handler (int sig);
void killChildProcesses(int * ChildIDs, int limit);

	////////////////////////////////////////////////////////////////////////

int sigEnd = 0;

	////////////////////////////////////////////////////////////////////////

/* Calculates the distance from the Frobenius Norm
*/
float frobeniusNormCalculator(float arr [], int length)
{
	float sum = 0;
	for(int i = 0; i < length; i++) sum += pow(arr[i], 2);
	return (float)sqrt(sum);
}

/*	Reads 30 bytes from the input file
*/
char * readLine(int * fdin)
{
	unsigned char buffer [3];
	char * result;
	int counter = 0;
	size_t bytes_read;

	result = (char*) malloc(30 * sizeof(char));

	while(counter < 10)
	{
		bytes_read = read(*fdin, buffer, sizeof(buffer));
		
		if(bytes_read != sizeof(buffer))
		{
			return NULL;
		}

		result[0 + counter * 3] = buffer[0];
		result[1 + counter * 3] = buffer[1];
		result[2 + counter * 3] = buffer[2];
		counter++;
	}

	return result;
}

/* Reads 1 line from the output file
*/
float * readLineOutputFile(int * fd)
{
	float * arr;
	char buffer [1];
	char tempStr[5];
	size_t bytes_read = sizeof(buffer);
	int counter1 = 0, counter2 = 0;
	
	arr = (float*) malloc(50 * sizeof(float));

	while(counter1 < 10)
	{
		bytes_read = read(*fd, buffer, sizeof(buffer));

		if(buffer[0] == ',' || buffer[0] == '\n' || bytes_read != sizeof(buffer))
		{

			arr[counter1] = atof(tempStr);
			counter2 = 0;
			counter1++;
			for(int j = 4; j >= 0; j--) tempStr[j] = '\0';
		}
		else
		{
			tempStr[counter2] = buffer[0];
			counter2++;
		}
	}
	return arr;

}

/* Reads all of the output file
*/
float * readOutputFileMain(char * fileName, int limit, int idArr [])
{
	int fdout;
	float * subResults = (float*) malloc(limit * sizeof(float));
	float** result = (float**) malloc(limit * sizeof(float*));
    for (int i = 0; i < limit; i++)
        result[i] = (float*) malloc(10 * sizeof(float));

    fdout = open(fileName, O_RDONLY);

    struct flock lock;
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_RDLCK;
	if(fcntl(fdout, F_SETLKW, &lock) == -1)
	{
		perror("Other file descriptor is using the file!\n");
		return NULL;
	}	

    if(fdout == -1)
	{
		perror("Output file couldn't be opened!");
		exit(0);
	}

    printf("Reached EOF, collecting outputs from %s\n", fileName);

    for(int i = 0; i < limit; i++)
    {
    	result[i] = readLineOutputFile(&fdout);
    	idArr[i] = result[i][9];
    	subResults[i] = frobeniusNormCalculator(result[i], 9);
    }
    return subResults;
}

/* Finds the closest 2 matrixes to each other
*/
void closestDistanceCalculator(float * distanceArr, int * idArr, int limit, int * idFin1, int * idFin2, float * minDistFin)
{
	float minDist = abs((float)distanceArr[0] - (float)distanceArr[1]);
	int id1 = idArr[0], id2 = idArr[1];

	for(int i = 0; i < limit - 1; i++)
	{
		for(int j = i + 1; j < limit; j++)
		{
			if(abs((float)distanceArr[i] - (float)distanceArr[j]) < minDist)
			{
				id1 = idArr[i];
				id2 = idArr[j];
				minDist = abs(distanceArr[i] - distanceArr[j]);
			}
		}
	}

	*idFin1 = id1;
	*idFin2 = id2;
	*minDistFin = minDist;
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

/* Kills all of the child processes. Uses the child process id's stored in an array
*/
void killChildProcesses(int * ChildIDs, int limit)
{
	for(int i = 0; i < limit; i++)
	{
		kill(ChildIDs[i], SIGTERM);
	}
}

	////////////////////////////////////////////////////////////////////////

int main(int argc, char * const argv[])
{
	int fdin, fdout, flag = 0, counter = 0;
	char tempOutputFileName[30];
	int processIDs [1000]; 

	if(argc != 5)
	{
		perror("Wrong amount of command line arguments");
		exit(EXIT_FAILURE);
	}

	struct sigaction sigAct;
	memset(&sigAct, 0, sizeof(sigAct));
	sigAct.sa_handler = &handler;
	sigAct.sa_flags = 0;

	if((sigemptyset(&sigAct.sa_mask) == -1) || (sigaction(SIGINT, &sigAct, NULL) == -1))
	{
		perror("Couldn't install sa_handler");
		exit(EXIT_FAILURE);
	}

	fdin = open(argv[2], O_RDONLY);

	if(fdin == -1)
	{
		perror("Input file couldn't be opened!");
		return(0);
	}

	fdout = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0666);

	if(fdout == -1)
	{
		perror("Output file couldn't be created!");
		return(0);
	}

	close(fdout);

	printf("ProcessP reading %s\n", argv[2]);

	strcpy(tempOutputFileName, argv[4]);
	
		while(flag == 0) 
   		{
   			char tempStr [10]; 

   			if(sigEnd == 1)	// if SIGINT signal comes it will end all of the child processes. It will release the allocated memory and close the files
   			{
				killChildProcesses(processIDs, counter);
				for(int i = 0; i < counter; i++) wait(NULL);
				close(fdin);
				remove(argv[4]);	// Deletes output file
				exit(0);
			}	

   			sprintf(tempStr, "%d", counter);
   			char * const envVec [50] = {readLine(&fdin), tempStr, NULL};

   			if(envVec[0] == NULL)
   			{
   				flag = 1;
   			}
   			else
   			{
   				pid_t child_pid = fork();

       		if(child_pid == 0)
       		{		

       			char * const arguments [50] = {"./R_i", tempOutputFileName, NULL};
            	execve(arguments[0], arguments, envVec);
   			    exit(0);
        	}
        	else if(child_pid > 0)
        	{
        		processIDs[counter] = child_pid;
        	}

        	counter++;
        	}

   		}
    	
    	for(int i = 0; i < 5; i++) wait(NULL);		// Waits for child processes

    	float * x;

    	x = readOutputFileMain(argv[4], counter, processIDs);

    	float minDist;
    	int id1, id2;

    	if(x != NULL)
    	{
    		closestDistanceCalculator(x, processIDs, counter, &id1, &id2, &minDist);
    		printf("Closest 2 matrices are R_%d and R_%d, and their distance is  %f\n", id1, id2, minDist);
    	}
    	
    	close(fdin);	

			
}

	////////////////////////////////////////////////////////////////////////