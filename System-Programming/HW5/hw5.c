#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define PI 3.14159265359

int SIGINToccured = 0;
int SIGUSR1occured = 0;

int M, N, colonSize, matrixSideSize;
int ** matrixA;
int ** matrixB;
int ** matrixC;
float *** matrixResult;

pthread_t * threads;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int firstPartFinished = 0;

void handlerSIGINT()
{
	for(int i = 0;i < M;i++)
		pthread_kill(threads[i],SIGUSR1);
	SIGINToccured = 1;
}

void handlerSIGUSR1()
{
	SIGUSR1occured = 1;
}

void timeStamp(char* c_time_string)
{
	time_t current_time;
	current_time = time(NULL);

	if(current_time == ((time_t)-1))
	{
		fprintf(stderr, "Failure to obtain the current time\n");
		exit(EXIT_FAILURE);
	}

	strcpy(c_time_string,ctime(&current_time));

	if(c_time_string == NULL)
	{
		fprintf(stderr, "Failure to convert the current time\n");
		exit(EXIT_FAILURE);	
	}

	c_time_string[strlen(c_time_string) - 1] = '\0';

}

int isPowerOfTwo(int n)
{
  if (n == 0)
    return 0;
  while (n != 1)
  {
      if (n % 2 != 0)
         return 0;
      n = n/2;
  }
  return 1;
}

void checkInputs()
{
	int x = isPowerOfTwo(M);

	if(M > matrixSideSize) M = matrixSideSize;
				
	else if(x == 0)
	{
		int power = 1;
		while(power < M)
		{
			power *= 2;
		}
		M = power / 2;
	}
}

void readFile(char * filePath, int ** matrix, int n)
{
	unsigned char buffer[1];
	size_t bytes_read;
	int counter = 0;

	int fd = open(filePath, O_RDONLY);

	if(fd == -1)
	{
		perror("Failed to open file for reading!");
		exit(EXIT_FAILURE);;
	}

	while(counter < n * n)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));

		if(bytes_read != sizeof(buffer))
		{
			perror("Input file error!");
			exit(EXIT_FAILURE);
		}

		matrix[counter / n][counter % n] = buffer[0];
		counter++;
	}

	if(close(fd) == -1)
	{
		perror("Failed to close file for writing!");
		exit(EXIT_FAILURE);
	}
}

void writeFile(float*** Result, const char * filename)
{
	int fdWrite = open(filename,O_CREAT | O_WRONLY | O_TRUNC | O_APPEND, 0666);
	if(fdWrite == -1)
	{
		perror("Failed to open file for writing!");
		exit(EXIT_FAILURE);
	}

	char fileTemp[100];

	for(int i = 0;i < matrixSideSize;i++)
	{
		for(int j = 0;j < matrixSideSize;j++)
		{
			if(j != matrixSideSize - 1) sprintf(fileTemp,"%f + j(%f),",Result[i][j][0], Result[i][j][1]);
			else sprintf(fileTemp,"%f + j(%f)",Result[i][j][0], Result[i][j][1]); 
			write(fdWrite, fileTemp, strlen(fileTemp));
		}
		write(fdWrite, "\n", 1);
	}

	if(close(fdWrite) == -1)
	{
		perror("Failed to close file for writing!");
		exit(EXIT_FAILURE);
	}
}

void multiplyMatrices(int ** matrixA, int ** matrixB, int threadNum, int m, int n)
{
	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			int temp = 0;

			for(int k = 0; k < n; k++)
			{
				temp += matrixA[j][k] * matrixB[k][threadNum * m + i];
			}
			matrixC[j][threadNum * m + i] = temp;
			temp = 0;
		}
	}
}

void * threadFunction(void * var)
{
	clock_t startTime, endTime;
	char c_time_string[100];
	startTime = clock();

	multiplyMatrices(matrixA, matrixB, *((int*)var), colonSize, matrixSideSize);

	if(SIGUSR1occured) pthread_exit(NULL);
		
	pthread_mutex_lock(&mutex);
	endTime = clock();
	timeStamp(c_time_string);
	fprintf(stdout,"%s | Thread %d has reached the rendezvous point in %.4f seconds.\n",c_time_string,*(int*)var,((double)(endTime-startTime))/CLOCKS_PER_SEC);

	if(++firstPartFinished == M) pthread_cond_broadcast(&cond);		
	else pthread_cond_wait(&cond, &mutex);	
		
	pthread_mutex_unlock(&mutex);
	timeStamp(c_time_string);
	fprintf(stdout,"%s | Thread %d is advancing to the second part.\n",c_time_string,*(int*)var);

	if(SIGUSR1occured) pthread_exit(NULL);
		
	float*** myMatrix;
	myMatrix = (float***)malloc(sizeof(float**) * matrixSideSize);
	for(int i = 0;i < matrixSideSize;i++){
		myMatrix[i] = (float**)malloc(sizeof(float*) * matrixSideSize);
		for(int j = 0;j < matrixSideSize / M;j++){
			myMatrix[i][j] = (float*)malloc(sizeof(float) * 2);
			myMatrix[i][j][0] = 0.0;
			myMatrix[i][j][1] = 0.0;
		}
	}
        
	for(int i = 0; i < matrixSideSize / M; i++)
	{	
		for(int j = 0; j < matrixSideSize; j++)
		{
			float tempRe = 0; 
  			float tempIm = 0;

			for(int k = 0; k < matrixSideSize; k++)
			{
				for(int l = 0; l < matrixSideSize; l++)
				{
					float y = -2.0 * PI * (*((int*)var) * colonSize + i) * k / (float) matrixSideSize;
					float x = -2.0 * PI * j * l / (float) matrixSideSize;

					tempRe += (float)matrixC[l][k] * cos(x + y);
					tempIm += (float)matrixC[l][k] * 1.0 * sin(x + y);			
				}
			}

			myMatrix[j][i][0] = tempRe;
 			myMatrix[j][i][1] = tempIm;

		}

	}	

	startTime = clock();
	endTime = clock();
	timeStamp(c_time_string);
	fprintf(stdout,"%s | Thread %d has finished the second part in %.4f seconds.\n",c_time_string,*(int*)var,((double)(endTime - startTime)) / CLOCKS_PER_SEC);

	float* test = (float*)malloc(sizeof(float) * matrixSideSize * matrixSideSize * 4 / M);

	for (int i = 0; i < matrixSideSize; ++i)
	{
		for (int j = 0; j < colonSize; j++)
		{	
			test[2 * (i * colonSize + j) + 0] = myMatrix[i][j][0];
			test[2 * (i * colonSize + j) + 1] = myMatrix[i][j][1];
		}
	}

	void* myPart;

	for(int i = 0;i < matrixSideSize;i++)
	{
		for(int j = 0;j < matrixSideSize / M;j++) free(myMatrix[i][j]);			
		free(myMatrix[i]);
	}

	free(myMatrix);
	myPart = test;
	pthread_exit(myPart);
}

