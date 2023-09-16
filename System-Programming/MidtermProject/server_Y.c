#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include "lib/pipes_fifo_seqnum.h"
#include "lib/error_functions.c"
#include "lib/become_daemon.h"

int pfd[1024][2];
int availableWorkers [1024];
int workersID [1024];
int signalIDs [1024];

int pfd_Z [2];

int lastPointer = 0;
int totalCLients = 0;
int forwardedCLients = 0;

int childSentSignal = 0;
int currentPointer = 0;
int fd_write;

FILE * stream;
FILE * stream2;

int pZID;

struct flock lock;

int pID;
int flag = 0;
int totalInvertible = 0;

int childReadSignal(int sig, siginfo_t * info, void * context)
{
	int saved = errno;	
	signalIDs[currentPointer++] = info -> si_pid;	
	childSentSignal = 1;
	errno = saved;
}

void sigintHandler()
{
	if(getpid() == pID) flag = 1; 
	else flag = 2; 
}

void sigintHappened()
{	
	if(flag == 1)
	{
		
	}
	else if(flag == 2)
	{

	}
}

void fixAvailabilities(int numberOfChildProcesses)
{
	for(int i = 0 ; i < currentPointer; i++)
	{
		for(int j = 0; j < numberOfChildProcesses; j++)
		{
			if(workersID[j] == signalIDs[i])
			{
				availableWorkers[j] = 1;
				break;
			}	
		}
	}


	childSentSignal = 0;
	currentPointer = 0;
}

		/*			CHILD PART			*/

void getCofactor(int ** mat, int ** temp, int p, int q, int n)                
{
    int i = 0, j = 0;
 
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = mat[row][col];

                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}
 
int determinantOfMatrix(int ** mat , int n)
{
    int D = 0;
 
    if (n == 1)
        return mat[0][0];
 
    int** temp = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        temp[i] = (int*)malloc(n * sizeof(int));
 
    int sign = 1;
 
    for (int f = 0; f < n; f++)
    {
        getCofactor(mat, temp, 0, f, n);
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1);           
        sign = -sign;
    }
 
    return D;
}

