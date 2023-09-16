#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/time.h>
#include "lib/pipes_fifo_seqnum.h"
#include "lib/error_functions.c"
#include "lib/get_num.c"

static char clientFifo[CLIENT_FIFO_NAME_LEN];
int flag = 0;

void sigintHandler()
{
	flag = 1;
}

int * readInputFile(char * filePath, int * lenPtr)
{
	int fd;
	int counterComma = 0, counterTotalNum = 0, counterLen = 0, arrLen = 0;
	char buffer[1];
	int * arr = (int *) malloc(10000 * sizeof(int));
	char temp1[50];
	size_t bytes_read = sizeof(buffer);

	fd = open(filePath, O_RDONLY);

	if(fd == -1)
	{
		perror("Input file couldn't be opened!");
		exit(0);
	}

	while(1)
	{		
		bytes_read = read(fd, buffer, sizeof(buffer));

		if(bytes_read != sizeof(buffer)) 	
		{
			char temp2[50] = {'\0'};

			for(int i = 0; i < counterComma; i++)
			{
				temp2[i] = temp1[i];
			}

			char x[50] = {'\0'};
			int num;

			if(temp2[0] == '-')
			{
				for(int i = 0; i < strlen(temp2) - 1; i++)
				{
					x[i] = temp2[i + 1];
					num = atoi(x) * (-1);
				}
			}
			else
			{
				num = atoi(temp2);
			}	 

			arr[counterLen] = num;

			counterComma = 0;
			counterLen++;
			if((*lenPtr) == 0) (*lenPtr) = counterLen;

			break;
		}

		if(buffer[0] == ',')
		{
			char temp2[50] = {'\0'};	

			for(int i = 0; i < counterComma; i++)
			{
				temp2[i] = temp1[i];
			}

			char x[50] = {'\0'};
			int num;

			if(temp2[0] == '-')
			{
				for(int i = 0; i < strlen(temp2) - 1; i++)
				{
					x[i] = temp2[i + 1];
					num = atoi(x) * (-1);
				}
			}
			else
			{
				num = atoi(temp2);
			}	

			arr[counterLen] = num;

			counterComma = 0;
			counterLen++;

		}
		else if(buffer[0] == '\n')
		{
			char temp2[50] = {'\0'};

			for(int i = 0; i < counterComma; i++)
			{
				temp2[i] = temp1[i];
			}

			char x[50] = {'\0'};
			int num;

			if(temp2[0] == '-')
			{
				for(int i = 0; i < strlen(temp2) - 1; i++)
				{
					x[i] = temp2[i + 1];
					num = atoi(x) * (-1);
				}
			}
			else
			{
				num = atoi(temp2);
			}

			arr[counterLen] = num;

			counterComma = 0;
			counterLen++;
			if((*lenPtr) == 0) (*lenPtr) = counterLen;
		}
		else
		{
			counterComma++;
			temp1[counterComma - 1] = buffer[0];			
		}

	}

	close(fd);
	return arr;

}

static void removeFifo(void)
{
	unlink(clientFifo);
}

int main(int argc, char* argv[])
{
	struct sigaction sa_int;
	memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = (void*)&sigintHandler;
	sa_int.sa_flags = SA_SIGINFO;

	if((sigemptyset(&sa_int.sa_mask) == -1) || (sigaction(SIGINT, &sa_int, NULL) == -1))
	{
		perror("Failed to install signal sa_handler\n");
		exit(EXIT_FAILURE);
	}

	int serverFd, clientFd;
	struct request req;
	struct response resp;
	float startTime;

	time_t seconds;
	time(&seconds);

	if(argc != 5 || strcmp(argv[1], "-s") != 0 || strcmp(argv[3], "-o") != 0)
	{
		//usageErr("%s [seq-len...]\n", argv[0]);
		usageErr("%s Wrong command linme arguments for client\n", argv[0]);
	}

	int len = 0;
	int * bb;

	bb = readInputFile(argv[4], &len);

			time_t current_time3;
    		char* c_time_string3;	
    		current_time3 = time(NULL);

    		if (current_time3 == ((time_t)-1))
    		{
       			(void) fprintf(stderr, "Failure to obtain the current time.\n");
        		exit(EXIT_FAILURE);
    		}

   			c_time_string3 = ctime(&current_time3);

    		if (c_time_string3 == NULL)
    		{
        		(void) fprintf(stderr, "Failure to convert the current time.\n");
       			exit(EXIT_FAILURE);
    		}	

    		char hg[100];
    		sprintf(hg, "%sClient PID#%d, (%s) is submitting a %d x %d matrix.\n", c_time_string3, getpid(), argv[4], len, len);
    		fputs(hg, stdout);


	//printf("Client PID#%d (%s) is submitting a %dx%d matrix", getpid(), argv[0], len, len);

	/*
	time_t current_time;
    char* c_time_string;	
    current_time = time(NULL);

    if (current_time == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    c_time_string = ctime(&current_time);

    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }	*/

	umask(0);
	snprintf(clientFifo, (sizeof(argv[2]) + 20), argv[2], (long) getpid());

	if(mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
	{
		errExit("mkfifo %s", clientFifo);
	}

	if(atexit(removeFifo) != 0)
	{
		errExit("atexit");
	}

	/* MANIPULATES MATRIX*/

	req.matrixLength = len;
	for(int i = 0; i < len * len; i++)	req.matrix[i] = bb[i];

	

	req.pid = getpid();
	//req.seqLen = (argc > 1) ? getInt(argv[1], GN_GT_0, "seq-len") : 1;
	serverFd = open(SERVER_FIFO, O_WRONLY);

	if(serverFd == -1)
	{
		errExit("atexit");
	}

	if(write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
	{
		fatal("Can't write to server");
	}

	clientFd = open(clientFifo, O_RDONLY);

	if(clientFd == -1)
	{
		errExit("open %s", clientFifo);
	}

	if(read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
	{
		if(flag == 1)
		{
			close(serverFd);
			close(clientFd);
			exit(0);
		}
		fatal("Can't read response from server");

	}

	float diff = difftime(resp.opTime, seconds);

	if(resp.isInvertible == 1) printf("Client PID%d: the matrix is invertible, total time %f seconds, goodbye.", getpid(), diff);
	else printf("Client PID%d: the matrix is not invertible, total time %f seconds, goodbye.\n", getpid(), diff);

	exit(EXIT_SUCCESS);

}