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
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "lib/pipes_fifo_seqnum.h"
#include "lib/error_functions.c"
#include "lib/become_daemon.h"

int pfd_Z [2];
FILE * stream;
struct flock lock;

int totalCLients = 0;
int flag = 0;
int pID;
int fd_log;
int childSentSignal = 0;
int workersID [1024];
extern char **environ;

void sigintHandler()
{
	if(getpid() == pID) flag = 1; 
	else flag = 2; 
}

int childReadSignal(int sig, siginfo_t * info, void * context)
{
	int saved = errno;		
	childSentSignal = 1;
	errno = saved;
}

int getPID(char * str)
{
	int counterComma = 0, counterArr = 0;
	int strLen = strlen(str);
	char tempStr[1000];
	int * arr = (int*) malloc(1000 * sizeof(int));
	int res;

	for(int i = 0; i < strLen; i++)
	{

		if(i == strLen - 1)
		{
			int num;
			char x[50] = {'\0'};
			char temp2[50] = {'\0'};

			for(int j = 0; j < counterComma; j++)	temp2[j] = tempStr[j];
			
			res = atoi(temp2);
			
			arr[counterArr] = num;
			counterArr++;
			counterComma = 0;
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

	return res;
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

	int fd, fd_sem, fd_counter, fd_cID, fd_resp;
	int sleepTime, numberOfChilds;
	char fifoName[50];

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = (void*) &sigintHandler;
	sa.sa_flags = SA_SIGINFO;

	if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
	{
		perror("Failed to install signal sa_handler\n");
		exit(EXIT_FAILURE);
	}

	
	memset(&lock, 0, sizeof(lock));

	fd_log = open(environ[4], O_WRONLY, 0666);

	pfd_Z[0] = atoi(environ[1]);
	pfd_Z[1] = atoi(environ[2]);

	char gg[50];
	sprintf(gg, "%d", pID);	
	FILE * yt = fdopen(pfd_Z[1], "w");
	fprintf(yt, "%s\n", gg);
	fflush(yt);
	

	numberOfChilds = atoi(environ[0]);
	sleepTime = atoi(environ[3]);
	strcpy(fifoName, environ[5]);

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

    char inp[10000];
    sprintf(inp,"%sZ: Server Z(%s, t = %d, r = %d) started\n", c_time_string3, environ[4], sleepTime, numberOfChilds);

    lseek(fd_log, 0, SEEK_END);
    lock.l_type = F_WRLCK;
	fcntl(fd_log, F_SETLKW, &lock);
	write(fd_log, inp, strlen(inp));
	lock.l_type = F_UNLCK;
	fcntl(fd_log, F_SETLKW, &lock);

	for(int i = 0; i < numberOfChilds; i++)
	{
		workersID[i] = fork();
		if(workersID[i] == 0)
		{
			fd_cID = shm_open("/server_CID", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd_cID == -1) errExit("shm_open");
			if (ftruncate(fd_cID, sizeof(int)) == -1) errExit("ftruncate");
			int * c_ID = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_cID, 0);

			fd_counter = shm_open("/server_z_ctr", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd_counter == -1) errExit("shm_open");
			if (ftruncate(fd_counter, sizeof(int) * 100000) == -1) errExit("ftruncate");
			int * invOps = mmap(NULL, sizeof(int) * numberOfChilds, PROT_READ | PROT_WRITE, MAP_SHARED, fd_counter, 0);

			fd_sem = shm_open("/server_z_sem", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd_sem == -1) errExit("shm_open");
			if (ftruncate(fd_sem, sizeof(sem_t)) == -1) errExit("ftruncate");
			sem_t * sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd_sem, 0);
			sem_init(sem, 1, 0);

			fd = shm_open("/server_CID", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			if (fd == -1) errExit("shm_open");
			if (ftruncate(fd, sizeof(char) * 100000) == -1) errExit("ftruncate");
			char * t = mmap(NULL, sizeof(char) * 100000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

			int clientFd;
			int * arr;
			int ** dblArr;

			while(1)
			{			

				if(flag == 2)
				{

					free(arr);
					free(dblArr);

					sem_close(sem);
					close(fd);
					close(fd_sem);
					close(fd_cID);
					close(fd_counter);			
					close(clientFd);
					exit(0);
				}

				if(sem_wait(sem) == -1)
				{
					continue;
				}

				struct response resp;
				int len, cID;
				arr = pickNumbersFromString(t, &len, &cID);
				
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
    			char inp[1000];
    			sprintf(inp,"%sZ: Worker PID%d is handling client PID%d , matrix size is %dx%d pool busy %d/%d\n", c_time_string2, getpid(), cID, len, len, i + 1, numberOfChilds);

    			lseek(fd_log, 0, SEEK_END);
    			lock.l_type = F_WRLCK;
				fcntl(fd_log, F_SETLKW, &lock);
				write(fd_log, inp, strlen(inp));
				lock.l_type = F_UNLCK;
				fcntl(fd_log, F_SETLKW, &lock);

				resp.isInvertible = checkInvertible(dblArr, len);

				(*invOps)++;		
				
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
    			else sprintf(inp,"%sZ: Worker PID%d responding to client PID%d : the matrix IS NOT invertible. \n", c_time_string, getpid(), cID); 

    			lseek(fd_log, 0, SEEK_END);
    			lock.l_type = F_WRLCK;
				fcntl(fd_log, F_SETLKW, &lock);
				write(fd_log, inp, strlen(inp));
				lock.l_type = F_UNLCK;
				fcntl(fd_log, F_SETLKW, &lock);

				time_t seconds;
				time(&seconds);
				resp.opTime = seconds;

				char clientFifo[CLIENT_FIFO_NAME_LEN];
				snprintf(clientFifo, (sizeof(fifoName) + 20), fifoName, *c_ID);

				clientFd = open(clientFifo, O_WRONLY);

				if(clientFd == -1)
				{		
					errMsg("open22 %s", clientFifo);
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

				if(sleep(sleepTime) > 0)
				{
					continue;
				}

			}
		}
	}

	fd_cID = shm_open("/server_CID", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_cID == -1) errExit("shm_open");
	if (ftruncate(fd_cID, sizeof(int)) == -1) errExit("ftruncate");
	int * c_ID = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_cID, 0);

	fd = shm_open("/server_CID", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1) errExit("shm_open");
	if (ftruncate(fd, sizeof(char) * 100000) == -1) errExit("ftruncate");
	char * t = mmap(NULL, sizeof(char) * 100000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	fd_counter = shm_open("/server_z_ctr", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_counter == -1) errExit("shm_open");
	if (ftruncate(fd_counter, sizeof(int)== -1)) errExit("ftruncate");
	int * invOps = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_counter, 0);

	fd_sem = shm_open("/server_z_sem", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_sem == -1) errExit("shm_open");
	if (ftruncate(fd_sem, sizeof(sem_t)) == -1) errExit("ftruncate");
	sem_t * sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd_sem, 0);
	sem_init(sem, 1, 0);

	if(close(fd) == -1)	errExit("close");
	if(close(fd_sem) == -1)	errExit("close");
	
	while(1)
	{
		if(flag == 1)
		{

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

    	sprintf(inp,"%sZ: SIGINT received, exiting server Z. Total requests handled: %d, %d invertible, %d not.\n", 
    		c_time_string3, totalCLients, (*invOps), totalCLients - (*invOps));

    	lseek(fd_log, 0, SEEK_END);
    	lock.l_type = F_WRLCK;
		fcntl(fd_log, F_SETLKW, &lock);
		write(fd_log, inp, strlen(inp));
		lock.l_type = F_UNLCK;
		fcntl(fd_log, F_SETLKW, &lock);

			for(int i = 0; i < numberOfChilds; i++)
			{
				kill(workersID[i], SIGINT);
			}

			sem_close(sem);
			close(fd);
			close(fd_sem);
			close(fd_cID);
			close(fd_counter);			

			close(pfd_Z[0]);
			close(pfd_Z[1]);

			exit(1);
		}

		char buffer [100000];
		stream = fdopen(pfd_Z[0], "r");
		if(fgets(buffer, sizeof(buffer), stream) == NULL) 
		{
			continue;
		}

			strcpy(t, buffer);
			totalCLients++;
			sem_post(sem);	
		
		
	}
	
		
}