int checkInvertible(int ** mat, int n)
{
	int det = determinantOfMatrix(mat, n);
	if(det == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void displayMatrix(int ** mat, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            printf("  %d", mat[i][j]);
        printf("\n");
    }
}

int * pickNumbersFromString(char * str , int * lenPtr, int * clientID)
{
	int counterComma = 0, counterArr = 0;
	int strLen = strlen(str);
	char tempStr[1000];
	int * arr = (int*) malloc(1000 * sizeof(int));

	for(int i = 0; i < strLen; i++)
	{
		char temp2[50] = {'\0'};

		if(i == strLen - 1)
		{
			for(int j = 0; j < counterComma; j++)	temp2[j] = tempStr[j];
			*clientID = atoi(temp2);			
		}
		else if(str[i] == ',')
		{
			int num;
			char x[50] = {'\0'};
			char temp2[50] = {'\0'};

			for(int j = 0; j < counterComma; j++)	temp2[j] = tempStr[j];

			if(tempStr[0] == '-')
			{
				for(int j = 0; j < strlen(temp2) - 1; j++)
				{
					x[j] = temp2[j + 1];
					num = atoi(x) * (-1);
				}
			}
			else
			{
				num = atoi(temp2);
			}
			arr[counterArr] = num;
			counterArr++;
			counterComma = 0;
		}
		else
		{
			tempStr[counterComma] = str[i];
			counterComma++;
		}
	}

	(* lenPtr) = sqrt(counterArr);
	return arr;
}

int becomeDaemon(int flags) /* Returns 0 on success, -1 on error */
{
	int maxfd, fd;
	switch (fork()) /* Become background process */
	{ 
		case -1: return -1;
		case 0: break; /* Child falls through; adopted by init */
		default: _exit(EXIT_SUCCESS); /* parent terminates and shell prompt is back*/
	}

	if (setsid() == -1) /* Become leader of new session, dissociate from tty */
		return -1; /* can still acquire a controlling terminal */

	switch (fork())  /* Ensure we are not session leader */
	{
		case -1: return -1; /* thanks to 2nd fork, there is no way of acquiring a tty */
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	}

	if (!(flags & BD_NO_UMASK0))	umask(0); /* Clear file mode creation mask */
	if (!(flags & BD_NO_CHDIR))		chdir("/"); /* Change to root directory */
	if (!(flags & BD_NO_CLOSE_FILES)) 	/* Close all open files */
	{
		maxfd = sysconf(_SC_OPEN_MAX);
		if (maxfd == -1) /* Limit is indeterminate... */
		maxfd = BD_MAX_CLOSE; /* so take a guess */
		for (fd = 0; fd < maxfd; fd++)	close(fd);
	}

	if (!(flags & BD_NO_REOPEN_STD_FDS)) 
	{
		close(STDIN_FILENO); /* Reopen standard fd's to /dev/null */
		fd = open("/dev/null", O_RDWR);
		if (fd != STDIN_FILENO) /* 'fd' should be 0 */
			return -1;
		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	becomeDaemon(31);
	pID = getpid();

	fd_write = open("a.txt", O_CREAT | O_WRONLY, 0666);
	struct flock lock_w;
	memset(&lock_w, 0, sizeof(lock_w));
	lock_w.l_type = F_WRLCK;
	if(fcntl(fd_write, F_SETLK, &lock_w) == -1)
	{
		perror("Another Server Y is Running!");
		exit(0);
	}

	memset(&lock, 0, sizeof(lock));

	if(argc != 11 || strcmp(argv[1], "-s") != 0 || strcmp(argv[3], "-o") != 0 || strcmp(argv[5], "-p") != 0 || strcmp(argv[7], "-r") != 0 || strcmp(argv[9], "-t") != 0)
	{
		//usageErr("%s [seq-len...]\n", argv[0]);
		usageErr("%s Wrong command linme arguments for server_Y\n", argv[0]);
	}
	
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = (void*)&childReadSignal;
	sa.sa_flags = SA_SIGINFO;

	if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGUSR1, &sa, NULL) == -1))
	{
		perror("Failed to install signal sa_handler\n");
		exit(EXIT_FAILURE);
	}

	struct sigaction sa_int;
	memset(&sa_int, 0, sizeof(sa_int));
	sa_int.sa_handler = (void*)&sigintHandler;
	sa_int.sa_flags = SA_SIGINFO;

	if((sigemptyset(&sa_int.sa_mask) == -1) || (sigaction(SIGINT, &sa_int, NULL) == -1))
	{
		perror("Failed to install signal sa_handler\n");
		exit(EXIT_FAILURE);
	}

	int numberOfChildProcesses, numberOfChildProcesses2, delayTime;
	int serverFd, dummyFd, clientFd;
	char clientFifo[CLIENT_FIFO_NAME_LEN];
	struct request req;
	int seqNum = 0;
	int equal = 0;
	int fd_log;
	char inp[1000];

	fd_log = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0666);

	time_t current_time2;
    char* c_time_string2;	
    current_time2 = time(NULL);

    if (current_time2 == ((time_t)-1))
    {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    c_time_string2 = ctime(&current_time2);

    if (c_time_string2 == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }	

    sprintf(inp,"%sServer Y (%s, p=%s, t = %s) started\n", c_time_string2, argv[4], argv[6], argv[10]);

    lseek(fd_log, 0, SEEK_END);
    lock.l_type = F_WRLCK;
	fcntl(fd_log, F_SETLKW, &lock);
	write(fd_log, inp, strlen(inp));
	lock.l_type = F_UNLCK;
	fcntl(fd_log, F_SETLKW, &lock);

	numberOfChildProcesses = atoi(argv[6]);
	numberOfChildProcesses2 = atoi(argv[8]);
	delayTime = atoi(argv[10]);

	for(int i = 0; i < numberOfChildProcesses; i++)	availableWorkers[i] = 1;

	/* CREATE SERVER_Z */

	if(pipe(pfd_Z) == -1)
	{
		errExit("pipe");
	}	

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
    }	

    sprintf(inp,"%sInstantiated server Z\n", c_time_string);

    lseek(fd_log, 0, SEEK_END);
    lock.l_type = F_WRLCK;
	fcntl(fd_log, F_SETLKW, &lock);
	write(fd_log, inp, strlen(inp));
	lock.l_type = F_UNLCK;
	fcntl(fd_log, F_SETLKW, &lock);


	if(fork() == 0)
	{

		char * numberOfChildProcesses_Z = (char *) malloc(50 * sizeof(char));
		char * delayTime_Z = (char *) malloc(50 * sizeof(char));
		char * pfd_Z_str1 = (char *) malloc(50 * sizeof(char));
		char * pfd_Z_str2 = (char *) malloc(50 * sizeof(char));

		
		sprintf(numberOfChildProcesses_Z, "%d", numberOfChildProcesses2);
		sprintf(delayTime_Z, "%d", delayTime);
		sprintf(pfd_Z_str1, "%d", pfd_Z[0]);
		sprintf(pfd_Z_str2, "%d", pfd_Z[1]);

		char * const envVec [50] = {numberOfChildProcesses_Z, pfd_Z_str1, pfd_Z_str2, delayTime_Z, argv[4], argv[2], NULL};
		char * const arguments [50] = {"./server_Z", NULL};

        execve(arguments[0], arguments, envVec);
    }


	char uu [50];
	stream = fdopen(pfd_Z[0], "r");
	fgets(uu, sizeof(uu), stream);
		
	pZID = atoi(uu);	

	
	for(int i = 0; i < numberOfChildProcesses; i++)
	{
		//char temp[100];
		//sprintf(temp, "/sem%d", i);
		//sp[i] = sem_open (temp, O_CREAT | O_RDWR, 0);

		if(pipe(pfd[i]) == -1)
		{
			errExit("pipe");
		}
	
		workersID[i] = fork();
		if(workersID[i] == 0)	
		{
			int invCounter = 0;
			int clientFd;
			int * arr;
			int ** dblArr;
			while(1)
			{				
				if(flag == 2)
				{
					FILE * stream_fin = fdopen(pfd[i][1], "w");
					char tt[50];
					sprintf(tt, "%d", invCounter);
					fprintf(stream_fin, "%s\n", tt);
					fflush(stream_fin);

					free(arr);
					free(dblArr);
					close(clientFd);
					
					exit(1);
				}

				char buffer [100000];
				stream2 = fdopen(pfd[i][0], "r");
				if(fgets(buffer, sizeof(buffer), stream2) == NULL)
				{
					continue;
				}
		
				struct response resp;
				int len, cID;
				arr = pickNumbersFromString(buffer, &len, &cID);

				dblArr = (int**)malloc(len * sizeof(int*));
    			for (int i = 0; i < len; i++)
        			dblArr[i] = (int*)malloc(len * sizeof(int));

   				for(int i = 0; i < len; i++)
    			{
    				for(int j = 0; j < len; j++)
    				{
    					dblArr[i][j] = arr[i * len + j];
    				}
    			}

				time_t current_time2;
    			char* c_time_string2;	
    			current_time2 = time(NULL);

    			if (current_time2 == ((time_t)-1))
    			{
        			(void) fprintf(stderr, "Failure to obtain the current time.\n");
        			exit(EXIT_FAILURE);
    			}

    			c_time_string2 = ctime(&current_time2);

   			 	if (c_time_string2 == NULL)
    			{
        			(void) fprintf(stderr, "Failure to convert the current time.\n");
        			exit(EXIT_FAILURE);
    			}

    			sprintf(inp,"%sWorker PID%d is handling client PID%d , matrix size is %dx%d pool busy %d/%d\n", c_time_string2, getpid(), cID, len, len, i + 1, numberOfChildProcesses);

    			lseek(fd_log, 0, SEEK_END);
    			lock.l_type = F_WRLCK;
				fcntl(fd_log, F_SETLKW, &lock);
				write(fd_log, inp, strlen(inp));
				lock.l_type = F_UNLCK;
				fcntl(fd_log, F_SETLKW, &lock);

				resp.isInvertible = checkInvertible(dblArr, len);

				invCounter += resp.isInvertible;

				time_t seconds;
				time(&seconds);			

				resp.opTime = seconds;

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
    			}

    			if(resp.isInvertible == 1) sprintf(inp,"%sWorker PID%d responding to client PID%d : the matrix IS invertible. \n", c_time_string, getpid(), cID);
    			else sprintf(inp,"%sWorker PID%d responding to client PID%d : the matrix IS NOT invertible. \n", c_time_string, getpid(), cID); 

    			lseek(fd_log, 0, SEEK_END);
    			lock.l_type = F_WRLCK;
				fcntl(fd_log, F_SETLKW, &lock);
				write(fd_log, inp, strlen(inp));
				lock.l_type = F_UNLCK;
				fcntl(fd_log, F_SETLKW, &lock);
			
				snprintf(clientFifo, (sizeof(argv[2]) + 20), argv[2], (long) cID);
				clientFd = open(clientFifo, O_WRONLY);

				if(clientFd == -1)
				{
					errMsg("open %s", clientFifo);
					continue;
				}

				if(write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
				{
					fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
				}

				if(close(clientFd) == -1)
				{
					errMsg("close");
				}
				if(sleep(delayTime) > 0)
				{
					continue;
				}
				kill(getppid(), SIGUSR1);
			}
						
		}
			

	}

	umask(0);

	

	if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST)
	{
		errExit("mkfifo %s", SERVER_FIFO);
	}
	
	serverFd = open(SERVER_FIFO, O_RDONLY);

	if(flag == 0)
	{

	if(serverFd == -1)
	{
		errExit("open %s", SERVER_FIFO);
	}

	dummyFd = open(SERVER_FIFO, O_WRONLY);
	
	if(dummyFd == -1)
	{
		errExit("open %s", SERVER_FIFO);
	}

	/*
	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		errExit("signal");
	}		*/
	}

	for(;;)
	{
		if(flag == 1)
		{		

			kill(pZID, SIGINT);

			for(int i = 0; i < numberOfChildProcesses; i++)
			{
				kill(workersID[i], SIGINT);
			}

			for(int i = 0; i < numberOfChildProcesses; i++)
			{
				char buffer [100000];
				FILE * stream_fin = fdopen(pfd[i][0], "r");
				fgets(buffer, sizeof(buffer), stream_fin);
				totalInvertible += atoi(buffer);
			}

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

    	sprintf(inp,"%sSIGINT received, terminating Z and exiting server Y. Total requests handled: %d, %d invertible, %d not. %d requests were forwarded.\n", 
    		c_time_string3, totalCLients, totalInvertible, totalCLients - totalInvertible, forwardedCLients);

    	lseek(fd_log, 0, SEEK_END);
    	lock.l_type = F_WRLCK;
		fcntl(fd_log, F_SETLKW, &lock);
		write(fd_log, inp, strlen(inp));
		lock.l_type = F_UNLCK;
		fcntl(fd_log, F_SETLKW, &lock);

		close(serverFd);
		close(pfd_Z[0]);
		close(pfd_Z[1]);

		for(int i = 0; i < numberOfChildProcesses; i++)
		{
			close(pfd[i][0]);
			close(pfd[i][1]);
		}

			exit(1);
		}

		if(childSentSignal == 1) fixAvailabilities(numberOfChildProcesses);

		if(read(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
		{
			//fprintf(stderr, "Error reading request; discarding\n");
			continue;
		}
		int done = 0;
		for(int i = 0; i < numberOfChildProcesses; i++)
		{
			if(availableWorkers[i] == 1)
			{
				totalCLients++;

				char tempStr[100000];
				tempStr[0] = '\0';

				for(int i = 0; i < req.matrixLength * req.matrixLength; i++)
				{
					sprintf(tempStr, "%s%d,", tempStr, req.matrix[i]);
				}

				sprintf(tempStr, "%s%d", tempStr,req.pid);

				availableWorkers[i] = 0;
				stream = fdopen(pfd[i][1], "w");
				fprintf(stream, "%s\n", tempStr);
				fflush(stream);

				done = 1;
				//close(pfd[i][1]);
				break;	
			}
		}	
		
		if(done == 0)
		{
			forwardedCLients++;
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

    	sprintf(inp,"%sForwarding request of client PID#%d to serverZ, matrix size %dx%d , pool busy %d/%d\n", c_time_string3, req.pid, req.matrixLength, req.matrixLength, numberOfChildProcesses, numberOfChildProcesses);

    	lseek(fd_log, 0, SEEK_END);
    	lock.l_type = F_WRLCK;
		fcntl(fd_log, F_SETLKW, &lock);
		write(fd_log, inp, strlen(inp));
		lock.l_type = F_UNLCK;
		fcntl(fd_log, F_SETLKW, &lock);

		char tempStr[100000];
		tempStr[0] = '\0';
		for(int i = 0; i < req.matrixLength * req.matrixLength; i++)
		{
			sprintf(tempStr, "%s%d,", tempStr, req.matrix[i]);
		}
		sprintf(tempStr, "%s%d", tempStr, req.pid);

		stream = fdopen(pfd_Z[1], "w");
		fprintf(stream, "%s\n", tempStr);
		fflush(stream);

	}

	
	}
	
}