int main(int argc, char *argv[])
{
	if(argc != 11) 
	{
		perror("Wrong Amount of Inputs!");
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1],"-i") != 0 || strcmp(argv[3],"-j") != 0 || strcmp(argv[5],"-o") != 0 || strcmp(argv[7],"-n") != 0 || 
	   atoi(argv[8]) <= 1 || strcmp(argv[9], "-m") != 0)
	{
		perror("Wrong Inputs!");
		exit(EXIT_FAILURE);	
	}

	struct sigaction sas;
	memset(&sas, 0, sizeof(sas));
	sas.sa_handler = &handlerSIGUSR1;
	sas.sa_flags = 0;

	if((sigemptyset(&sas.sa_mask) == -1) || (sigaction(SIGUSR1, &sas, NULL) == -1))
	{
		perror("Failed to install signal sa_handler");
		exit(EXIT_FAILURE);
	}

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &handlerSIGINT;
	sa.sa_flags = 0;

	if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
	{
		perror("Failed to install signal sa_handler");
		exit(EXIT_FAILURE);
	}

	N = atoi(argv[8]);
	M = atoi(argv[10]);
	matrixSideSize = pow(2, N);
	checkInputs();
	colonSize = matrixSideSize / M;

	matrixA = (int **) malloc(sizeof(int *) * matrixSideSize);
	matrixB = (int **) malloc(sizeof(int *) * matrixSideSize);
	matrixC = (int **) malloc(sizeof(int *) * matrixSideSize);

	for(int i = 0;i < matrixSideSize;i++)
	{
		matrixA[i] = (int *) malloc(sizeof(int) * matrixSideSize);
		matrixB[i] = (int *) malloc(sizeof(int) * matrixSideSize);
		matrixC[i] = (int *) malloc(sizeof(int) * matrixSideSize);
	}

	matrixResult = (float***)malloc(sizeof(float**) * matrixSideSize);
	for(int i = 0;i < matrixSideSize;i++)
	{
		matrixResult[i] = (float**)malloc(sizeof(float*) * matrixSideSize);
		for(int j = 0;j < matrixSideSize;j++){
			matrixResult[i][j] = (float*)malloc(sizeof(float) * 2);
			matrixResult[i][j][0] = 0.0;
			matrixResult[i][j][1] = 0.0;
		}
	}

	char timeChar[100];
	clock_t startTime, endTime;

	readFile(argv[2], matrixA, matrixSideSize);
	readFile(argv[4], matrixB, matrixSideSize);

	timeStamp(timeChar);

	fprintf(stdout,"%s | Two matrices of size %dx%d have been read. The number of threads is %d.\n",timeChar, matrixSideSize, matrixSideSize, M);
	startTime = clock();

	threads = (pthread_t*) malloc(sizeof(pthread_t) * M);

	int Num[M];
	for(int i = 0 ;i < M;i++){
		Num[i] = i;
		if(pthread_create(&threads[i], NULL, threadFunction, (void *) &Num[i]) != 0)
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	void* res[M];
	for(int i = 0 ;i < M; i++)
	{
		if(pthread_join(threads[i], &res[i]) != 0)
		{
			perror("pthread_join");
			exit(EXIT_FAILURE);	
		}
	}

	if(SIGINToccured == 0)
	{
		for(int i = 0; i < M; i++)
		{
			float* resTemp = (float*)res[i];

			for(int j = 0; j < colonSize * 2 * matrixSideSize; j += 2)
			{
				matrixResult[j / 2 / colonSize][(j / 2) % colonSize + i * colonSize][0] = resTemp[j];
				matrixResult[j / 2 / colonSize][(j / 2) % colonSize + i * colonSize][1] = resTemp[j + 1];			
			}
		}

		writeFile(matrixResult, argv[6]);
		endTime = clock();
		timeStamp(timeChar);
		fprintf(stdout,"%s | The process has written the output file. The total time spent is %.4f seconds.\n",timeChar,((double)(endTime-startTime))/CLOCKS_PER_SEC);
	}
	
	for(int i = 0; i < M;i++) free(res[i]);
		
	for(int i = 0; i < matrixSideSize; i++)
	{
		free(matrixA[i]);
		free(matrixB[i]);
		free(matrixC[i]);
	}

	for(int i=0;i<matrixSideSize;i++)
    {
        for(int j=0;j<matrixSideSize;j++)
        {
            free(matrixResult[i][j]);
        }
        free(matrixResult[i]);
    }
    free(matrixResult);

    free(threads);
	free(matrixA);
	free(matrixB);
	free(matrixC);
	
}