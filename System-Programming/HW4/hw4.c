#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>

////////////////////////////////////////////////////////////////////////////////

void * consumer_func (void * var);
void SIGINT_handler();
void SIGUSR1_handler();
void * supplier_func (void * var);

///////////////////////////////////////////////////////////////////////////////

union semun
{
	int val;
	struct semid_ds * buf;
	unsigned short * arr;
	struct seminfo * _buf;
} sem_attr;

pthread_t * consumer_threads;
char file_path[50];
int semaphores;
int C, N;
int got_SIGINT = 0;
int got_SIGUSR1 = 0;

///////////////////////////////////////////////////////////////////////////////

void SIGINT_handler()
{
	for(int i = 0;i < C; i++) pthread_kill(consumer_threads[i], SIGUSR1);		
	got_SIGINT = 1;
}

void SIGUSR1_handler()
{
	got_SIGUSR1 = 1;
}

void * consumer_func (void * var)
{
	struct sembuf buf[2];

	buf[0].sem_num = 0;
	buf[0].sem_op = -1;
	buf[0].sem_flg = 0;

	buf[1].sem_num = 1;
	buf[1].sem_op = -1;
	buf[1].sem_flg = 0;
	
	for(int i = 0;i < N; i++)
	{
		if(got_SIGUSR1) break;
			
		time_t current_time;
    	char* c_time_string;	
   		current_time = time(NULL);

    	if (current_time == ((time_t) - 1))
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

		printf("%s Consumer-%d at iteration %d (waiting). Current amounts: %d x '1' , %d x '2'.\n",c_time_string,*((int*)var),i,semctl(semaphores, 0, GETVAL), semctl(semaphores, 1, GETVAL));
		if(semop(semaphores, buf, 2) != 0) continue;			
		
		time_t current_time2;
    	char* c_time_string2;	
   		current_time2 = time(NULL);

    	if (current_time2 == ((time_t) - 1))
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

		printf("[%s] Consumer-%d at iteration %d (consumer). Post-consumption amounts: %d x '1' , %d x '2'.\n",c_time_string2,*((int*)var),i,semctl(semaphores, 0, GETVAL), semctl(semaphores, 1, GETVAL));
	}
	
	time_t current_time3;
    char* c_time_string3;	
   	current_time3 = time(NULL);

    if (current_time3 == ((time_t) - 1))
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

	printf("[%s] Consumer-%d has left.\n",c_time_string3,*((int*)var));
	pthread_exit(NULL);
}

void * supplier_func (void * var)
{
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_op = 1;
	buf.sem_flg = 0;

	int fd = open(file_path, O_RDONLY);

	if(fd == -1)
	{
		perror("Error while opening input");
		exit(EXIT_FAILURE);
	}

	struct flock lock;
	memset(&lock, 0, sizeof(lock));
	lock.l_type = F_RDLCK;
	fcntl(fd, F_SETLK, &lock);

	unsigned char buffer[1];
	char c_time_string[100];
	size_t bytes_read;

	while(1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));
		if(bytes_read != sizeof(buffer) || got_SIGUSR1) break;
			
		if(buffer[0] == '1') buf.sem_num = 0;		
		else if(buffer[0] == '2') buf.sem_num = 1;		
		else continue;
			
		time_t current_time;
    	char* c_time_string;	
   		current_time = time(NULL);

    	if (current_time == ((time_t) - 1))
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

		printf("[%s] Supplier: read from input a %c. Current amounts: %d x '1' , %d x '2'.\n",c_time_string,buffer[0],semctl(semaphores, 0, GETVAL), semctl(semaphores, 1, GETVAL));
		semop(semaphores, &buf, 1);
		
		time_t current_time2;
    	char* c_time_string2;	
   		current_time2 = time(NULL);

    	if (current_time2 == ((time_t) - 1))
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

		printf("[%s] Supplier: delivered a %c. Post-delivery amounts: %d x '1' , %d x '2'.\n",c_time_string,buffer[0],semctl(semaphores, 0, GETVAL), semctl(semaphores, 1, GETVAL));
	}

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);

	if(close(fd) == -1) perror("Error occured while trying to close file after reading!");
		
	time_t current_time3;
    char* c_time_string3;	
   	current_time3 = time(NULL);

    if (current_time3 == ((time_t) - 1))
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

	printf("[%s] The Supplier has left.\n",c_time_string);
	pthread_exit(NULL);
}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);

	if(argc != 7) 
	{
		perror("Wrong number of inputs!\n");
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1],"-C") != 0 || atoi(argv[2]) <= 4 || strcmp(argv[3],"-N") != 0 || atoi(argv[4]) <= 1 || strcmp(argv[5],"-F") != 0)
	{
		perror("Wrong Input!");
		exit(EXIT_FAILURE);	
	}

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &SIGINT_handler;
	sa.sa_flags = 0;

	if((sigemptyset(&sa.sa_mask) == -1) || (sigaction(SIGINT, &sa, NULL) == -1))
	{
		perror("Failed to install signal sa_handler");
		exit(EXIT_FAILURE);
	}

	struct sigaction sas;
	memset(&sas, 0, sizeof(sas));
	sas.sa_handler = &SIGUSR1_handler;
	sas.sa_flags = 0;

	if((sigemptyset(&sas.sa_mask) == -1) || (sigaction(SIGUSR1, &sas, NULL) == -1))
	{
		perror("Failed to install signal sa_handler");
		exit(EXIT_FAILURE);
	}
	
	setbuf(stdout, NULL);
	strcpy(file_path, argv[6]);
	C = atoi(argv[2]);
	N = atoi(argv[4]);
	pthread_t supplier;
	consumer_threads = (pthread_t*) malloc(sizeof(pthread_t) * C);
	pthread_attr_t attr;
	key_t supplierKey = ftok("/semaphores", 'S');
	semaphores = semget(supplierKey, 2, 0666 | IPC_CREAT);

	if(semaphores == -1)
	{
		perror("semget Failed!");
		exit(EXIT_FAILURE);
	}
	
	unsigned short temp_arr[2];
	temp_arr[0] = 0;
	temp_arr[1] = 0;
	sem_attr.arr = temp_arr;

	if(semctl(semaphores, 0, SETALL, sem_attr) == -1)
	{
		perror("semget Failed!");
		exit(EXIT_FAILURE);
	}	

	if(pthread_attr_init(&attr) != 0)
	{
		perror("pthread_attr_init");
		exit(EXIT_FAILURE);
	}

	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{
		perror("pthread_attr_setdetachstate");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&supplier, &attr, supplier_func, (void *) argv[1]) != 0)
	{
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	if(got_SIGINT == 0)
	{
		int num[C];
		void * x;

		for(int i = 0; i < C; i++)
		{
			num[i] = i;

			if(pthread_create(&consumer_threads[i], NULL, consumer_func, (void *) &num[i]) != 0)
			{
				perror("pthread_create");
				exit(EXIT_FAILURE);
			}
		}

		for(int i = 0; i < C; i++)
		{
			if(pthread_join(consumer_threads[i], &x) != 0)
			{
				perror("pthread_join");
				exit(EXIT_FAILURE);	
			}
		}
	}

	if(pthread_attr_destroy(&attr) != 0)
	{
		perror("pthread_attr_destroy");
		exit(EXIT_FAILURE);
	}

	if(semctl(semaphores,0, IPC_RMID) == -1)
	{
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	free(consumer_threads);
}